#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the test framework
#include "../test_framework.h"

// Include the data structures header
#include "../../apps/data/data_structures_demo/src/data_structures.h"

// Test integration between different data structures
int test_multi_structure_workflow(void) {
    // Create all data structures
    LinkedList* list = create_list();
    DynamicArray* array = create_array(5);
    HashTable* table = create_hash_table();
    BinaryTree* tree = create_tree();
    
    if (!list || !array || !table || !tree) {
        free_list(list);
        free_array(array);
        free_hash_table(table);
        free_tree(tree);
        return 0;
    }
    
    // Add same data to all structures
    int values[] = {50, 30, 70, 20, 40};
    int num_values = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < num_values; i++) {
        insert_at_end(list, values[i]);
        array_insert(array, values[i]);
        tree_insert(tree, values[i]);
        
        // Convert int to string for hash table
        char* key = integer_to_string(values[i]);
        if (key) {
            hash_insert(table, key, values[i]);
            free(key);
        }
    }
    
    // Verify all structures contain the data
    if (list->size != num_values || array->size != num_values || 
        table->size != num_values || tree->size != num_values) {
        free_list(list);
        free_array(array);
        free_hash_table(table);
        free_tree(tree);
        return 0;
    }
    
    // Test cross-structure operations
    for (int i = 0; i < num_values; i++) {
        // Check array
        if (array_get(array, i) != values[i]) {
            free_list(list);
            free_array(array);
            free_hash_table(table);
            free_tree(tree);
            return 0;
        }
        
        // Check tree
        if (!tree_search(tree, values[i])) {
            free_list(list);
            free_array(array);
            free_hash_table(table);
            free_tree(tree);
            return 0;
        }
        
        // Check hash table
        char* key = integer_to_string(values[i]);
        if (key) {
            bool found;
            int value = hash_search(table, key, &found);
            if (!found || value != values[i]) {
                free(key);
                free_list(list);
                free_array(array);
                free_hash_table(table);
                free_tree(tree);
                return 0;
            }
            free(key);
        }
    }
    
    free_list(list);
    free_array(array);
    free_hash_table(table);
    free_tree(tree);
    return 1;
}

// Test string functions integration with data structures
int test_string_integration(void) {
    HashTable* table = create_hash_table();
    if (!table) return 0;
    
    // Test string functions with hash table
    char* keys[] = {"apple", "banana", "cherry", "date"};
    int values[] = {1, 2, 3, 4};
    int num_items = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < num_items; i++) {
        hash_insert(table, keys[i], values[i]);
    }
    
    // Test string operations
    for (int i = 0; i < num_items; i++) {
        // Test string length
        int len = string_length(keys[i]);
        if (len <= 0) {
            free_hash_table(table);
            return 0;
        }
        
        // Test string copy and comparison
        char copied[50];
        string_copy(keys[i], copied);
        if (string_compare(keys[i], copied) != 0) {
            free_hash_table(table);
            return 0;
        }
        
        // Verify hash table still works
        bool found;
        int value = hash_search(table, copied, &found);
        if (!found || value != values[i]) {
            free_hash_table(table);
            return 0;
        }
    }
    
    // Test string concatenation with hash table operations
    char* concat_key = string_concatenate("new", "_key");
    if (!concat_key) {
        free_hash_table(table);
        return 0;
    }
    
    hash_insert(table, concat_key, 99);
    bool found;
    int value = hash_search(table, concat_key, &found);
    if (!found || value != 99) {
        free(concat_key);
        free_hash_table(table);
        return 0;
    }
    
    free(concat_key);
    free_hash_table(table);
    return 1;
}

// Test memory management across structures
int test_memory_management(void) {
    // Create and destroy structures multiple times
    for (int iteration = 0; iteration < 5; iteration++) {
        LinkedList* list = create_list();
        DynamicArray* array = create_array(10);
        HashTable* table = create_hash_table();
        BinaryTree* tree = create_tree();
        
        if (!list || !array || !table || !tree) {
            free_list(list);
            free_array(array);
            free_hash_table(table);
            free_tree(tree);
            return 0;
        }
        
        // Add data
        for (int i = 0; i < 10; i++) {
            insert_at_end(list, i);
            array_insert(array, i);
            tree_insert(tree, i);
            
            char* key = integer_to_string(i);
            if (key) {
                hash_insert(table, key, i);
                free(key);
            }
        }
        
        // Remove some data
        for (int i = 0; i < 5; i++) {
            delete_from_beginning(list);
            array_delete_at(array, 0);
            tree_delete(tree, i);
            
            char* key = integer_to_string(i);
            if (key) {
                hash_delete(table, key);
                free(key);
            }
        }
        
        // Clean up
        free_list(list);
        free_array(array);
        free_hash_table(table);
        free_tree(tree);
    }
    
    return 1;
}

// Test performance with large datasets
int test_performance_integration(void) {
    const int LARGE_SIZE = 1000;
    
    LinkedList* list = create_list();
    DynamicArray* array = create_array(LARGE_SIZE);
    BinaryTree* tree = create_tree();
    
    if (!list || !array || !tree) {
        free_list(list);
        free_array(array);
        free_tree(tree);
        return 0;
    }
    
    // Insert large amount of data
    for (int i = 0; i < LARGE_SIZE; i++) {
        insert_at_end(list, i);
        array_insert(array, i);
        tree_insert(tree, i);
    }
    
    // Verify sizes
    if (list->size != LARGE_SIZE || array->size != LARGE_SIZE || tree->size != LARGE_SIZE) {
        free_list(list);
        free_array(array);
        free_tree(tree);
        return 0;
    }
    
    // Test random access patterns
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        if (value < LARGE_SIZE) {
            // Array access should be fast
            if (array_get(array, value) != value) {
                free_list(list);
                free_array(array);
                free_tree(tree);
                return 0;
            }
            
            // Tree search should work
            if (!tree_search(tree, value)) {
                free_list(list);
                free_array(array);
                free_tree(tree);
                return 0;
            }
        }
    }
    
    free_list(list);
    free_array(array);
    free_tree(tree);
    return 1;
}

// Test error handling integration
int test_error_handling_integration(void) {
    // Test operations on NULL structures
    insert_at_end(NULL, 10);
    array_insert(NULL, 10);
    tree_insert(NULL, 10);
    hash_insert(NULL, "key", 10);
    
    // Test with invalid parameters
    LinkedList* list = create_list();
    DynamicArray* array = create_array(5);
    HashTable* table = create_hash_table();
    BinaryTree* tree = create_tree();
    
    if (!list || !array || !table || !tree) {
        free_list(list);
        free_array(array);
        free_hash_table(table);
        free_tree(tree);
        return 0;
    }
    
    // Test invalid operations
    delete_from_beginning(list);  // Delete from empty
    array_delete_at(array, -1);   // Invalid index
    tree_delete(tree, 999);       // Delete non-existent
    hash_delete(table, "nonexistent");  // Delete non-existent
    
    // Structures should still be valid
    insert_at_end(list, 10);
    array_insert(array, 10);
    tree_insert(tree, 10);
    hash_insert(table, "key", 10);
    
    if (list->size != 1 || array->size != 1 || tree->size != 1 || table->size != 1) {
        free_list(list);
        free_array(array);
        free_hash_table(table);
        free_tree(tree);
        return 0;
    }
    
    free_list(list);
    free_array(array);
    free_hash_table(table);
    free_tree(tree);
    return 1;
}

// Test real-world scenario: building a word frequency counter
int test_word_frequency_scenario(void) {
    HashTable* word_freq = create_hash_table();
    DynamicArray* word_list = create_array(10);
    
    if (!word_freq || !word_list) {
        free_hash_table(word_freq);
        free_array(word_list);
        return 0;
    }
    
    // Simulate word frequency counting
    char* words[] = {"hello", "world", "hello", "test", "world", "hello"};
    // int expected_freq[] = {3, 2, 3, 1, 2, 3};  // Frequency of each word
    int num_words = sizeof(words) / sizeof(words[0]);
    
    for (int i = 0; i < num_words; i++) {
        // Check if word exists
        bool found;
        int current_freq = hash_search(word_freq, words[i], &found);
        
        if (found) {
            // Update frequency
            hash_delete(word_freq, words[i]);  // Remove old entry
            hash_insert(word_freq, words[i], current_freq + 1);
        } else {
            // First occurrence
            hash_insert(word_freq, words[i], 1);
            array_insert(word_list, i);  // Store word index
        }
    }
    
    // Verify results
    bool found;
    if (hash_search(word_freq, "hello", &found) != 3 || !found) {
        free_hash_table(word_freq);
        free_array(word_list);
        return 0;
    }
    
    if (hash_search(word_freq, "world", &found) != 2 || !found) {
        free_hash_table(word_freq);
        free_array(word_list);
        return 0;
    }
    
    if (hash_search(word_freq, "test", &found) != 1 || !found) {
        free_hash_table(word_freq);
        free_array(word_list);
        return 0;
    }
    
    free_hash_table(word_freq);
    free_array(word_list);
    return 1;
}

int main(void) {
    printf("Data Structures Demo - Integration Test Suite\n");
    printf("==============================================\n\n");

    run_test(test_multi_structure_workflow, "test_multi_structure_workflow");
    run_test(test_string_integration, "test_string_integration");
    run_test(test_memory_management, "test_memory_management");
    run_test(test_performance_integration, "test_performance_integration");
    run_test(test_error_handling_integration, "test_error_handling_integration");
    run_test(test_word_frequency_scenario, "test_word_frequency_scenario");

    return test_summary("Data Structures Integration Test Suite");
}