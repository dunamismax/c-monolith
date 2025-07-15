#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Include the vector library
#include "../../libs/data_structures/include/vector.h"

/**
 * Simple test framework for the vector library
 */

int tests_run = 0;
int tests_passed = 0;

#define TEST(name) \
    do { \
        printf("Running test: %s ... ", name); \
        tests_run++; \
        if (test_##name()) { \
            printf("PASS\n"); \
            tests_passed++; \
        } else { \
            printf("FAIL\n"); \
        } \
    } while(0)

// Test vector creation and destruction
int test_create_destroy() {
    Vector *vec = vector_create(5);
    if (!vec) return 0;
    
    if (vector_size(vec) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    vector_destroy(vec);
    return 1;
}

// Test push and size operations
int test_push_and_size() {
    Vector *vec = vector_create(2);
    if (!vec) return 0;
    
    // Test pushing elements
    if (vector_push(vec, 10) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_size(vec) != 1) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_push(vec, 20) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_size(vec) != 2) {
        vector_destroy(vec);
        return 0;
    }
    
    // Test growth beyond initial capacity
    if (vector_push(vec, 30) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_size(vec) != 3) {
        vector_destroy(vec);
        return 0;
    }
    
    vector_destroy(vec);
    return 1;
}

// Test get and set operations
int test_get_set() {
    Vector *vec = vector_create(3);
    if (!vec) return 0;
    
    // Add some elements
    vector_push(vec, 100);
    vector_push(vec, 200);
    vector_push(vec, 300);
    
    // Test get
    int value;
    if (vector_get(vec, 0, &value) != 0 || value != 100) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_get(vec, 1, &value) != 0 || value != 200) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_get(vec, 2, &value) != 0 || value != 300) {
        vector_destroy(vec);
        return 0;
    }
    
    // Test set
    if (vector_set(vec, 1, 250) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_get(vec, 1, &value) != 0 || value != 250) {
        vector_destroy(vec);
        return 0;
    }
    
    // Test invalid indices
    if (vector_get(vec, 5, &value) == 0) {  // Should fail
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_set(vec, 5, 500) == 0) {  // Should fail
        vector_destroy(vec);
        return 0;
    }
    
    vector_destroy(vec);
    return 1;
}

// Test pop operation
int test_pop() {
    Vector *vec = vector_create(3);
    if (!vec) return 0;
    
    // Add elements
    vector_push(vec, 10);
    vector_push(vec, 20);
    vector_push(vec, 30);
    
    // Test pop
    int value;
    if (vector_pop(vec, &value) != 0 || value != 30) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_size(vec) != 2) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_pop(vec, &value) != 0 || value != 20) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_pop(vec, &value) != 0 || value != 10) {
        vector_destroy(vec);
        return 0;
    }
    
    if (vector_size(vec) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    // Test pop from empty vector
    if (vector_pop(vec, &value) == 0) {  // Should fail
        vector_destroy(vec);
        return 0;
    }
    
    vector_destroy(vec);
    return 1;
}

// Test edge cases
int test_edge_cases() {
    // Test NULL vector
    if (vector_size(NULL) != 0) return 0;
    
    // Test zero capacity creation
    Vector *vec = vector_create(0);
    if (!vec) return 0;
    
    // Should still be able to push (will use default capacity)
    if (vector_push(vec, 42) != 0) {
        vector_destroy(vec);
        return 0;
    }
    
    vector_destroy(vec);
    return 1;
}

int main() {
    printf("Vector Library Test Suite\n");
    printf("========================\n\n");
    
    TEST(create_destroy);
    TEST(push_and_size);
    TEST(get_set);
    TEST(pop);
    TEST(edge_cases);
    
    printf("\nTest Results:\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("\nAll tests passed! ✓\n");
        return 0;
    } else {
        printf("\nSome tests failed! ✗\n");
        return 1;
    }
}