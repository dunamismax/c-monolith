#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the test framework
#include "../test_framework.h"

// Include the data structures header
#include "../../apps/data/data_structures_demo/src/data_structures.h"

// Test hash table creation
int test_create_hash_table(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;
  if (table->size != 0)
    return 0;

  // Check that all buckets are initialized to NULL
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (table->buckets[i] != NULL) {
      free_hash_table(table);
      return 0;
    }
  }

  free_hash_table(table);
  return 1;
}

// Test hash function consistency
int test_hash_function(void) {
  // Same string should always produce same hash
  unsigned int hash1 = hash_function("test");
  unsigned int hash2 = hash_function("test");
  if (hash1 != hash2)
    return 0;

  // Hash should be within valid range
  if (hash1 >= HASH_TABLE_SIZE)
    return 0;

  // Different strings should (usually) produce different hashes
  unsigned int hash3 = hash_function("different");
  // This is not guaranteed, but very likely for these specific strings
  if (hash1 == hash3) {
    // Not necessarily a failure, but unlikely
  }

  return 1;
}

// Test hash table insertion
int test_hash_insert(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  hash_insert(table, "key1", 100);
  if (table->size != 1) {
    free_hash_table(table);
    return 0;
  }

  hash_insert(table, "key2", 200);
  if (table->size != 2) {
    free_hash_table(table);
    return 0;
  }

  // Test inserting with same key (should add another node, not replace)
  hash_insert(table, "key1", 150);
  if (table->size != 3) {
    free_hash_table(table);
    return 0;
  }

  free_hash_table(table);
  return 1;
}

// Test hash table search
int test_hash_search(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  hash_insert(table, "apple", 5);
  hash_insert(table, "banana", 3);
  hash_insert(table, "cherry", 7);

  bool found;
  int value;

  // Test successful searches
  value = hash_search(table, "apple", &found);
  if (!found || value != 5) {
    free_hash_table(table);
    return 0;
  }

  value = hash_search(table, "banana", &found);
  if (!found || value != 3) {
    free_hash_table(table);
    return 0;
  }

  // Test unsuccessful search
  value = hash_search(table, "orange", &found);
  if (found) {
    free_hash_table(table);
    return 0;
  }

  free_hash_table(table);
  return 1;
}

// Test hash table deletion
int test_hash_delete(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  hash_insert(table, "key1", 100);
  hash_insert(table, "key2", 200);
  hash_insert(table, "key3", 300);

  // Delete existing key
  if (!hash_delete(table, "key2")) {
    free_hash_table(table);
    return 0;
  }
  if (table->size != 2) {
    free_hash_table(table);
    return 0;
  }

  // Verify key is actually deleted
  bool found;
  hash_search(table, "key2", &found);
  if (found) {
    free_hash_table(table);
    return 0;
  }

  // Other keys should still exist
  hash_search(table, "key1", &found);
  if (!found) {
    free_hash_table(table);
    return 0;
  }

  // Delete non-existing key
  if (hash_delete(table, "nonexistent")) {
    free_hash_table(table);
    return 0;
  }

  free_hash_table(table);
  return 1;
}

// Test collision handling (multiple keys mapping to same bucket)
int test_collision_handling(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  // Insert multiple items that might collide
  // We can't guarantee collisions, but we can test the functionality
  hash_insert(table, "collision1", 1);
  hash_insert(table, "collision2", 2);
  hash_insert(table, "collision3", 3);

  if (table->size != 3) {
    free_hash_table(table);
    return 0;
  }

  // All should be searchable
  bool found;
  int value;

  value = hash_search(table, "collision1", &found);
  if (!found || value != 1) {
    free_hash_table(table);
    return 0;
  }

  value = hash_search(table, "collision2", &found);
  if (!found || value != 2) {
    free_hash_table(table);
    return 0;
  }

  value = hash_search(table, "collision3", &found);
  if (!found || value != 3) {
    free_hash_table(table);
    return 0;
  }

  // Delete one and others should still work
  hash_delete(table, "collision2");

  value = hash_search(table, "collision1", &found);
  if (!found || value != 1) {
    free_hash_table(table);
    return 0;
  }

  value = hash_search(table, "collision3", &found);
  if (!found || value != 3) {
    free_hash_table(table);
    return 0;
  }

  free_hash_table(table);
  return 1;
}

// Test with empty strings and special characters
int test_special_keys(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  // Test with empty string (if supported)
  hash_insert(table, "", 0);
  bool found;
  int value = hash_search(table, "", &found);
  if (!found || value != 0) {
    free_hash_table(table);
    return 0;
  }

  // Test with special characters
  hash_insert(table, "key with spaces", 1);
  hash_insert(table, "key-with-dashes", 2);
  hash_insert(table, "key_with_underscores", 3);
  hash_insert(table, "KEY_IN_CAPS", 4);

  value = hash_search(table, "key with spaces", &found);
  if (!found || value != 1) {
    free_hash_table(table);
    return 0;
  }

  value = hash_search(table, "KEY_IN_CAPS", &found);
  if (!found || value != 4) {
    free_hash_table(table);
    return 0;
  }

  free_hash_table(table);
  return 1;
}

// Test edge cases and error handling
int test_edge_cases(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  // Test with NULL key (should handle gracefully)
  hash_insert(table, NULL, 100);
  if (table->size != 0) {
    free_hash_table(table);
    return 0;
  }

  // Test search with NULL key
  bool found;
  hash_search(table, NULL, &found);

  // Test delete with NULL key
  hash_delete(table, NULL);

  // Test with NULL table
  hash_insert(NULL, "key", 100);
  hash_search(NULL, "key", &found);
  hash_delete(NULL, "key");

  free_hash_table(table);
  return 1;
}

// Test hash table capacity and performance
int test_capacity_stress(void) {
  HashTable *table = create_hash_table();
  if (!table)
    return 0;

  // Insert many items to test capacity
  char key[20];
  for (int i = 0; i < 50; i++) {
    snprintf(key, sizeof(key), "key_%d", i);
    hash_insert(table, key, i);
  }

  if (table->size != 50) {
    free_hash_table(table);
    return 0;
  }

  // Test that all can be found
  bool found;
  for (int i = 0; i < 50; i++) {
    snprintf(key, sizeof(key), "key_%d", i);
    int value = hash_search(table, key, &found);
    if (!found || value != i) {
      free_hash_table(table);
      return 0;
    }
  }

  free_hash_table(table);
  return 1;
}

int main(void) {
  printf("Data Structures Demo - Hash Table Test Suite\n");
  printf("=============================================\n\n");

  run_test(test_create_hash_table, "test_create_hash_table");
  run_test(test_hash_function, "test_hash_function");
  run_test(test_hash_insert, "test_hash_insert");
  run_test(test_hash_search, "test_hash_search");
  run_test(test_hash_delete, "test_hash_delete");
  run_test(test_collision_handling, "test_collision_handling");
  run_test(test_special_keys, "test_special_keys");
  run_test(test_edge_cases, "test_edge_cases");
  run_test(test_capacity_stress, "test_capacity_stress");

  return test_summary("Hash Table Test Suite");
}