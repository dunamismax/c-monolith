#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 8192
#define MAX_PATH 1024
#define MAX_CLIENTS 50

typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
} client_info_t;

static volatile int server_running = 1;
static int server_fd = -1;
static char document_root[MAX_PATH];

const char* get_mime_type(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return "application/octet-stream";
    
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".json") == 0) return "application/json";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".svg") == 0) return "image/svg+xml";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".xml") == 0) return "application/xml";
    
    return "application/octet-stream";
}

void send_response(int client_fd, int status_code, const char* status_text, 
                  const char* content_type, const char* body, size_t body_length) {
    char header[BUFFER_SIZE];
    time_t now = time(NULL);
    struct tm* gmt = gmtime(&now);
    char date_buf[128];
    
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Date: %s\r\n"
        "Server: C-Monolith-WebServer/1.0\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "X-Content-Type-Options: nosniff\r\n"
        "X-Frame-Options: DENY\r\n"
        "X-XSS-Protection: 1; mode=block\r\n"
        "\r\n",
        status_code, status_text, date_buf, content_type, body_length);
    
    if (send(client_fd, header, header_len, 0) == -1) {
        perror("send header");
        return;
    }
    
    if (body && body_length > 0) {
        if (send(client_fd, body, body_length, 0) == -1) {
            perror("send body");
        }
    }
}

void send_error(int client_fd, int status_code, const char* status_text, const char* message) {
    char body[BUFFER_SIZE];
    int body_len = snprintf(body, sizeof(body),
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <title>%d %s</title>\n"
        "    <style>\n"
        "        body { font-family: Arial, sans-serif; margin: 50px; background: #1a1a1a; color: #fff; }\n"
        "        h1 { color: #ff6b6b; }\n"
        "        p { font-size: 16px; }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>%d %s</h1>\n"
        "    <p>%s</p>\n"
        "    <hr>\n"
        "    <p><em>C-Monolith WebServer/1.0</em></p>\n"
        "</body>\n"
        "</html>\n",
        status_code, status_text, status_code, status_text, message);
    
    send_response(client_fd, status_code, status_text, "text/html", body, body_len);
}

int is_safe_path(const char* path) {
    if (strstr(path, "..") != NULL) return 0;
    if (strstr(path, "//") != NULL) return 0;
    if (strstr(path, "\\") != NULL) return 0;
    if (path[0] != '/') return 0;
    return 1;
}

void serve_file(int client_fd, const char* file_path) {
    if (!is_safe_path(file_path)) {
        send_error(client_fd, 400, "Bad Request", "Invalid file path");
        return;
    }
    
    char full_path[MAX_PATH];
    if (strcmp(file_path, "/") == 0) {
        snprintf(full_path, sizeof(full_path), "%s/index.html", document_root);
    } else {
        snprintf(full_path, sizeof(full_path), "%s%s", document_root, file_path);
    }
    
    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        send_error(client_fd, 404, "Not Found", "The requested file was not found");
        return;
    }
    
    if (!S_ISREG(file_stat.st_mode)) {
        send_error(client_fd, 403, "Forbidden", "Not a regular file");
        return;
    }
    
    int file_fd = open(full_path, O_RDONLY);
    if (file_fd == -1) {
        send_error(client_fd, 500, "Internal Server Error", "Cannot open file");
        return;
    }
    
    const char* mime_type = get_mime_type(full_path);
    
    char header[BUFFER_SIZE];
    time_t now = time(NULL);
    struct tm* gmt = gmtime(&now);
    char date_buf[128];
    
    strftime(date_buf, sizeof(date_buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Date: %s\r\n"
        "Server: C-Monolith-WebServer/1.0\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %lld\r\n"
        "Connection: close\r\n"
        "X-Content-Type-Options: nosniff\r\n"
        "X-Frame-Options: DENY\r\n"
        "X-XSS-Protection: 1; mode=block\r\n"
        "\r\n",
        date_buf, mime_type, (long long)file_stat.st_size);
    
    if (send(client_fd, header, header_len, 0) == -1) {
        perror("send header");
        close(file_fd);
        return;
    }
    
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        if (send(client_fd, buffer, bytes_read, 0) == -1) {
            perror("send file data");
            break;
        }
    }
    
    close(file_fd);
}

void parse_request_line(const char* request_line, char* method, char* path, char* version) {
    if (sscanf(request_line, "%s %s %s", method, path, version) != 3) {
        strcpy(method, "INVALID");
        strcpy(path, "/");
        strcpy(version, "HTTP/1.1");
    }
}

void* handle_client(void* arg) {
    client_info_t* client_info = (client_info_t*)arg;
    int client_fd = client_info->client_fd;
    char client_ip[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, &(client_info->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received <= 0) {
        close(client_fd);
        free(client_info);
        return NULL;
    }
    
    buffer[bytes_received] = '\0';
    
    char* first_line = strtok(buffer, "\r\n");
    if (!first_line) {
        send_error(client_fd, 400, "Bad Request", "Invalid HTTP request");
        close(client_fd);
        free(client_info);
        return NULL;
    }
    
    char method[32], path[MAX_PATH], version[32];
    parse_request_line(first_line, method, path, version);
    
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);
    
    printf("[%s] %s %s %s - %s\n", timestamp, client_ip, method, path, version);
    
    if (strcmp(method, "GET") != 0) {
        send_error(client_fd, 405, "Method Not Allowed", "Only GET method is supported");
    } else {
        serve_file(client_fd, path);
    }
    
    close(client_fd);
    free(client_info);
    return NULL;
}

void signal_handler(int signum) {
    printf("\nReceived signal %d, shutting down gracefully...\n", signum);
    server_running = 0;
    if (server_fd != -1) {
        close(server_fd);
    }
}

void setup_signal_handlers(void) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char* argv[]) {
    int port = PORT;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <document_root> [port]\n", argv[0]);
        fprintf(stderr, "  document_root: Directory to serve files from\n");
        fprintf(stderr, "  port: Port number (default: %d)\n", PORT);
        return 1;
    }
    
    strncpy(document_root, argv[1], sizeof(document_root) - 1);
    document_root[sizeof(document_root) - 1] = '\0';
    
    if (argc >= 3) {
        port = atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number: %s\n", argv[2]);
            return 1;
        }
    }
    
    struct stat doc_stat;
    if (stat(document_root, &doc_stat) == -1) {
        perror("Document root directory");
        return 1;
    }
    
    if (!S_ISDIR(doc_stat.st_mode)) {
        fprintf(stderr, "Document root is not a directory: %s\n", document_root);
        return 1;
    }
    
    setup_signal_handlers();
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }
    
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("setsockopt SO_REUSEADDR");
        close(server_fd);
        return 1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }
    
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }
    
    printf("C-Monolith WebServer starting...\n");
    printf("Document root: %s\n", document_root);
    printf("Server listening on port %d\n", port);
    printf("Server URL: http://localhost:%d/\n", port);
    printf("Press Ctrl+C to stop the server\n\n");
    
    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            if (server_running) {
                perror("accept");
            }
            continue;
        }
        
        client_info_t* client_info = malloc(sizeof(client_info_t));
        if (!client_info) {
            perror("malloc");
            close(client_fd);
            continue;
        }
        
        client_info->client_fd = client_fd;
        client_info->client_addr = client_addr;
        
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        
        if (pthread_create(&thread, &attr, handle_client, client_info) != 0) {
            perror("pthread_create");
            close(client_fd);
            free(client_info);
        }
        
        pthread_attr_destroy(&attr);
    }
    
    printf("Server shutdown complete.\n");
    return 0;
}