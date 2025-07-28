#include "data_structures.h"

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrate_linked_list(void) {
    printf("\n=== LINKED LIST DEMONSTRATION ===\n");
    
    LinkedList* list = create_list();
    if (!list) {
        printf("Failed to create linked list\n");
        return;
    }
    
    printf("1. Inserting elements at the beginning: 10, 20, 30\n");
    insert_at_beginning(list, 10);
    insert_at_beginning(list, 20);
    insert_at_beginning(list, 30);
    display_list(list);
    
    printf("\n2. Inserting elements at the end: 40, 50\n");
    insert_at_end(list, 40);
    insert_at_end(list, 50);
    display_list(list);
    
    printf("\n3. Inserting 25 at position 2\n");
    insert_at_position(list, 25, 2);
    display_list(list);
    
    printf("\n4. Deleting from beginning\n");
    delete_from_beginning(list);
    display_list(list);
    
    printf("\n5. Deleting from end\n");
    delete_from_end(list);
    display_list(list);
    
    printf("\n6. Deleting at position 1\n");
    delete_at_position(list, 1);
    display_list(list);
    
    free_list(list);
    printf("\nLinked list demonstration completed!\n");
}

void demonstrate_dynamic_array(void) {
    printf("\n=== DYNAMIC ARRAY DEMONSTRATION ===\n");
    
    DynamicArray* arr = create_array(3);
    if (!arr) {
        printf("Failed to create dynamic array\n");
        return;
    }
    
    printf("1. Initial array (capacity 3):\n");
    display_array(arr);
    
    printf("\n2. Inserting elements: 10, 20, 30, 40, 50\n");
    array_insert(arr, 10);
    array_insert(arr, 20);
    array_insert(arr, 30);
    display_array(arr);
    
    printf("Adding more elements to trigger resize:\n");
    array_insert(arr, 40);
    array_insert(arr, 50);
    display_array(arr);
    
    printf("\n3. Inserting 25 at index 2\n");
    array_insert_at(arr, 25, 2);
    display_array(arr);
    
    printf("\n4. Getting element at index 3: %d\n", array_get(arr, 3));
    
    printf("\n5. Setting element at index 1 to 99\n");
    array_set(arr, 1, 99);
    display_array(arr);
    
    printf("\n6. Deleting element at index 2\n");
    array_delete_at(arr, 2);
    display_array(arr);
    
    free_array(arr);
    printf("\nDynamic array demonstration completed!\n");
}

void demonstrate_hash_table(void) {
    printf("\n=== HASH TABLE DEMONSTRATION ===\n");
    
    HashTable* table = create_hash_table();
    if (!table) {
        printf("Failed to create hash table\n");
        return;
    }
    
    printf("1. Inserting key-value pairs:\n");
    hash_insert(table, "apple", 5);
    hash_insert(table, "banana", 3);
    hash_insert(table, "orange", 8);
    hash_insert(table, "grape", 12);
    hash_insert(table, "cherry", 7);
    display_hash_table(table);
    
    printf("\n2. Searching for keys:\n");
    bool found;
    int value;
    
    value = hash_search(table, "apple", &found);
    printf("apple: %s (value: %d)\n", found ? "found" : "not found", value);
    
    value = hash_search(table, "banana", &found);
    printf("banana: %s (value: %d)\n", found ? "found" : "not found", value);
    
    value = hash_search(table, "mango", &found);
    printf("mango: %s (value: %d)\n", found ? "found" : "not found", value);
    
    printf("\n3. Deleting 'banana'\n");
    if (hash_delete(table, "banana")) {
        printf("Successfully deleted 'banana'\n");
    } else {
        printf("Failed to delete 'banana'\n");
    }
    display_hash_table(table);
    
    free_hash_table(table);
    printf("\nHash table demonstration completed!\n");
}

void demonstrate_binary_tree(void) {
    printf("\n=== BINARY TREE DEMONSTRATION ===\n");
    
    BinaryTree* tree = create_tree();
    if (!tree) {
        printf("Failed to create binary tree\n");
        return;
    }
    
    printf("1. Inserting elements: 50, 30, 70, 20, 40, 60, 80\n");
    tree_insert(tree, 50);
    tree_insert(tree, 30);
    tree_insert(tree, 70);
    tree_insert(tree, 20);
    tree_insert(tree, 40);
    tree_insert(tree, 60);
    tree_insert(tree, 80);
    display_tree_traversals(tree);
    
    printf("\n2. Searching for elements:\n");
    printf("Search 40: %s\n", tree_search(tree, 40) ? "found" : "not found");
    printf("Search 25: %s\n", tree_search(tree, 25) ? "found" : "not found");
    printf("Search 70: %s\n", tree_search(tree, 70) ? "found" : "not found");
    
    printf("\n3. Deleting element 30\n");
    tree_delete(tree, 30);
    display_tree_traversals(tree);
    
    printf("\n4. Deleting element 50 (root)\n");
    tree_delete(tree, 50);
    display_tree_traversals(tree);
    
    free_tree(tree);
    printf("\nBinary tree demonstration completed!\n");
}

void demonstrate_graph(void) {
    printf("\n=== GRAPH DEMONSTRATION ===\n");
    
    Graph* graph = create_graph(6);
    if (!graph) {
        printf("Failed to create graph\n");
        return;
    }
    
    printf("1. Creating graph with 6 vertices and adding edges:\n");
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 1, 3);
    add_edge(graph, 2, 4);
    add_edge(graph, 3, 5);
    add_edge(graph, 4, 5);
    display_graph(graph);
    
    printf("\n2. Graph traversals:\n");
    bfs(graph, 0);
    dfs(graph, 0);
    
    printf("\n3. Traversals from different starting points:\n");
    bfs(graph, 2);
    dfs(graph, 2);
    
    free_graph(graph);
    printf("\nGraph demonstration completed!\n");
}

void demonstrate_string_functions(void) {
    printf("\n=== CUSTOM STRING LIBRARY DEMONSTRATION ===\n");
    
    printf("1. String length function:\n");
    char test1[] = "Hello, World!";
    printf("Length of '%s': %d\n", test1, string_length(test1));
    
    printf("\n2. String copy function:\n");
    char source[] = "Source string";
    char dest[50];
    string_copy(source, dest);
    printf("Source: '%s'\n", source);
    printf("Destination: '%s'\n", dest);
    
    printf("\n3. String comparison function:\n");
    char str1[] = "apple";
    char str2[] = "banana";
    char str3[] = "apple";
    printf("Compare '%s' and '%s': %d\n", str1, str2, string_compare(str1, str2));
    printf("Compare '%s' and '%s': %d\n", str1, str3, string_compare(str1, str3));
    printf("Compare '%s' and '%s': %d\n", str2, str1, string_compare(str2, str1));
    
    printf("\n4. String concatenation function:\n");
    char* concat_result = string_concatenate("Hello, ", "World!");
    if (concat_result) {
        printf("Concatenated string: '%s'\n", concat_result);
        free(concat_result);
    }
    
    printf("\n5. Integer to string conversion:\n");
    int numbers[] = {123, -456, 0, 9999};
    for (int i = 0; i < 4; i++) {
        char* str_num = integer_to_string(numbers[i]);
        if (str_num) {
            printf("Integer %d -> String '%s'\n", numbers[i], str_num);
            free(str_num);
        }
    }
    
    printf("\n6. String to integer conversion:\n");
    char* str_numbers[] = {"123", "-456", "0", "999", "invalid", "12abc"};
    for (int i = 0; i < 6; i++) {
        if (is_valid_integer(str_numbers[i])) {
            int num = string_to_integer(str_numbers[i]);
            printf("String '%s' -> Integer %d\n", str_numbers[i], num);
        } else {
            printf("String '%s' -> Invalid integer\n", str_numbers[i]);
        }
    }
    
    printf("\nCustom string library demonstration completed!\n");
}

// =============================================================================
// UTILITY AND MENU FUNCTIONS
// =============================================================================

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_user_choice(void) {
    int choice;
    printf("\nEnter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        if (feof(stdin)) {
            return 0; // Exit gracefully on EOF
        }
        return -1;
    }
    clear_input_buffer();
    
    return choice;
}

void display_main_menu(void) {
    printf("\n" "========================================\n");
    printf("    C DATA STRUCTURES DEMONSTRATION\n");
    printf("========================================\n");
    printf("1. Linked List Operations\n");
    printf("2. Dynamic Array Operations\n");
    printf("3. Hash Table Operations\n");
    printf("4. Binary Tree Operations\n");
    printf("5. Graph Operations (BFS/DFS)\n");
    printf("6. Custom String Library\n");
    printf("7. Run All Demonstrations\n");
    printf("0. Exit\n");
    printf("========================================\n");
}

int main(void) {
    printf("Welcome to the Comprehensive C Data Structures Demo!\n");
    printf("This program demonstrates various data structures and algorithms.\n");
    
    int choice;
    do {
        display_main_menu();
        choice = get_user_choice();
        
        switch (choice) {
            case 1:
                demonstrate_linked_list();
                break;
            case 2:
                demonstrate_dynamic_array();
                break;
            case 3:
                demonstrate_hash_table();
                break;
            case 4:
                demonstrate_binary_tree();
                break;
            case 5:
                demonstrate_graph();
                break;
            case 6:
                demonstrate_string_functions();
                break;
            case 7:
                printf("\n=== RUNNING ALL DEMONSTRATIONS ===\n");
                demonstrate_linked_list();
                demonstrate_dynamic_array();
                demonstrate_hash_table();
                demonstrate_binary_tree();
                demonstrate_graph();
                demonstrate_string_functions();
                printf("\n=== ALL DEMONSTRATIONS COMPLETED ===\n");
                break;
            case 0:
                printf("\nThank you for using the Data Structures Demo!\n");
                break;
            default:
                printf("\nInvalid choice! Please enter a number between 0 and 7.\n");
                break;
        }
        
        if (choice != 0) {
            printf("\nPress Enter to continue...");
            if (!feof(stdin)) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {
                    if (c == EOF) break;
                }
            }
        }
        
    } while (choice != 0);
    
    return 0;
}