#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// LINKED LIST STRUCTURES AND FUNCTIONS
// =============================================================================

typedef struct Node {
  int data;
  struct Node *next;
} Node;

typedef struct LinkedList {
  Node *head;
  int size;
} LinkedList;

// Linked List Functions
LinkedList *create_list(void);
void insert_at_beginning(LinkedList *list, int data);
void insert_at_end(LinkedList *list, int data);
void insert_at_position(LinkedList *list, int data, int position);
bool delete_from_beginning(LinkedList *list);
bool delete_from_end(LinkedList *list);
bool delete_at_position(LinkedList *list, int position);
void display_list(LinkedList *list);
void free_list(LinkedList *list);

// =============================================================================
// DYNAMIC ARRAY STRUCTURES AND FUNCTIONS
// =============================================================================

typedef struct DynamicArray {
  int *data;
  int size;
  int capacity;
} DynamicArray;

// Dynamic Array Functions
DynamicArray *create_array(int initial_capacity);
void array_insert(DynamicArray *arr, int value);
void array_insert_at(DynamicArray *arr, int value, int index);
bool array_delete_at(DynamicArray *arr, int index);
int array_get(DynamicArray *arr, int index);
void array_set(DynamicArray *arr, int index, int value);
void display_array(DynamicArray *arr);
void free_array(DynamicArray *arr);

// =============================================================================
// HASH TABLE STRUCTURES AND FUNCTIONS
// =============================================================================

#define HASH_TABLE_SIZE 101

typedef struct HashNode {
  char *key;
  int value;
  struct HashNode *next;
} HashNode;

typedef struct HashTable {
  HashNode *buckets[HASH_TABLE_SIZE];
  int size;
} HashTable;

// Hash Table Functions
HashTable *create_hash_table(void);
unsigned int hash_function(const char *key);
void hash_insert(HashTable *table, const char *key, int value);
int hash_search(HashTable *table, const char *key, bool *found);
bool hash_delete(HashTable *table, const char *key);
void display_hash_table(HashTable *table);
void free_hash_table(HashTable *table);

// =============================================================================
// BINARY TREE STRUCTURES AND FUNCTIONS
// =============================================================================

typedef struct TreeNode {
  int data;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

typedef struct BinaryTree {
  TreeNode *root;
  int size;
} BinaryTree;

// Binary Tree Functions
BinaryTree *create_tree(void);
TreeNode *create_tree_node(int data);
void tree_insert(BinaryTree *tree, int data);
TreeNode *tree_insert_recursive(TreeNode *root, int data);
bool tree_search(BinaryTree *tree, int data);
TreeNode *tree_search_recursive(TreeNode *root, int data);
void tree_delete(BinaryTree *tree, int data);
TreeNode *tree_delete_recursive(TreeNode *root, int data);
TreeNode *find_min_node(TreeNode *root);
void inorder_traversal(TreeNode *root);
void preorder_traversal(TreeNode *root);
void postorder_traversal(TreeNode *root);
void display_tree_traversals(BinaryTree *tree);
void free_tree(BinaryTree *tree);
void free_tree_recursive(TreeNode *root);

// =============================================================================
// GRAPH STRUCTURES AND FUNCTIONS
// =============================================================================

#define MAX_VERTICES 100

typedef struct Graph {
  int adjacency_matrix[MAX_VERTICES][MAX_VERTICES];
  int num_vertices;
} Graph;

typedef struct Queue {
  int items[MAX_VERTICES];
  int front;
  int rear;
} Queue;

// Graph Functions
Graph *create_graph(int vertices);
void add_edge(Graph *graph, int src, int dest);
void display_graph(Graph *graph);
void bfs(Graph *graph, int start_vertex);
void dfs(Graph *graph, int start_vertex);
void dfs_recursive(Graph *graph, int vertex, bool visited[]);
void free_graph(Graph *graph);

// Queue Functions (for BFS)
Queue *create_queue(void);
void enqueue(Queue *q, int value);
int dequeue(Queue *q);
bool is_queue_empty(Queue *q);
void free_queue(Queue *q);

// =============================================================================
// CUSTOM STRING LIBRARY FUNCTIONS
// =============================================================================

// Basic String Functions
int string_length(const char *str);
void string_copy(const char *from, char *to);
int string_compare(const char *str1, const char *str2);
char *string_concatenate(const char *str1, const char *str2);

// Conversion Functions
char *integer_to_string(int num);
int string_to_integer(const char *str);
bool is_valid_integer(const char *str);

// =============================================================================
// UTILITY AND MENU FUNCTIONS
// =============================================================================

void display_main_menu(void);
void demonstrate_linked_list(void);
void demonstrate_dynamic_array(void);
void demonstrate_hash_table(void);
void demonstrate_binary_tree(void);
void demonstrate_graph(void);
void demonstrate_string_functions(void);
void clear_input_buffer(void);
int get_user_choice(void);

#endif // DATA_STRUCTURES_H