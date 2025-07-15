#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

/**
 * Integration tests for C Monorepo applications
 * Tests real application behavior with input/output validation
 */

int tests_run = 0;
int tests_passed = 0;

#define TEST(name) \
    do { \
        printf("Running integration test: %s ... ", #name); \
        tests_run++; \
        if (name()) { \
            printf("PASS\n"); \
            tests_passed++; \
        } else { \
            printf("FAIL\n"); \
        } \
    } while(0)

/**
 * Run application with input and capture output
 */
int run_app_with_input(const char *app_path, const char *input, char *output, size_t output_size) {
    int pipe_in[2], pipe_out[2];
    pid_t pid;
    
    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        return -1;
    }
    
    pid = fork();
    if (pid == -1) {
        return -1;
    }
    
    if (pid == 0) {
        // Child process
        close(pipe_in[1]);
        close(pipe_out[0]);
        
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        dup2(pipe_out[1], STDERR_FILENO);
        
        execl(app_path, app_path, (char *)NULL);
        exit(1);
    } else {
        // Parent process
        close(pipe_in[0]);
        close(pipe_out[1]);
        
        // Send input
        if (input) {
            write(pipe_in[1], input, strlen(input));
        }
        close(pipe_in[1]);
        
        // Read output
        ssize_t bytes_read = read(pipe_out[0], output, output_size - 1);
        if (bytes_read > 0) {
            output[bytes_read] = '\0';
        } else {
            output[0] = '\0';
        }
        
        close(pipe_out[0]);
        
        int status;
        waitpid(pid, &status, 0);
        
        return WEXITSTATUS(status);
    }
}

// Test calculator basic operations
int test_calculator_basic() {
    char output[1024];
    const char *input = "+ 5 3\nquit\n";
    
    int result = run_app_with_input("../../build/release/bin/calculator", input, output, sizeof(output));
    
    return (result == 0 && strstr(output, "8.00") != NULL);
}

// Test calculator security (format string protection)
int test_calculator_security() {
    char output[1024];
    const char *input = "%s %p %n\nquit\n";
    
    int result = run_app_with_input("../../build/release/bin/calculator", input, output, sizeof(output));
    
    // Should not crash and should show error message
    return (result == 0 && strstr(output, "Error") != NULL);
}

// Test calculator factorial with overflow protection
int test_calculator_factorial_overflow() {
    char output[1024];
    const char *input = "fact 25\nquit\n";  // Should trigger overflow protection
    
    int result = run_app_with_input("../../build/release/bin/calculator", input, output, sizeof(output));
    
    return (result == 0 && strstr(output, "-1") != NULL);
}

// Test file_utils security (path traversal protection)
int test_file_utils_security() {
    char output[1024];
    const char *input = "info ../../../etc/passwd\nquit\n";
    
    int result = run_app_with_input("../../build/release/bin/file_utils", input, output, sizeof(output));
    
    // Should reject dangerous path
    return (result == 0 && strstr(output, "Invalid or dangerous path") != NULL);
}

// Test file_utils basic functionality
int test_file_utils_basic() {
    char output[1024];
    
    // Create a test file
    FILE *test_file = fopen("test_file.txt", "w");
    if (!test_file) return 0;
    fprintf(test_file, "Hello World\nTest file\n");
    fclose(test_file);
    
    const char *input = "info test_file.txt\nquit\n";
    int result = run_app_with_input("../../build/release/bin/file_utils", input, output, sizeof(output));
    
    // Clean up
    unlink("test_file.txt");
    
    return (result == 0 && strstr(output, "File Information") != NULL);
}

// Test text_processor security (buffer overflow protection)
int test_text_processor_security() {
    char output[1024];
    
    // Create very long input to test buffer overflow protection
    char long_input[2048];
    memset(long_input, 'A', sizeof(long_input) - 20);
    strcpy(long_input + sizeof(long_input) - 20, "\nquit\n");
    
    int result = run_app_with_input("../../build/release/bin/text_processor", long_input, output, sizeof(output));
    
    // Should not crash
    return (result == 0);
}

// Test text_processor basic functionality
int test_text_processor_basic() {
    char output[1024];
    const char *input = "upper hello world\nquit\n";
    
    int result = run_app_with_input("../../build/release/bin/text_processor", input, output, sizeof(output));
    
    return (result == 0 && strstr(output, "HELLO WORLD") != NULL);
}

// Test tic_tac_toe basic functionality
int test_tic_tac_toe_basic() {
    char output[1024];
    const char *input = "3\n";  // Exit option
    
    int result = run_app_with_input("../../build/release/bin/tic_tac_toe", input, output, sizeof(output));
    
    return (result == 0);
}

// Test number_guessing basic functionality
int test_number_guessing_basic() {
    char output[1024];
    const char *input = "3\n";  // Exit option
    
    int result = run_app_with_input("../../build/release/bin/number_guessing", input, output, sizeof(output));
    
    return (result == 0);
}

int main() {
    printf("Integration Test Suite\n");
    printf("======================\n\n");
    
    // Check if applications exist
    if (access("../../build/release/bin/calculator", X_OK) != 0) {
        printf("Calculator not found or not executable. Build the project first.\n");
        return 1;
    }
    
    TEST(test_calculator_basic);
    TEST(test_calculator_security);
    TEST(test_calculator_factorial_overflow);
    TEST(test_file_utils_security);
    TEST(test_file_utils_basic);
    TEST(test_text_processor_security);
    TEST(test_text_processor_basic);
    TEST(test_tic_tac_toe_basic);
    TEST(test_number_guessing_basic);
    
    printf("\nIntegration Test Results:\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("\nAll integration tests passed! ✓\n");
        return 0;
    } else {
        printf("\nSome integration tests failed! ✗\n");
        return 1;
    }
}