#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Include our math utilities library
#include "../../../../libs/math_utils/include/math_utils.h"

/**
 * Simple command-line calculator
 * Demonstrates: basic I/O, string parsing, arithmetic operations
 * Uses the math_utils library for extended operations
 */

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_POW,
    OP_GCD,
    OP_LCM,
    OP_FACTORIAL,
    OP_FIBONACCI,
    OP_ISPRIME,
    OP_SQRT,
    OP_QUIT,
    OP_HELP,
    OP_INVALID
} Operation;

/**
 * Parse operation from string
 */
Operation parse_operation(const char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0) return OP_ADD;
    if (strcmp(op, "-") == 0 || strcmp(op, "sub") == 0) return OP_SUB;
    if (strcmp(op, "*") == 0 || strcmp(op, "mul") == 0) return OP_MUL;
    if (strcmp(op, "/") == 0 || strcmp(op, "div") == 0) return OP_DIV;
    if (strcmp(op, "%") == 0 || strcmp(op, "mod") == 0) return OP_MOD;
    if (strcmp(op, "^") == 0 || strcmp(op, "pow") == 0) return OP_POW;
    if (strcmp(op, "gcd") == 0) return OP_GCD;
    if (strcmp(op, "lcm") == 0) return OP_LCM;
    if (strcmp(op, "!") == 0 || strcmp(op, "fact") == 0) return OP_FACTORIAL;
    if (strcmp(op, "fib") == 0) return OP_FIBONACCI;
    if (strcmp(op, "prime") == 0) return OP_ISPRIME;
    if (strcmp(op, "sqrt") == 0) return OP_SQRT;
    if (strcmp(op, "quit") == 0 || strcmp(op, "exit") == 0) return OP_QUIT;
    if (strcmp(op, "help") == 0 || strcmp(op, "?") == 0) return OP_HELP;
    return OP_INVALID;
}

/**
 * Display help information
 */
void show_help() {
    printf("\n=== Calculator Help ===\n");
    printf("Basic Operations:\n");
    printf("  +, add      - Addition (a + b)\n");
    printf("  -, sub      - Subtraction (a - b)\n");
    printf("  *, mul      - Multiplication (a * b)\n");
    printf("  /, div      - Division (a / b)\n");
    printf("  %%, mod      - Modulo (a %% b)\n");
    printf("  ^, pow      - Power (a ^ b)\n");
    printf("\nAdvanced Operations:\n");
    printf("  gcd         - Greatest Common Divisor\n");
    printf("  lcm         - Least Common Multiple\n");
    printf("  !, fact     - Factorial (single number)\n");
    printf("  fib         - Fibonacci (single number)\n");
    printf("  prime       - Check if prime (single number)\n");
    printf("  sqrt        - Square root (single number)\n");
    printf("\nCommands:\n");
    printf("  help, ?     - Show this help\n");
    printf("  quit, exit  - Exit calculator\n");
    printf("\nUsage: <operation> <number1> [number2]\n");
    printf("Example: + 5 3\n");
    printf("Example: fact 5\n\n");
}

/**
 * Process calculator operation
 */
void process_operation(Operation op, double a, double b) {
    switch (op) {
        case OP_ADD:
            printf("Result: %.2f\n", a + b);
            break;
        case OP_SUB:
            printf("Result: %.2f\n", a - b);
            break;
        case OP_MUL:
            printf("Result: %.2f\n", a * b);
            break;
        case OP_DIV:
            if (b == 0) {
                printf("Error: Division by zero!\n");
            } else {
                printf("Result: %.2f\n", a / b);
            }
            break;
        case OP_MOD:
            if (b == 0) {
                printf("Error: Modulo by zero!\n");
            } else {
                printf("Result: %d\n", (int)a % (int)b);
            }
            break;
        case OP_POW:
            printf("Result: %lld\n", math_power((int)a, (int)b));
            break;
        case OP_GCD:
            printf("Result: %d\n", math_gcd((int)a, (int)b));
            break;
        case OP_LCM:
            printf("Result: %d\n", math_lcm((int)a, (int)b));
            break;
        case OP_FACTORIAL:
            {
                long long result = math_factorial((int)a);
                if (result == -1) {
                    printf("Error: Factorial undefined for negative numbers!\n");
                } else {
                    printf("Result: %lld\n", result);
                }
            }
            break;
        case OP_FIBONACCI:
            {
                long long result = math_fibonacci((int)a);
                if (result == -1) {
                    printf("Error: Invalid input for Fibonacci!\n");
                } else {
                    printf("Result: %lld\n", result);
                }
            }
            break;
        case OP_ISPRIME:
            if (math_is_prime((int)a)) {
                printf("Result: %d is prime\n", (int)a);
            } else {
                printf("Result: %d is not prime\n", (int)a);
            }
            break;
        case OP_SQRT:
            {
                double result = math_sqrt(a);
                if (result == -1) {
                    printf("Error: Square root of negative number!\n");
                } else {
                    printf("Result: %.6f\n", result);
                }
            }
            break;
        default:
            printf("Error: Unknown operation!\n");
            break;
    }
}

int main() {
    char input[256];
    char op_str[32];
    double num1, num2;
    
    printf("=== C Programming Calculator ===\n");
    printf("Type 'help' for instructions or 'quit' to exit\n\n");
    
    while (1) {
        printf("calc> ");
        
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
        
        // Parse input
        int parsed = sscanf(input, "%s %lf %lf", op_str, &num1, &num2);
        
        if (parsed < 1) {
            printf("Error: Invalid input. Type 'help' for instructions.\n");
            continue;
        }
        
        Operation op = parse_operation(op_str);
        
        if (op == OP_QUIT) {
            printf("Goodbye!\n");
            break;
        }
        
        if (op == OP_HELP) {
            show_help();
            continue;
        }
        
        if (op == OP_INVALID) {
            printf("Error: Unknown operation '%s'. Type 'help' for list.\n", op_str);
            continue;
        }
        
        // Check if we have enough operands
        if ((op == OP_FACTORIAL || op == OP_FIBONACCI || op == OP_ISPRIME || op == OP_SQRT) && parsed < 2) {
            printf("Error: Missing operand. Usage: %s <number>\n", op_str);
            continue;
        }
        
        if ((op != OP_FACTORIAL && op != OP_FIBONACCI && op != OP_ISPRIME && op != OP_SQRT) && parsed < 3) {
            printf("Error: Missing operand. Usage: %s <number1> <number2>\n", op_str);
            continue;
        }
        
        // Process the operation
        process_operation(op, num1, num2);
        printf("\n");
    }
    
    return 0;
}