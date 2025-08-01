#ifndef CHAT_PROTOCOL_H
#define CHAT_PROTOCOL_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define MAX_CLIENTS 32
#define MAX_USERNAME_LENGTH 32
#define MAX_MESSAGE_LENGTH 256
#define BUFFER_SIZE 512
#define DEFAULT_PORT 8080

typedef enum {
  MSG_JOIN,
  MSG_LEAVE,
  MSG_CHAT,
  MSG_SERVER_INFO,
  MSG_ERROR
} message_type_t;

typedef struct {
  message_type_t type;
  char username[MAX_USERNAME_LENGTH];
  char content[MAX_MESSAGE_LENGTH];
  time_t timestamp;
} chat_message_t;

typedef struct {
  int socket_fd;
  struct sockaddr_in address;
  char username[MAX_USERNAME_LENGTH];
  bool is_active;
  pthread_t thread_id;
} client_info_t;

void format_timestamp(time_t timestamp, char *buffer, size_t buffer_size);
int send_message_to_client(int client_socket, const chat_message_t *message);
int receive_message_from_client(int client_socket, chat_message_t *message);
void print_server_message(const char *format, ...);
void print_client_message(const char *username, const char *message);

#endif