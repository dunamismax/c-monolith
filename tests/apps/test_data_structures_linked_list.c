#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Include the test framework
#include "../test_framework.h"

// Include the data structures header
#include "../../apps/data/data_structures_demo/src/data_structures.h"

// Test linked list creation
int test_create_list(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;
  if (list->head != NULL)
    return 0;
  if (list->size != 0)
    return 0;
  free_list(list);
  return 1;
}

// Test insert at beginning
int test_insert_at_beginning(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;

  insert_at_beginning(list, 10);
  if (list->size != 1) {
    free_list(list);
    return 0;
  }
  if (list->head->data != 10) {
    free_list(list);
    return 0;
  }

  insert_at_beginning(list, 20);
  if (list->size != 2) {
    free_list(list);
    return 0;
  }
  if (list->head->data != 20) {
    free_list(list);
    return 0;
  }
  if (list->head->next->data != 10) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

// Test insert at end
int test_insert_at_end(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;

  insert_at_end(list, 10);
  if (list->size != 1 || list->head->data != 10) {
    free_list(list);
    return 0;
  }

  insert_at_end(list, 20);
  if (list->size != 2) {
    free_list(list);
    return 0;
  }
  if (list->head->data != 10 || list->head->next->data != 20) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

// Test insert at position
int test_insert_at_position(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;

  // Insert at position 0 (beginning)
  insert_at_position(list, 10, 0);
  if (list->size != 1 || list->head->data != 10) {
    free_list(list);
    return 0;
  }

  // Insert at position 1 (end)
  insert_at_position(list, 30, 1);
  if (list->size != 2 || list->head->next->data != 30) {
    free_list(list);
    return 0;
  }

  // Insert at position 1 (middle)
  insert_at_position(list, 20, 1);
  if (list->size != 3) {
    free_list(list);
    return 0;
  }
  if (list->head->data != 10 || list->head->next->data != 20 ||
      list->head->next->next->data != 30) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

// Test delete from beginning
int test_delete_from_beginning(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;

  // Test delete from empty list
  if (delete_from_beginning(list)) {
    free_list(list);
    return 0;
  }

  // Add elements and test deletion
  insert_at_beginning(list, 10);
  insert_at_beginning(list, 20);

  if (!delete_from_beginning(list)) {
    free_list(list);
    return 0;
  }
  if (list->size != 1 || list->head->data != 10) {
    free_list(list);
    return 0;
  }

  if (!delete_from_beginning(list)) {
    free_list(list);
    return 0;
  }
  if (list->size != 0 || list->head != NULL) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

// Test delete from end
int test_delete_from_end(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;

  // Test delete from empty list
  if (delete_from_end(list)) {
    free_list(list);
    return 0;
  }

  // Test with single element
  insert_at_end(list, 10);
  if (!delete_from_end(list)) {
    free_list(list);
    return 0;
  }
  if (list->size != 0 || list->head != NULL) {
    free_list(list);
    return 0;
  }

  // Test with multiple elements
  insert_at_end(list, 10);
  insert_at_end(list, 20);
  insert_at_end(list, 30);

  if (!delete_from_end(list)) {
    free_list(list);
    return 0;
  }
  if (list->size != 2 || list->head->next->data != 20) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

// Test delete at position
int test_delete_at_position(void) {
  LinkedList *list = create_list();
  if (!list)
    return 0;

  // Test delete from empty list
  if (delete_at_position(list, 0)) {
    free_list(list);
    return 0;
  }

  // Add elements
  insert_at_end(list, 10);
  insert_at_end(list, 20);
  insert_at_end(list, 30);

  // Delete middle element
  if (!delete_at_position(list, 1)) {
    free_list(list);
    return 0;
  }
  if (list->size != 2 || list->head->next->data != 30) {
    free_list(list);
    return 0;
  }

  // Delete first element
  if (!delete_at_position(list, 0)) {
    free_list(list);
    return 0;
  }
  if (list->size != 1 || list->head->data != 30) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

// Test edge cases and error handling
int test_edge_cases(void) {
  // Test with NULL pointer
  insert_at_beginning(NULL, 10);
  insert_at_end(NULL, 10);
  insert_at_position(NULL, 10, 0);

  LinkedList *list = create_list();
  if (!list)
    return 0;

  // Test invalid positions
  insert_at_position(list, 10, -1); // Should handle gracefully
  if (list->size != 0) {
    free_list(list);
    return 0;
  }

  // Test delete invalid positions
  if (delete_at_position(list, -1)) {
    free_list(list);
    return 0;
  }
  if (delete_at_position(list, 5)) {
    free_list(list);
    return 0;
  }

  free_list(list);
  return 1;
}

int main(void) {
  printf("Data Structures Demo - Linked List Test Suite\n");
  printf("==============================================\n\n");

  run_test(test_create_list, "test_create_list");
  run_test(test_insert_at_beginning, "test_insert_at_beginning");
  run_test(test_insert_at_end, "test_insert_at_end");
  run_test(test_insert_at_position, "test_insert_at_position");
  run_test(test_delete_from_beginning, "test_delete_from_beginning");
  run_test(test_delete_from_end, "test_delete_from_end");
  run_test(test_delete_at_position, "test_delete_at_position");
  run_test(test_edge_cases, "test_edge_cases");

  return test_summary("Linked List Test Suite");
}