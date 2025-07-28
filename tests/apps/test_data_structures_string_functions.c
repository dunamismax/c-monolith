#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the test framework
#include "../test_framework.h"

// Include the data structures header
#include "../../apps/data/data_structures_demo/src/data_structures.h"

// Test string length function
int test_string_length(void) {
    if (string_length("hello") != 5) return 0;
    if (string_length("") != 0) return 0;
    if (string_length("a") != 1) return 0;
    if (string_length("Hello, World!") != 13) return 0;
    if (string_length(NULL) != 0) return 0;
    return 1;
}

// Test string copy function
int test_string_copy(void) {
    char dest[50];
    
    string_copy("hello", dest);
    if (strcmp(dest, "hello") != 0) return 0;
    
    string_copy("", dest);
    if (strcmp(dest, "") != 0) return 0;
    
    string_copy("Hello, World!", dest);
    if (strcmp(dest, "Hello, World!") != 0) return 0;
    
    // Test with NULL (should handle gracefully)
    string_copy(NULL, dest);  // Should not crash
    string_copy("test", NULL);  // Should not crash
    
    return 1;
}

// Test string compare function
int test_string_compare(void) {
    // Equal strings
    if (string_compare("hello", "hello") != 0) return 0;
    if (string_compare("", "") != 0) return 0;
    
    // First string less than second
    if (string_compare("apple", "banana") >= 0) return 0;
    if (string_compare("a", "b") >= 0) return 0;
    
    // First string greater than second
    if (string_compare("banana", "apple") <= 0) return 0;
    if (string_compare("b", "a") <= 0) return 0;
    
    // Different lengths
    if (string_compare("cat", "cats") >= 0) return 0;
    if (string_compare("cats", "cat") <= 0) return 0;
    
    // Test with NULL (should handle gracefully)
    if (string_compare(NULL, "test") != 0) return 0;
    if (string_compare("test", NULL) != 0) return 0;
    if (string_compare(NULL, NULL) != 0) return 0;
    
    return 1;
}

// Test string concatenate function
int test_string_concatenate(void) {
    char* result;
    
    result = string_concatenate("hello", " world");
    if (!result || strcmp(result, "hello world") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    result = string_concatenate("", "test");
    if (!result || strcmp(result, "test") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    result = string_concatenate("test", "");
    if (!result || strcmp(result, "test") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    result = string_concatenate("", "");
    if (!result || strcmp(result, "") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test with NULL
    result = string_concatenate(NULL, "test");
    if (result != NULL) {
        free(result);
        return 0;
    }
    
    result = string_concatenate("test", NULL);
    if (result != NULL) {
        free(result);
        return 0;
    }
    
    return 1;
}

// Test integer to string conversion
int test_integer_to_string(void) {
    char* result;
    
    // Test positive numbers
    result = integer_to_string(123);
    if (!result || strcmp(result, "123") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test zero
    result = integer_to_string(0);
    if (!result || strcmp(result, "0") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test negative numbers
    result = integer_to_string(-456);
    if (!result || strcmp(result, "-456") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test single digit
    result = integer_to_string(7);
    if (!result || strcmp(result, "7") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test large number
    result = integer_to_string(9999);
    if (!result || strcmp(result, "9999") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    return 1;
}

// Test string to integer conversion
int test_string_to_integer(void) {
    // Test positive numbers
    if (string_to_integer("123") != 123) return 0;
    if (string_to_integer("0") != 0) return 0;
    if (string_to_integer("1") != 1) return 0;
    
    // Test negative numbers
    if (string_to_integer("-456") != -456) return 0;
    if (string_to_integer("-1") != -1) return 0;
    
    // Test with plus sign
    if (string_to_integer("+789") != 789) return 0;
    
    // Test invalid strings (should return 0)
    if (string_to_integer("abc") != 0) return 0;
    if (string_to_integer("12abc") != 0) return 0;
    if (string_to_integer("") != 0) return 0;
    if (string_to_integer(NULL) != 0) return 0;
    
    return 1;
}

// Test is_valid_integer function
int test_is_valid_integer(void) {
    // Valid integers
    if (!is_valid_integer("123")) return 0;
    if (!is_valid_integer("0")) return 0;
    if (!is_valid_integer("-456")) return 0;
    if (!is_valid_integer("+789")) return 0;
    if (!is_valid_integer("1")) return 0;
    
    // Invalid integers
    if (is_valid_integer("abc")) return 0;
    if (is_valid_integer("12abc")) return 0;
    if (is_valid_integer("")) return 0;
    if (is_valid_integer("-")) return 0;
    if (is_valid_integer("+")) return 0;
    if (is_valid_integer(" 123")) return 0;  // Leading space
    if (is_valid_integer("123 ")) return 0;  // Trailing space
    if (is_valid_integer(NULL)) return 0;
    
    return 1;
}

// Test string functions with special characters
int test_special_characters(void) {
    // Test with special characters
    if (string_length("Hello\nWorld") != 11) return 0;
    if (string_length("Tab\tCharacter") != 13) return 0;  // "Tab" + "\t" + "Character" = 3 + 1 + 9 = 13
    
    char dest[50];
    string_copy("Special\nChars", dest);
    if (strcmp(dest, "Special\nChars") != 0) return 0;
    
    // Test concatenation with special chars
    char* result = string_concatenate("Line1\n", "Line2");
    if (!result || strcmp(result, "Line1\nLine2") != 0) {
        free(result);
        return 0;
    }
    free(result);
    
    return 1;
}

// Test string functions with long strings
int test_long_strings(void) {
    char long_str[1000];
    char dest[1500];
    
    // Create a long string
    for (int i = 0; i < 999; i++) {
        long_str[i] = 'a' + (i % 26);
    }
    long_str[999] = '\0';
    
    if (string_length(long_str) != 999) return 0;
    
    string_copy(long_str, dest);
    if (strcmp(dest, long_str) != 0) return 0;
    
    char* result = string_concatenate(long_str, "END");
    if (!result) return 0;
    if (string_length(result) != 1002) {
        free(result);
        return 0;
    }
    free(result);
    
    return 1;
}

// Test edge cases and boundary conditions
int test_edge_cases(void) {
    // Test very large integers (within int range)
    char* result = integer_to_string(2147483647);  // INT_MAX
    if (!result) return 0;
    int back = string_to_integer(result);
    if (back != 2147483647) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test very small integers
    result = integer_to_string(-2147483647);
    if (!result) return 0;
    back = string_to_integer(result);
    if (back != -2147483647) {
        free(result);
        return 0;
    }
    free(result);
    
    // Test string comparison edge cases
    if (string_compare("a", "aa") >= 0) return 0;
    if (string_compare("aa", "a") <= 0) return 0;
    
    return 1;
}

int main(void) {
    printf("Data Structures Demo - String Functions Test Suite\n");
    printf("===================================================\n\n");

    run_test(test_string_length, "test_string_length");
    run_test(test_string_copy, "test_string_copy");
    run_test(test_string_compare, "test_string_compare");
    run_test(test_string_concatenate, "test_string_concatenate");
    run_test(test_integer_to_string, "test_integer_to_string");
    run_test(test_string_to_integer, "test_string_to_integer");
    run_test(test_is_valid_integer, "test_is_valid_integer");
    run_test(test_special_characters, "test_special_characters");
    run_test(test_long_strings, "test_long_strings");
    run_test(test_edge_cases, "test_edge_cases");

    return test_summary("String Functions Test Suite");
}