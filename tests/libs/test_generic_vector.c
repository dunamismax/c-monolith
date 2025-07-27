#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the test framework
#include "../test_framework.h"

// Include the generic vector library
#include "../../libs/data_structures/include/generic_vector.h"

// Test structure for complex data types
typedef struct {
    int id;
    char name[32];
    double value;
} TestStruct;

// Comparison function for TestStruct
int compare_test_struct(const void *a, const void *b) {
    const TestStruct *ta = (const TestStruct *)a;
    const TestStruct *tb = (const TestStruct *)b;
    return ta->id - tb->id;
}

// Comparison function for integers
int compare_int(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return (*ia > *ib) - (*ia < *ib);
}

// Test creation and destruction
int test_create_destroy(void) {
    GenericVector *vec = gvector_create(5, sizeof(int));
    if (!vec) return 0;
    
    if (gvector_size(vec) != 0) {
        gvector_destroy(vec);
        return 0;
    }
    
    if (gvector_capacity(vec) != 5) {
        gvector_destroy(vec);
        return 0;
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test push and size operations with integers
int test_push_and_size_int(void) {
    GenericVector *vec = gvector_create(2, sizeof(int));
    if (!vec) return 0;
    
    int values[] = {10, 20, 30, 40};
    
    // Test pushing elements
    for (int i = 0; i < 4; i++) {
        if (gvector_push(vec, &values[i]) != 0) {
            gvector_destroy(vec);
            return 0;
        }
        
        if (gvector_size(vec) != (size_t)(i + 1)) {
            gvector_destroy(vec);
            return 0;
        }
    }
    
    // Test automatic growth beyond initial capacity
    if (gvector_capacity(vec) < 4) {
        gvector_destroy(vec);
        return 0;
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test get and set operations
int test_get_set(void) {
    GenericVector *vec = gvector_create(3, sizeof(int));
    if (!vec) return 0;
    
    int values[] = {100, 200, 300};
    
    // Add elements
    for (int i = 0; i < 3; i++) {
        gvector_push(vec, &values[i]);
    }
    
    // Test get
    for (int i = 0; i < 3; i++) {
        int value;
        if (gvector_get(vec, i, &value) != 0 || value != values[i]) {
            gvector_destroy(vec);
            return 0;
        }
    }
    
    // Test set
    int new_value = 250;
    if (gvector_set(vec, 1, &new_value) != 0) {
        gvector_destroy(vec);
        return 0;
    }
    
    int retrieved_value;
    if (gvector_get(vec, 1, &retrieved_value) != 0 || retrieved_value != 250) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test invalid indices
    if (gvector_get(vec, 5, &retrieved_value) == 0) {
        gvector_destroy(vec);
        return 0; // Should fail
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test insert and remove operations
int test_insert_remove(void) {
    GenericVector *vec = gvector_create(3, sizeof(int));
    if (!vec) return 0;
    
    int values[] = {10, 30};
    
    // Add initial elements
    for (int i = 0; i < 2; i++) {
        gvector_push(vec, &values[i]);
    }
    
    // Insert element at index 1
    int insert_value = 20;
    if (gvector_insert(vec, 1, &insert_value) != 0) {
        gvector_destroy(vec);
        return 0;
    }
    
    if (gvector_size(vec) != 3) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Verify order: 10, 20, 30
    int expected[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        int value;
        if (gvector_get(vec, i, &value) != 0 || value != expected[i]) {
            gvector_destroy(vec);
            return 0;
        }
    }
    
    // Remove element at index 1
    int removed_value;
    if (gvector_remove(vec, 1, &removed_value) != 0 || removed_value != 20) {
        gvector_destroy(vec);
        return 0;
    }
    
    if (gvector_size(vec) != 2) {
        gvector_destroy(vec);
        return 0;
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test with complex data structures
int test_complex_structs(void) {
    GenericVector *vec = gvector_create(2, sizeof(TestStruct));
    if (!vec) return 0;
    
    TestStruct structs[] = {
        {1, "First", 1.5},
        {3, "Third", 3.5},
        {2, "Second", 2.5}
    };
    
    // Add structs
    for (int i = 0; i < 3; i++) {
        if (gvector_push(vec, &structs[i]) != 0) {
            gvector_destroy(vec);
            return 0;
        }
    }
    
    // Test sorting
    if (gvector_sort(vec, compare_test_struct) != 0) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Verify sorted order by ID
    for (size_t i = 0; i < gvector_size(vec); i++) {
        TestStruct retrieved;
        if (gvector_get(vec, i, &retrieved) != 0 || retrieved.id != (int)(i + 1)) {
            gvector_destroy(vec);
            return 0;
        }
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test find operation
int test_find(void) {
    GenericVector *vec = gvector_create(3, sizeof(int));
    if (!vec) return 0;
    
    int values[] = {100, 200, 300};
    
    for (int i = 0; i < 3; i++) {
        gvector_push(vec, &values[i]);
    }
    
    // Test finding existing element
    int search_value = 200;
    size_t index = gvector_find(vec, &search_value, compare_int);
    if (index != 1) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test finding non-existing element
    search_value = 400;
    index = gvector_find(vec, &search_value, compare_int);
    if (index != SIZE_MAX) {
        gvector_destroy(vec);
        return 0;
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test copy operation
int test_copy(void) {
    GenericVector *vec = gvector_create(3, sizeof(int));
    if (!vec) return 0;
    
    int values[] = {10, 20, 30};
    
    for (int i = 0; i < 3; i++) {
        gvector_push(vec, &values[i]);
    }
    
    GenericVector *copy = gvector_copy(vec);
    if (!copy) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Verify copy has same elements
    if (gvector_size(copy) != gvector_size(vec)) {
        gvector_destroy(vec);
        gvector_destroy(copy);
        return 0;
    }
    
    for (size_t i = 0; i < gvector_size(vec); i++) {
        int orig_val, copy_val;
        gvector_get(vec, i, &orig_val);
        gvector_get(copy, i, &copy_val);
        
        if (orig_val != copy_val) {
            gvector_destroy(vec);
            gvector_destroy(copy);
            return 0;
        }
    }
    
    gvector_destroy(vec);
    gvector_destroy(copy);
    return 1;
}

// Test edge cases and error conditions
int test_edge_cases(void) {
    // Test NULL vector operations
    if (gvector_size(NULL) != 0) return 0;
    if (gvector_empty(NULL) != -1) return 0;
    if (gvector_at(NULL, 0) != NULL) return 0;
    
    // Test zero element size creation
    GenericVector *vec = gvector_create(5, 0);
    if (vec != NULL) return 0;
    
    // Test zero capacity creation
    vec = gvector_create(0, sizeof(int));
    if (!vec) return 0;
    
    // Should still be able to push (will use default capacity)
    int value = 42;
    if (gvector_push(vec, &value) != 0) {
        gvector_destroy(vec);
        return 0;
    }
    
    if (gvector_size(vec) != 1) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test pop from single element
    int popped_value;
    if (gvector_pop(vec, &popped_value) != 0 || popped_value != 42) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test pop from empty vector
    if (gvector_pop(vec, &popped_value) == 0) {
        gvector_destroy(vec);
        return 0; // Should fail
    }
    
    gvector_destroy(vec);
    return 1;
}

// Test direct access functions
int test_direct_access(void) {
    GenericVector *vec = gvector_create(3, sizeof(int));
    if (!vec) return 0;
    
    int values[] = {10, 20, 30};
    
    for (int i = 0; i < 3; i++) {
        gvector_push(vec, &values[i]);
    }
    
    // Test gvector_at
    int *ptr = (int *)gvector_at(vec, 1);
    if (!ptr || *ptr != 20) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test gvector_front
    ptr = (int *)gvector_front(vec);
    if (!ptr || *ptr != 10) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test gvector_back
    ptr = (int *)gvector_back(vec);
    if (!ptr || *ptr != 30) {
        gvector_destroy(vec);
        return 0;
    }
    
    // Test gvector_data
    ptr = (int *)gvector_data(vec);
    if (!ptr || ptr[0] != 10 || ptr[1] != 20 || ptr[2] != 30) {
        gvector_destroy(vec);
        return 0;
    }
    
    gvector_destroy(vec);
    return 1;
}

int main(void) {
    printf("Generic Vector Library Test Suite\n");
    printf("=================================\n\n");
    
    run_test(test_create_destroy, "test_create_destroy");
    run_test(test_push_and_size_int, "test_push_and_size_int");
    run_test(test_get_set, "test_get_set");
    run_test(test_insert_remove, "test_insert_remove");
    run_test(test_complex_structs, "test_complex_structs");
    run_test(test_find, "test_find");
    run_test(test_copy, "test_copy");
    run_test(test_edge_cases, "test_edge_cases");
    run_test(test_direct_access, "test_direct_access");
    
    return test_summary("Generic Vector Library Test Suite");
}