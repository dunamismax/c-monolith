#include "chat_protocol.h"

static volatile bool client_running = true;
static int client_socket = -1;
static char username[MAX_USERNAME_LENGTH];

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        printf("\n\nDisconnecting from chat server...\n");
        client_running = false;
        if (client_socket >= 0) {
            chat_message_t leave_msg = {0};
            leave_msg.type = MSG_LEAVE;
            leave_msg.timestamp = time(NULL);
            strcpy(leave_msg.username, username);
            strcpy(leave_msg.content, "User disconnected");
            
            send_message_to_client(client_socket, &leave_msg);
            shutdown(client_socket, SHUT_RDWR);
            close(client_socket);
            client_socket = -1;
        }
        exit(EXIT_SUCCESS);
    }
}

void* receive_messages(void* arg) {
    (void)arg;
    chat_message_t message;
    
    while (client_running) {
        int result = receive_message_from_client(client_socket, &message);
        
        if (result <= 0) {
            if (client_running) {
                if (result < 0) {
                    printf("\nError receiving message from server: %s\n", strerror(errno));
                } else {
                    printf("\nServer disconnected\n");
                }
                client_running = false;
            }
            break;
        }
        
        char timestamp_str[64];
        format_timestamp(message.timestamp, timestamp_str, sizeof(timestamp_str));
        
        switch (message.type) {
            case MSG_CHAT:
                printf("\r[%s] %s: %s\n> ", timestamp_str, message.username, message.content);
                fflush(stdout);
                break;
                
            case MSG_SERVER_INFO:
                printf("\r*** [%s] %s ***\n> ", timestamp_str, message.content);
                fflush(stdout);
                break;
                
            case MSG_ERROR:
                printf("\rERROR: %s\n> ", message.content);
                fflush(stdout);
                break;
                
            default:
                break;
        }
    }
    
    return NULL;
}

bool connect_to_server(const char* server_ip, int port) {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return false;
    }
    
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address: %s\n", server_ip);
        close(client_socket);
        return false;
    }
    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        return false;
    }
    
    return true;
}

bool join_chat_room(void) {
    chat_message_t join_msg = {0};
    join_msg.type = MSG_JOIN;
    join_msg.timestamp = time(NULL);
    strcpy(join_msg.username, username);
    strcpy(join_msg.content, "User joining");
    
    if (send_message_to_client(client_socket, &join_msg) < 0) {
        printf("Failed to send join request\n");
        return false;
    }
    
    chat_message_t response;
    if (receive_message_from_client(client_socket, &response) <= 0) {
        printf("Failed to receive join response\n");
        return false;
    }
    
    if (response.type == MSG_ERROR) {
        printf("Join failed: %s\n", response.content);
        return false;
    }
    
    return true;
}

void print_help(void) {
    printf("\n=== Realtime Chat Client ===\n");
    printf("Commands:\n");
    printf("  /help    - Show this help message\n");
    printf("  /quit    - Exit the chat\n");
    printf("  /users   - List online users (server feature)\n");
    printf("  <message> - Send a message to all users\n");
    printf("\nJust type your message and press Enter to chat!\n\n");
}

bool get_user_input(char* buffer, size_t buffer_size) {
    if (!fgets(buffer, buffer_size, stdin)) {
        return false;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0;
}

int main(int argc, char* argv[]) {
    char server_ip[INET_ADDRSTRLEN] = "127.0.0.1";
    int port = DEFAULT_PORT;
    
    if (argc > 1) {
        strncpy(server_ip, argv[1], sizeof(server_ip) - 1);
        server_ip[sizeof(server_ip) - 1] = '\0';
    }
    
    if (argc > 2) {
        port = atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number. Using default port %d\n", DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    }
    
    printf("=== Realtime Chat Client ===\n");
    printf("Server: %s:%d\n\n", server_ip, port);
    
    printf("Enter your username (1-31 characters): ");
    fflush(stdout);
    
    if (!get_user_input(username, sizeof(username))) {
        printf("Invalid username\n");
        return EXIT_FAILURE;
    }
    
    if (strlen(username) == 0 || strlen(username) >= MAX_USERNAME_LENGTH) {
        printf("Username must be 1-31 characters long\n");
        return EXIT_FAILURE;
    }
    
    printf("Connecting to server %s:%d...\n", server_ip, port);
    
    if (!connect_to_server(server_ip, port)) {
        return EXIT_FAILURE;
    }
    
    printf("Connected! Joining chat room...\n");
    
    if (!join_chat_room()) {
        close(client_socket);
        return EXIT_FAILURE;
    }
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);
    
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        printf("Failed to create receive thread\n");
        close(client_socket);
        return EXIT_FAILURE;
    }
    
    printf("\nWelcome to the chat! Type '/help' for commands.\n");
    printf("You can start chatting now:\n\n");
    
    char input_buffer[MAX_MESSAGE_LENGTH];
    
    while (client_running) {
        printf("> ");
        fflush(stdout);
        
        if (!get_user_input(input_buffer, sizeof(input_buffer))) {
            continue;
        }
        
        if (strcmp(input_buffer, "/quit") == 0 || strcmp(input_buffer, "/exit") == 0) {
            printf("Leaving chat...\n");
            break;
        }
        
        if (strcmp(input_buffer, "/help") == 0) {
            print_help();
            continue;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        if (input_buffer[0] == '/') {
            printf("Unknown command: %s (type '/help' for available commands)\n", input_buffer);
            continue;
        }
        
        chat_message_t message = {0};
        message.type = MSG_CHAT;
        message.timestamp = time(NULL);
        strcpy(message.username, username);
        strncpy(message.content, input_buffer, sizeof(message.content) - 1);
        message.content[sizeof(message.content) - 1] = '\0';
        
        if (send_message_to_client(client_socket, &message) < 0) {
            printf("Failed to send message\n");
            break;
        }
    }
    
    client_running = false;
    
    chat_message_t leave_msg = {0};
    leave_msg.type = MSG_LEAVE;
    leave_msg.timestamp = time(NULL);
    strcpy(leave_msg.username, username);
    strcpy(leave_msg.content, "User disconnected");
    
    send_message_to_client(client_socket, &leave_msg);
    
    pthread_cancel(receive_thread);
    pthread_join(receive_thread, NULL);
    
    close(client_socket);
    printf("Disconnected from chat server. Goodbye!\n");
    
    return EXIT_SUCCESS;
}