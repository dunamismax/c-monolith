#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

/**
 * File utilities application
 * Demonstrates: file I/O, directory operations, system calls
 */

/**
 * Display file information
 */
void show_file_info(const char *filename) {
    struct stat file_stat;
    
    if (stat(filename, &file_stat) != 0) {
        printf("Error: Cannot access file '%s'\n", filename);
        return;
    }
    
    printf("\n=== File Information: %s ===\n", filename);
    printf("Size: %lld bytes\n", (long long)file_stat.st_size);
    printf("Type: ");
    
    if (S_ISREG(file_stat.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("Symbolic link\n");
    } else {
        printf("Other\n");
    }
    
    printf("Permissions: ");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    
    // Convert timestamps
    printf("Last modified: %s", ctime(&file_stat.st_mtime));
    printf("Last accessed: %s", ctime(&file_stat.st_atime));
}

/**
 * Count lines, words, and characters in a file
 */
void count_file_content(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    int lines = 0, words = 0, chars = 0;
    int in_word = 0;
    int c;
    
    while ((c = fgetc(file)) != EOF) {
        chars++;
        
        if (c == '\n') {
            lines++;
        }
        
        if (c == ' ' || c == '\t' || c == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }
    
    // If file doesn't end with newline, still count the last line
    if (chars > 0 && c != '\n') {
        lines++;
    }
    
    fclose(file);
    
    printf("\n=== File Statistics: %s ===\n", filename);
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", chars);
}

/**
 * List directory contents
 */
void list_directory(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        printf("Error: Cannot open directory '%s'\n", dirname);
        return;
    }
    
    printf("\n=== Directory Contents: %s ===\n", dirname);
    
    struct dirent *entry;
    int count = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files starting with '.'
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        printf("%s", entry->d_name);
        
        // Try to get more info about the entry
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dirname, entry->d_name);
        
        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == 0) {
            if (S_ISDIR(entry_stat.st_mode)) {
                printf("/");
            } else if (S_ISREG(entry_stat.st_mode)) {
                printf(" (%lld bytes)", (long long)entry_stat.st_size);
            }
        }
        
        printf("\n");
        count++;
    }
    
    printf("\nTotal entries: %d\n", count);
    closedir(dir);
}

/**
 * Copy file contents
 */
void copy_file(const char *source, const char *dest) {
    FILE *src = fopen(source, "rb");
    if (!src) {
        printf("Error: Cannot open source file '%s'\n", source);
        return;
    }
    
    FILE *dst = fopen(dest, "wb");
    if (!dst) {
        printf("Error: Cannot create destination file '%s'\n", dest);
        fclose(src);
        return;
    }
    
    char buffer[4096];
    size_t bytes_read;
    size_t total_bytes = 0;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dst);
        if (bytes_written != bytes_read) {
            printf("Error: Write failed\n");
            break;
        }
        total_bytes += bytes_written;
    }
    
    fclose(src);
    fclose(dst);
    
    printf("Successfully copied %zu bytes from '%s' to '%s'\n", 
           total_bytes, source, dest);
}

/**
 * Display help information
 */
void show_help() {
    printf("\n=== File Utils Help ===\n");
    printf("Commands:\n");
    printf("  info <file>        - Show file information\n");
    printf("  count <file>       - Count lines, words, chars\n");
    printf("  list <directory>   - List directory contents\n");
    printf("  copy <src> <dst>   - Copy file\n");
    printf("  help               - Show this help\n");
    printf("  quit               - Exit program\n");
    printf("\nExample: info myfile.txt\n");
    printf("Example: list /home/user\n\n");
}

int main() {
    char input[512];
    char command[64];
    char arg1[256], arg2[256];
    
    printf("=== File Utilities ===\n");
    printf("Type 'help' for commands or 'quit' to exit\n\n");
    
    while (1) {
        printf("fileutils> ");
        
        // Read input line
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(input) == 0) {
            continue;
        }
        
        // Parse command and arguments
        int parsed = sscanf(input, "%s %s %s", command, arg1, arg2);
        
        if (parsed < 1) {
            continue;
        }
        
        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(command, "help") == 0) {
            show_help();
        } else if (strcmp(command, "info") == 0) {
            if (parsed < 2) {
                printf("Usage: info <filename>\n");
            } else {
                show_file_info(arg1);
            }
        } else if (strcmp(command, "count") == 0) {
            if (parsed < 2) {
                printf("Usage: count <filename>\n");
            } else {
                count_file_content(arg1);
            }
        } else if (strcmp(command, "list") == 0) {
            if (parsed < 2) {
                list_directory(".");
            } else {
                list_directory(arg1);
            }
        } else if (strcmp(command, "copy") == 0) {
            if (parsed < 3) {
                printf("Usage: copy <source> <destination>\n");
            } else {
                copy_file(arg1, arg2);
            }
        } else {
            printf("Unknown command: %s\n", command);
            printf("Type 'help' for available commands.\n");
        }
        
        printf("\n");
    }
    
    return 0;
}