#include "chat_protocol.h"

static client_info_t clients[MAX_CLIENTS];
static pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile bool server_running = true;
static int server_socket = -1;
static int shutdown_pipe[2] = {-1, -1};

void signal_handler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    print_server_message("Shutdown signal received. Cleaning up...");
    server_running = false;

    char byte = 1;
    if (shutdown_pipe[1] >= 0) {
      write(shutdown_pipe[1], &byte, 1);
    }
  }
}

int find_free_client_slot(void) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (!clients[i].is_active) {
      return i;
    }
  }
  return -1;
}

void broadcast_message(const chat_message_t *message, int sender_index) {
  pthread_mutex_lock(&clients_mutex);

  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i].is_active && i != sender_index) {
      if (send_message_to_client(clients[i].socket_fd, message) < 0) {
        print_server_message("Failed to send message to client %s",
                             clients[i].username);
      }
    }
  }

  pthread_mutex_unlock(&clients_mutex);
}

void broadcast_user_joined(const char *username) {
  chat_message_t message = {0};
  message.type = MSG_SERVER_INFO;
  message.timestamp = time(NULL);
  snprintf(message.content, sizeof(message.content), "%s has joined the chat",
           username);
  strcpy(message.username, "SERVER");

  broadcast_message(&message, -1);
  print_server_message("User %s joined the chat", username);
}

void broadcast_user_left(const char *username) {
  chat_message_t message = {0};
  message.type = MSG_SERVER_INFO;
  message.timestamp = time(NULL);
  snprintf(message.content, sizeof(message.content), "%s has left the chat",
           username);
  strcpy(message.username, "SERVER");

  broadcast_message(&message, -1);
  print_server_message("User %s left the chat", username);
}

void cleanup_client(int client_index) {
  pthread_mutex_lock(&clients_mutex);

  if (clients[client_index].is_active) {
    broadcast_user_left(clients[client_index].username);
    close(clients[client_index].socket_fd);
    memset(&clients[client_index], 0, sizeof(client_info_t));
    clients[client_index].is_active = false;
  }

  pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
  int client_index = *(int *)arg;
  free(arg);

  client_info_t *client = &clients[client_index];
  chat_message_t message;

  char welcome_buffer[BUFFER_SIZE];
  snprintf(
      welcome_buffer, sizeof(welcome_buffer),
      "Welcome to the chat server, %s! Type your messages and press Enter.",
      client->username);

  chat_message_t welcome_msg = {0};
  welcome_msg.type = MSG_SERVER_INFO;
  welcome_msg.timestamp = time(NULL);
  strcpy(welcome_msg.username, "SERVER");
  strcpy(welcome_msg.content, welcome_buffer);

  if (send_message_to_client(client->socket_fd, &welcome_msg) < 0) {
    print_server_message("Failed to send welcome message to %s",
                         client->username);
    cleanup_client(client_index);
    return NULL;
  }

  broadcast_user_joined(client->username);

  while (server_running && client->is_active) {
    int result = receive_message_from_client(client->socket_fd, &message);

    if (result <= 0) {
      if (result < 0) {
        print_server_message("Error receiving message from %s: %s",
                             client->username, strerror(errno));
      }
      break;
    }

    if (message.type == MSG_CHAT) {
      message.timestamp = time(NULL);
      print_client_message(message.username, message.content);
      broadcast_message(&message, client_index);
    } else if (message.type == MSG_LEAVE) {
      print_server_message("Client %s requested to leave", client->username);
      break;
    }
  }

  cleanup_client(client_index);
  return NULL;
}

bool setup_server_socket(int port) {
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Socket creation failed");
    return false;
  }

  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    perror("Setsockopt failed");
    close(server_socket);
    return false;
  }

  struct sockaddr_in server_addr = {0};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close(server_socket);
    return false;
  }

  if (listen(server_socket, MAX_CLIENTS) < 0) {
    perror("Listen failed");
    close(server_socket);
    return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  int port = DEFAULT_PORT;

  if (argc > 1) {
    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
      fprintf(stderr, "Invalid port number. Using default port %d\n",
              DEFAULT_PORT);
      port = DEFAULT_PORT;
    }
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGPIPE, SIG_IGN);

  if (pipe(shutdown_pipe) == -1) {
    perror("Failed to create shutdown pipe");
    return EXIT_FAILURE;
  }

  memset(clients, 0, sizeof(clients));

  if (!setup_server_socket(port)) {
    fprintf(stderr, "Failed to setup server socket\n");
    return EXIT_FAILURE;
  }

  print_server_message("Realtime Chat Server started on port %d", port);
  print_server_message("Waiting for clients to connect...");
  print_server_message("Press Ctrl+C to shutdown the server");

  while (server_running) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);
    FD_SET(shutdown_pipe[0], &read_fds);

    int max_fd =
        (server_socket > shutdown_pipe[0]) ? server_socket : shutdown_pipe[0];

    int select_result = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if (select_result < 0) {
      if (server_running) {
        perror("Select failed");
      }
      break;
    }

    if (FD_ISSET(shutdown_pipe[0], &read_fds)) {
      break;
    }

    if (!FD_ISSET(server_socket, &read_fds)) {
      continue;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
      if (server_running) {
        perror("Accept failed");
      }
      continue;
    }

    int client_index = find_free_client_slot();
    if (client_index < 0) {
      print_server_message(
          "Maximum clients reached. Connection rejected from %s",
          inet_ntoa(client_addr.sin_addr));

      chat_message_t reject_msg = {0};
      reject_msg.type = MSG_ERROR;
      reject_msg.timestamp = time(NULL);
      strcpy(reject_msg.username, "SERVER");
      strcpy(reject_msg.content, "Server is full. Please try again later.");

      send_message_to_client(client_socket, &reject_msg);
      close(client_socket);
      continue;
    }

    chat_message_t join_request;
    if (receive_message_from_client(client_socket, &join_request) <= 0 ||
        join_request.type != MSG_JOIN) {
      print_server_message("Invalid join request from %s",
                           inet_ntoa(client_addr.sin_addr));
      close(client_socket);
      continue;
    }

    if (strlen(join_request.username) == 0 ||
        strlen(join_request.username) >= MAX_USERNAME_LENGTH) {
      print_server_message("Invalid username from %s",
                           inet_ntoa(client_addr.sin_addr));

      chat_message_t error_msg = {0};
      error_msg.type = MSG_ERROR;
      error_msg.timestamp = time(NULL);
      strcpy(error_msg.username, "SERVER");
      strcpy(error_msg.content,
             "Invalid username. Please use 1-31 characters.");

      send_message_to_client(client_socket, &error_msg);
      close(client_socket);
      continue;
    }

    pthread_mutex_lock(&clients_mutex);
    bool username_taken = false;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (clients[i].is_active &&
          strcmp(clients[i].username, join_request.username) == 0) {
        username_taken = true;
        break;
      }
    }
    pthread_mutex_unlock(&clients_mutex);

    if (username_taken) {
      print_server_message("Username '%s' already taken",
                           join_request.username);

      chat_message_t error_msg = {0};
      error_msg.type = MSG_ERROR;
      error_msg.timestamp = time(NULL);
      strcpy(error_msg.username, "SERVER");
      strcpy(error_msg.content,
             "Username already taken. Please choose another.");

      send_message_to_client(client_socket, &error_msg);
      close(client_socket);
      continue;
    }

    pthread_mutex_lock(&clients_mutex);
    clients[client_index].socket_fd = client_socket;
    clients[client_index].address = client_addr;
    strcpy(clients[client_index].username, join_request.username);
    clients[client_index].is_active = true;
    pthread_mutex_unlock(&clients_mutex);

    int *thread_arg = malloc(sizeof(int));
    *thread_arg = client_index;

    if (pthread_create(&clients[client_index].thread_id, NULL, handle_client,
                       thread_arg) != 0) {
      print_server_message("Failed to create thread for client %s",
                           join_request.username);
      cleanup_client(client_index);
      free(thread_arg);
      continue;
    }

    pthread_detach(clients[client_index].thread_id);
  }

  print_server_message("Shutting down server...");

  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i].is_active) {
      shutdown(clients[i].socket_fd, SHUT_RDWR);
      close(clients[i].socket_fd);
      clients[i].is_active = false;
    }
  }
  pthread_mutex_unlock(&clients_mutex);

  if (server_socket >= 0) {
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
  }

  if (shutdown_pipe[0] >= 0)
    close(shutdown_pipe[0]);
  if (shutdown_pipe[1] >= 0)
    close(shutdown_pipe[1]);

  sleep(1);

  print_server_message("Server shutdown complete");
  return EXIT_SUCCESS;
}