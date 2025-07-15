#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Include our data structures library
#include "../../../../libs/data_structures/include/vector.h"

/**
 * Text processing application
 * Demonstrates: string manipulation, text processing algorithms
 * Uses the vector data structure for storing lines
 */

#define MAX_LINE_LENGTH 1024

/**
 * Convert string to uppercase
 */
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

/**
 * Convert string to lowercase
 */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

/**
 * Reverse a string in place
 */
void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

/**
 * Count occurrences of a character in a string
 */
int count_char(const char *str, char c) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == c) {
            count++;
        }
    }
    return count;
}

/**
 * Remove extra whitespace from string
 */
void trim_whitespace(char *str) {
    // Remove leading whitespace
    int start = 0;
    while (str[start] && isspace(str[start])) {
        start++;
    }
    
    // Shift string to remove leading whitespace
    memmove(str, str + start, strlen(str + start) + 1);
    
    // Remove trailing whitespace
    int end = strlen(str) - 1;
    while (end >= 0 && isspace(str[end])) {
        str[end] = '\0';
        end--;
    }
    
    // Remove extra internal whitespace
    int read = 0, write = 0;
    int in_space = 0;
    
    while (str[read]) {
        if (isspace(str[read])) {
            if (!in_space) {
                str[write++] = ' ';
                in_space = 1;
            }
        } else {
            str[write++] = str[read];
            in_space = 0;
        }
        read++;
    }
    str[write] = '\0';
}

/**
 * Find and replace substring in text
 */
char* find_replace(const char *text, const char *find, const char *replace) {
    if (!text || !find || !replace) {
        return NULL;
    }
    
    size_t find_len = strlen(find);
    size_t replace_len = strlen(replace);
    size_t text_len = strlen(text);
    
    // Count occurrences
    int count = 0;
    const char *pos = text;
    while ((pos = strstr(pos, find)) != NULL) {
        count++;
        pos += find_len;
    }
    
    // Calculate new string size
    size_t new_size = text_len + count * (replace_len - find_len) + 1;
    char *result = malloc(new_size);
    if (!result) {
        return NULL;
    }
    
    // Perform replacement
    const char *src = text;
    char *dst = result;
    
    while ((pos = strstr(src, find)) != NULL) {
        // Copy text before match
        size_t prefix_len = pos - src;
        memcpy(dst, src, prefix_len);
        dst += prefix_len;
        
        // Copy replacement
        memcpy(dst, replace, replace_len);
        dst += replace_len;
        
        // Move past the match
        src = pos + find_len;
    }
    
    // Copy remaining text
    strcpy(dst, src);
    
    return result;
}

/**
 * Sort lines alphabetically (simple bubble sort for educational purposes)
 */
void sort_lines(char lines[][MAX_LINE_LENGTH], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (strcmp(lines[j], lines[j + 1]) > 0) {
                char temp[MAX_LINE_LENGTH];
                strcpy(temp, lines[j]);
                strcpy(lines[j], lines[j + 1]);
                strcpy(lines[j + 1], temp);
            }
        }
    }
}

/**
 * Display help information
 */
void show_help() {
    printf("\n=== Text Processor Help ===\n");
    printf("Commands:\n");
    printf("  upper <text>         - Convert to uppercase\n");
    printf("  lower <text>         - Convert to lowercase\n");
    printf("  reverse <text>       - Reverse text\n");
    printf("  count <char> <text>  - Count character occurrences\n");
    printf("  trim <text>          - Remove extra whitespace\n");
    printf("  replace <find> <replace> <text> - Find and replace\n");
    printf("  sort                 - Enter sort mode for lines\n");
    printf("  help                 - Show this help\n");
    printf("  quit                 - Exit program\n");
    printf("\nExample: upper Hello World\n");
    printf("Example: count a banana\n");
    printf("Example: replace old new This is old text\n\n");
}

/**
 * Interactive sorting mode
 */
void sort_mode() {
    printf("\n=== Sort Mode ===\n");
    printf("Enter lines of text (empty line to finish):\n");
    
    char lines[100][MAX_LINE_LENGTH];
    int count = 0;
    
    while (count < 100) {
        printf("%d> ", count + 1);
        
        if (!fgets(lines[count], MAX_LINE_LENGTH, stdin)) {
            break;
        }
        
        // Remove newline
        lines[count][strcspn(lines[count], "\n")] = 0;
        
        // Empty line signals end
        if (strlen(lines[count]) == 0) {
            break;
        }
        
        count++;
    }
    
    if (count == 0) {
        printf("No lines to sort.\n");
        return;
    }
    
    printf("\nOriginal lines:\n");
    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", i + 1, lines[i]);
    }
    
    sort_lines(lines, count);
    
    printf("\nSorted lines:\n");
    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", i + 1, lines[i]);
    }
}

int main() {
    char input[MAX_LINE_LENGTH];
    char command[64];
    char text[MAX_LINE_LENGTH];
    
    printf("=== Text Processor ===\n");
    printf("Type 'help' for commands or 'quit' to exit\n\n");
    
    while (1) {
        printf("textproc> ");
        
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
        
        // Parse command
        int parsed = sscanf(input, "%s", command);
        if (parsed < 1) {
            continue;
        }
        
        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(command, "help") == 0) {
            show_help();
        } else if (strcmp(command, "sort") == 0) {
            sort_mode();
        } else if (strcmp(command, "upper") == 0) {
            // Extract text after command
            char *text_start = input + strlen(command);
            while (*text_start && isspace(*text_start)) text_start++;
            
            if (strlen(text_start) == 0) {
                printf("Usage: upper <text>\n");
            } else {
                strcpy(text, text_start);
                to_uppercase(text);
                printf("Result: %s\n", text);
            }
        } else if (strcmp(command, "lower") == 0) {
            char *text_start = input + strlen(command);
            while (*text_start && isspace(*text_start)) text_start++;
            
            if (strlen(text_start) == 0) {
                printf("Usage: lower <text>\n");
            } else {
                strcpy(text, text_start);
                to_lowercase(text);
                printf("Result: %s\n", text);
            }
        } else if (strcmp(command, "reverse") == 0) {
            char *text_start = input + strlen(command);
            while (*text_start && isspace(*text_start)) text_start++;
            
            if (strlen(text_start) == 0) {
                printf("Usage: reverse <text>\n");
            } else {
                strcpy(text, text_start);
                reverse_string(text);
                printf("Result: %s\n", text);
            }
        } else if (strcmp(command, "count") == 0) {
            char target_char;
            char *text_start = strchr(input + strlen(command), ' ');
            if (text_start) {
                text_start++;
                if (*text_start) {
                    target_char = *text_start;
                    text_start += 2; // Skip char and space
                    
                    if (*text_start) {
                        int count = count_char(text_start, target_char);
                        printf("Character '%c' appears %d times\n", target_char, count);
                    } else {
                        printf("Usage: count <char> <text>\n");
                    }
                } else {
                    printf("Usage: count <char> <text>\n");
                }
            } else {
                printf("Usage: count <char> <text>\n");
            }
        } else if (strcmp(command, "trim") == 0) {
            char *text_start = input + strlen(command);
            while (*text_start && isspace(*text_start)) text_start++;
            
            if (strlen(text_start) == 0) {
                printf("Usage: trim <text>\n");
            } else {
                strcpy(text, text_start);
                trim_whitespace(text);
                printf("Result: '%s'\n", text);
            }
        } else if (strcmp(command, "replace") == 0) {
            char find[256], replace[256], source[512];
            int args = sscanf(input, "%*s %s %s %[^\n]", find, replace, source);
            
            if (args < 3) {
                printf("Usage: replace <find> <replace> <text>\n");
            } else {
                char *result = find_replace(source, find, replace);
                if (result) {
                    printf("Result: %s\n", result);
                    free(result);
                } else {
                    printf("Error: Memory allocation failed\n");
                }
            }
        } else {
            printf("Unknown command: %s\n", command);
            printf("Type 'help' for available commands.\n");
        }
        
        printf("\n");
    }
    
    return 0;
}