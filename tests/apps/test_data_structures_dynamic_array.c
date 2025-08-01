#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Include the test framework
#include "../test_framework.h"

// Include the data structures header
#include "../../apps/data/data_structures_demo/src/data_structures.h"

// Test dynamic array creation
int test_create_array(void) {
  DynamicArray *arr = create_array(5);
  if (!arr)
    return 0;
  if (arr->size != 0)
    return 0;
  if (arr->capacity != 5)
    return 0;
  if (!arr->data)
    return 0;
  free_array(arr);

  // Test with default capacity
  arr = create_array(0);
  if (!arr)
    return 0;
  if (arr->capacity != 10)
    return 0; // Should default to 10
  free_array(arr);

  return 1;
}

// Test array insertion
int test_array_insert(void) {
  DynamicArray *arr = create_array(2);
  if (!arr)
    return 0;

  array_insert(arr, 10);
  if (arr->size != 1 || arr->data[0] != 10) {
    free_array(arr);
    return 0;
  }

  array_insert(arr, 20);
  if (arr->size != 2 || arr->data[1] != 20) {
    free_array(arr);
    return 0;
  }

  // Test resize when capacity is exceeded
  array_insert(arr, 30);
  if (arr->size != 3 || arr->capacity < 3 || arr->data[2] != 30) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

// Test array insertion at specific index
int test_array_insert_at(void) {
  DynamicArray *arr = create_array(5);
  if (!arr)
    return 0;

  // Insert at beginning of empty array
  array_insert_at(arr, 10, 0);
  if (arr->size != 1 || arr->data[0] != 10) {
    free_array(arr);
    return 0;
  }

  // Insert at end
  array_insert_at(arr, 30, 1);
  if (arr->size != 2 || arr->data[1] != 30) {
    free_array(arr);
    return 0;
  }

  // Insert in middle
  array_insert_at(arr, 20, 1);
  if (arr->size != 3) {
    free_array(arr);
    return 0;
  }
  if (arr->data[0] != 10 || arr->data[1] != 20 || arr->data[2] != 30) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

// Test array deletion
int test_array_delete_at(void) {
  DynamicArray *arr = create_array(5);
  if (!arr)
    return 0;

  // Add some elements
  array_insert(arr, 10);
  array_insert(arr, 20);
  array_insert(arr, 30);
  array_insert(arr, 40);

  // Delete middle element
  if (!array_delete_at(arr, 1)) {
    free_array(arr);
    return 0;
  }
  if (arr->size != 3) {
    free_array(arr);
    return 0;
  }
  if (arr->data[0] != 10 || arr->data[1] != 30 || arr->data[2] != 40) {
    free_array(arr);
    return 0;
  }

  // Delete first element
  if (!array_delete_at(arr, 0)) {
    free_array(arr);
    return 0;
  }
  if (arr->size != 2 || arr->data[0] != 30) {
    free_array(arr);
    return 0;
  }

  // Delete last element
  if (!array_delete_at(arr, 1)) {
    free_array(arr);
    return 0;
  }
  if (arr->size != 1 || arr->data[0] != 30) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

// Test array get function
int test_array_get(void) {
  DynamicArray *arr = create_array(5);
  if (!arr)
    return 0;

  array_insert(arr, 10);
  array_insert(arr, 20);
  array_insert(arr, 30);

  if (array_get(arr, 0) != 10) {
    free_array(arr);
    return 0;
  }
  if (array_get(arr, 1) != 20) {
    free_array(arr);
    return 0;
  }
  if (array_get(arr, 2) != 30) {
    free_array(arr);
    return 0;
  }

  // Test invalid index
  if (array_get(arr, -1) != -1) {
    free_array(arr);
    return 0;
  }
  if (array_get(arr, 5) != -1) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

// Test array set function
int test_array_set(void) {
  DynamicArray *arr = create_array(5);
  if (!arr)
    return 0;

  array_insert(arr, 10);
  array_insert(arr, 20);
  array_insert(arr, 30);

  array_set(arr, 1, 99);
  if (array_get(arr, 1) != 99) {
    free_array(arr);
    return 0;
  }

  // Other elements should remain unchanged
  if (array_get(arr, 0) != 10 || array_get(arr, 2) != 30) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

// Test array resize functionality
int test_array_resize(void) {
  DynamicArray *arr = create_array(2);
  if (!arr)
    return 0;

  int initial_capacity = arr->capacity;

  // Fill array to capacity
  array_insert(arr, 10);
  array_insert(arr, 20);

  // Add one more to trigger resize
  array_insert(arr, 30);

  if (arr->capacity <= initial_capacity) {
    free_array(arr);
    return 0;
  }

  // Check all elements are still there
  if (arr->data[0] != 10 || arr->data[1] != 20 || arr->data[2] != 30) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

// Test edge cases and error handling
int test_edge_cases(void) {
  // Test with NULL pointer
  array_insert(NULL, 10);
  array_insert_at(NULL, 10, 0);

  DynamicArray *arr = create_array(3);
  if (!arr)
    return 0;

  // Test invalid indices
  array_insert_at(arr, 10, -1); // Should handle gracefully
  if (arr->size != 0) {
    free_array(arr);
    return 0;
  }

  array_insert_at(arr, 10, 5); // Beyond size
  if (arr->size != 0) {
    free_array(arr);
    return 0;
  }

  // Test delete from empty array
  if (array_delete_at(arr, 0)) {
    free_array(arr);
    return 0;
  }

  // Test invalid delete indices
  array_insert(arr, 10);
  if (array_delete_at(arr, -1)) {
    free_array(arr);
    return 0;
  }
  if (array_delete_at(arr, 5)) {
    free_array(arr);
    return 0;
  }

  free_array(arr);
  return 1;
}

int main(void) {
  printf("Data Structures Demo - Dynamic Array Test Suite\n");
  printf("================================================\n\n");

  run_test(test_create_array, "test_create_array");
  run_test(test_array_insert, "test_array_insert");
  run_test(test_array_insert_at, "test_array_insert_at");
  run_test(test_array_delete_at, "test_array_delete_at");
  run_test(test_array_get, "test_array_get");
  run_test(test_array_set, "test_array_set");
  run_test(test_array_resize, "test_array_resize");
  run_test(test_edge_cases, "test_edge_cases");

  return test_summary("Dynamic Array Test Suite");
}