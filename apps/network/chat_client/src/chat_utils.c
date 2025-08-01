#include "chat_protocol.h"
#include <stdarg.h>

void format_timestamp(time_t timestamp, char *buffer, size_t buffer_size) {
  if (!buffer || buffer_size == 0) {
    return;
  }

  struct tm *tm_info = localtime(&timestamp);
  if (tm_info) {
    strftime(buffer, buffer_size, "%H:%M:%S", tm_info);
  } else {
    snprintf(buffer, buffer_size, "??:??:??");
  }
}

int send_message_to_client(int client_socket, const chat_message_t *message) {
  if (client_socket < 0 || !message) {
    return -1;
  }

  ssize_t bytes_sent =
      send(client_socket, message, sizeof(chat_message_t), MSG_NOSIGNAL);
  if (bytes_sent < 0) {
    return -1;
  }

  if (bytes_sent != sizeof(chat_message_t)) {
    return -1;
  }

  return bytes_sent;
}

int receive_message_from_client(int client_socket, chat_message_t *message) {
  if (client_socket < 0 || !message) {
    return -1;
  }

  memset(message, 0, sizeof(chat_message_t));

  ssize_t bytes_received =
      recv(client_socket, message, sizeof(chat_message_t), 0);
  if (bytes_received <= 0) {
    return bytes_received;
  }

  if (bytes_received != sizeof(chat_message_t)) {
    return -1;
  }

  message->username[MAX_USERNAME_LENGTH - 1] = '\0';
  message->content[MAX_MESSAGE_LENGTH - 1] = '\0';

  return bytes_received;
}

void print_server_message(const char *format, ...) {
  time_t now = time(NULL);
  char timestamp_str[64];
  format_timestamp(now, timestamp_str, sizeof(timestamp_str));

  printf("[%s] SERVER: ", timestamp_str);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\n");
  fflush(stdout);
}

void print_client_message(const char *username, const char *message) {
  time_t now = time(NULL);
  char timestamp_str[64];
  format_timestamp(now, timestamp_str, sizeof(timestamp_str));

  printf("[%s] %s: %s\n", timestamp_str, username, message);
  fflush(stdout);
}