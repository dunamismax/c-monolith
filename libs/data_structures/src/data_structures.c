#include "data_structures.h"

// =============================================================================
// LINKED LIST IMPLEMENTATION
// =============================================================================

LinkedList *create_list(void) {
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  if (!list)
    return NULL;
  list->head = NULL;
  list->size = 0;
  return list;
}

void insert_at_beginning(LinkedList *list, int data) {
  if (!list)
    return;

  Node *new_node = (Node *)malloc(sizeof(Node));
  if (!new_node)
    return;

  new_node->data = data;
  new_node->next = list->head;
  list->head = new_node;
  list->size++;
}

void insert_at_end(LinkedList *list, int data) {
  if (!list)
    return;

  Node *new_node = (Node *)malloc(sizeof(Node));
  if (!new_node)
    return;

  new_node->data = data;
  new_node->next = NULL;

  if (list->head == NULL) {
    list->head = new_node;
  } else {
    Node *current = list->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }
  list->size++;
}

void insert_at_position(LinkedList *list, int data, int position) {
  if (!list || position < 0)
    return;

  if (position == 0) {
    insert_at_beginning(list, data);
    return;
  }

  Node *new_node = (Node *)malloc(sizeof(Node));
  if (!new_node)
    return;

  new_node->data = data;

  Node *current = list->head;
  for (int i = 0; i < position - 1 && current != NULL; i++) {
    current = current->next;
  }

  if (current == NULL) {
    free(new_node);
    return;
  }

  new_node->next = current->next;
  current->next = new_node;
  list->size++;
}

bool delete_from_beginning(LinkedList *list) {
  if (!list || list->head == NULL)
    return false;

  Node *temp = list->head;
  list->head = list->head->next;
  free(temp);
  list->size--;
  return true;
}

bool delete_from_end(LinkedList *list) {
  if (!list || list->head == NULL)
    return false;

  if (list->head->next == NULL) {
    free(list->head);
    list->head = NULL;
    list->size--;
    return true;
  }

  Node *current = list->head;
  while (current->next->next != NULL) {
    current = current->next;
  }

  free(current->next);
  current->next = NULL;
  list->size--;
  return true;
}

bool delete_at_position(LinkedList *list, int position) {
  if (!list || list->head == NULL || position < 0)
    return false;

  if (position == 0) {
    return delete_from_beginning(list);
  }

  Node *current = list->head;
  for (int i = 0; i < position - 1 && current->next != NULL; i++) {
    current = current->next;
  }

  if (current->next == NULL)
    return false;

  Node *temp = current->next;
  current->next = temp->next;
  free(temp);
  list->size--;
  return true;
}

void display_list(LinkedList *list) {
  if (!list || list->head == NULL) {
    printf("List is empty\n");
    return;
  }

  printf("List: ");
  Node *current = list->head;
  while (current != NULL) {
    printf("%d ", current->data);
    current = current->next;
  }
  printf("(Size: %d)\n", list->size);
}

void free_list(LinkedList *list) {
  if (!list)
    return;

  Node *current = list->head;
  while (current != NULL) {
    Node *temp = current;
    current = current->next;
    free(temp);
  }
  free(list);
}

// =============================================================================
// DYNAMIC ARRAY IMPLEMENTATION
// =============================================================================

DynamicArray *create_array(int initial_capacity) {
  if (initial_capacity <= 0)
    initial_capacity = 10;

  DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
  if (!arr)
    return NULL;

  arr->data = (int *)malloc(initial_capacity * sizeof(int));
  if (!arr->data) {
    free(arr);
    return NULL;
  }

  arr->size = 0;
  arr->capacity = initial_capacity;
  return arr;
}

static void resize_array(DynamicArray *arr) {
  if (!arr)
    return;

  int new_capacity = arr->capacity * 2;
  int *new_data = (int *)realloc(arr->data, new_capacity * sizeof(int));
  if (!new_data)
    return;

  arr->data = new_data;
  arr->capacity = new_capacity;
}

void array_insert(DynamicArray *arr, int value) {
  if (!arr)
    return;

  if (arr->size >= arr->capacity) {
    resize_array(arr);
  }

  arr->data[arr->size] = value;
  arr->size++;
}

void array_insert_at(DynamicArray *arr, int value, int index) {
  if (!arr || index < 0 || index > arr->size)
    return;

  if (arr->size >= arr->capacity) {
    resize_array(arr);
  }

  for (int i = arr->size; i > index; i--) {
    arr->data[i] = arr->data[i - 1];
  }

  arr->data[index] = value;
  arr->size++;
}

bool array_delete_at(DynamicArray *arr, int index) {
  if (!arr || index < 0 || index >= arr->size)
    return false;

  for (int i = index; i < arr->size - 1; i++) {
    arr->data[i] = arr->data[i + 1];
  }

  arr->size--;
  return true;
}

int array_get(DynamicArray *arr, int index) {
  if (!arr || index < 0 || index >= arr->size)
    return -1;
  return arr->data[index];
}

void array_set(DynamicArray *arr, int index, int value) {
  if (!arr || index < 0 || index >= arr->size)
    return;
  arr->data[index] = value;
}

void display_array(DynamicArray *arr) {
  if (!arr || arr->size == 0) {
    printf("Array is empty\n");
    return;
  }

  printf("Array: [");
  for (int i = 0; i < arr->size; i++) {
    printf("%d", arr->data[i]);
    if (i < arr->size - 1)
      printf(", ");
  }
  printf("] (Size: %d, Capacity: %d)\n", arr->size, arr->capacity);
}

void free_array(DynamicArray *arr) {
  if (!arr)
    return;
  free(arr->data);
  free(arr);
}

// =============================================================================
// HASH TABLE IMPLEMENTATION
// =============================================================================

HashTable *create_hash_table(void) {
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  if (!table)
    return NULL;

  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    table->buckets[i] = NULL;
  }
  table->size = 0;
  return table;
}

unsigned int hash_function(const char *key) {
  unsigned int hash = 5381;
  int c;

  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % HASH_TABLE_SIZE;
}

void hash_insert(HashTable *table, const char *key, int value) {
  if (!table || !key)
    return;

  unsigned int index = hash_function(key);
  HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
  if (!new_node)
    return;

  new_node->key = (char *)malloc(strlen(key) + 1);
  if (!new_node->key) {
    free(new_node);
    return;
  }

  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->next = table->buckets[index];
  table->buckets[index] = new_node;
  table->size++;
}

int hash_search(HashTable *table, const char *key, bool *found) {
  if (!table || !key || !found) {
    if (found)
      *found = false;
    return -1;
  }

  unsigned int index = hash_function(key);
  HashNode *current = table->buckets[index];

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      *found = true;
      return current->value;
    }
    current = current->next;
  }

  *found = false;
  return -1;
}

bool hash_delete(HashTable *table, const char *key) {
  if (!table || !key)
    return false;

  unsigned int index = hash_function(key);
  HashNode *current = table->buckets[index];
  HashNode *prev = NULL;

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (prev == NULL) {
        table->buckets[index] = current->next;
      } else {
        prev->next = current->next;
      }

      free(current->key);
      free(current);
      table->size--;
      return true;
    }
    prev = current;
    current = current->next;
  }

  return false;
}

void display_hash_table(HashTable *table) {
  if (!table)
    return;

  printf("Hash Table (Size: %d):\n", table->size);
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    HashNode *current = table->buckets[i];
    if (current != NULL) {
      printf("Bucket %d: ", i);
      while (current != NULL) {
        printf("(%s: %d) ", current->key, current->value);
        current = current->next;
      }
      printf("\n");
    }
  }
}

void free_hash_table(HashTable *table) {
  if (!table)
    return;

  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    HashNode *current = table->buckets[i];
    while (current != NULL) {
      HashNode *temp = current;
      current = current->next;
      free(temp->key);
      free(temp);
    }
  }
  free(table);
}

// =============================================================================
// BINARY TREE IMPLEMENTATION
// =============================================================================

BinaryTree *create_tree(void) {
  BinaryTree *tree = (BinaryTree *)malloc(sizeof(BinaryTree));
  if (!tree)
    return NULL;
  tree->root = NULL;
  tree->size = 0;
  return tree;
}

TreeNode *create_tree_node(int data) {
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
  if (!node)
    return NULL;
  node->data = data;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void tree_insert(BinaryTree *tree, int data) {
  if (!tree)
    return;
  tree->root = tree_insert_recursive(tree->root, data);
  tree->size++;
}

TreeNode *tree_insert_recursive(TreeNode *root, int data) {
  if (root == NULL) {
    return create_tree_node(data);
  }

  if (data < root->data) {
    root->left = tree_insert_recursive(root->left, data);
  } else if (data > root->data) {
    root->right = tree_insert_recursive(root->right, data);
  }

  return root;
}

bool tree_search(BinaryTree *tree, int data) {
  if (!tree)
    return false;
  return tree_search_recursive(tree->root, data) != NULL;
}

TreeNode *tree_search_recursive(TreeNode *root, int data) {
  if (root == NULL || root->data == data) {
    return root;
  }

  if (data < root->data) {
    return tree_search_recursive(root->left, data);
  }

  return tree_search_recursive(root->right, data);
}

void tree_delete(BinaryTree *tree, int data) {
  if (!tree)
    return;
  // Only delete if the element exists
  if (tree_search_recursive(tree->root, data)) {
    tree->root = tree_delete_recursive(tree->root, data);
    tree->size--;
  }
}

TreeNode *tree_delete_recursive(TreeNode *root, int data) {
  if (root == NULL)
    return root;

  if (data < root->data) {
    root->left = tree_delete_recursive(root->left, data);
  } else if (data > root->data) {
    root->right = tree_delete_recursive(root->right, data);
  } else {
    if (root->left == NULL) {
      TreeNode *temp = root->right;
      free(root);
      return temp;
    } else if (root->right == NULL) {
      TreeNode *temp = root->left;
      free(root);
      return temp;
    }

    TreeNode *temp = find_min_node(root->right);
    root->data = temp->data;
    root->right = tree_delete_recursive(root->right, temp->data);
  }
  return root;
}

TreeNode *find_min_node(TreeNode *root) {
  while (root && root->left != NULL) {
    root = root->left;
  }
  return root;
}

void inorder_traversal(TreeNode *root) {
  if (root != NULL) {
    inorder_traversal(root->left);
    printf("%d ", root->data);
    inorder_traversal(root->right);
  }
}

void preorder_traversal(TreeNode *root) {
  if (root != NULL) {
    printf("%d ", root->data);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
  }
}

void postorder_traversal(TreeNode *root) {
  if (root != NULL) {
    postorder_traversal(root->left);
    postorder_traversal(root->right);
    printf("%d ", root->data);
  }
}

void display_tree_traversals(BinaryTree *tree) {
  if (!tree || tree->root == NULL) {
    printf("Tree is empty\n");
    return;
  }

  printf("Tree Traversals (Size: %d):\n", tree->size);
  printf("In-order: ");
  inorder_traversal(tree->root);
  printf("\n");

  printf("Pre-order: ");
  preorder_traversal(tree->root);
  printf("\n");

  printf("Post-order: ");
  postorder_traversal(tree->root);
  printf("\n");
}

void free_tree(BinaryTree *tree) {
  if (!tree)
    return;
  free_tree_recursive(tree->root);
  free(tree);
}

void free_tree_recursive(TreeNode *root) {
  if (root != NULL) {
    free_tree_recursive(root->left);
    free_tree_recursive(root->right);
    free(root);
  }
}

// =============================================================================
// GRAPH IMPLEMENTATION
// =============================================================================

Graph *create_graph(int vertices) {
  if (vertices <= 0 || vertices > MAX_VERTICES)
    return NULL;

  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (!graph)
    return NULL;

  graph->num_vertices = vertices;

  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      graph->adjacency_matrix[i][j] = 0;
    }
  }

  return graph;
}

void add_edge(Graph *graph, int src, int dest) {
  if (!graph || src < 0 || src >= graph->num_vertices || dest < 0 ||
      dest >= graph->num_vertices)
    return;

  graph->adjacency_matrix[src][dest] = 1;
  graph->adjacency_matrix[dest][src] = 1; // Undirected graph
}

void display_graph(Graph *graph) {
  if (!graph)
    return;

  printf("Graph Adjacency Matrix (%d vertices):\n", graph->num_vertices);
  printf("   ");
  for (int i = 0; i < graph->num_vertices; i++) {
    printf("%2d ", i);
  }
  printf("\n");

  for (int i = 0; i < graph->num_vertices; i++) {
    printf("%2d ", i);
    for (int j = 0; j < graph->num_vertices; j++) {
      printf("%2d ", graph->adjacency_matrix[i][j]);
    }
    printf("\n");
  }
}

Queue *create_queue(void) {
  Queue *q = (Queue *)malloc(sizeof(Queue));
  if (!q)
    return NULL;
  q->front = -1;
  q->rear = -1;
  return q;
}

void enqueue(Queue *q, int value) {
  if (!q)
    return;
  if (q->rear == MAX_VERTICES - 1)
    return;

  if (q->front == -1) {
    q->front = 0;
  }
  q->rear++;
  q->items[q->rear] = value;
}

int dequeue(Queue *q) {
  if (!q || q->front == -1)
    return -1;

  int item = q->items[q->front];
  q->front++;

  if (q->front > q->rear) {
    q->front = q->rear = -1;
  }

  return item;
}

bool is_queue_empty(Queue *q) { return (!q || q->front == -1); }

void free_queue(Queue *q) {
  if (q)
    free(q);
}

void bfs(Graph *graph, int start_vertex) {
  if (!graph || start_vertex < 0 || start_vertex >= graph->num_vertices)
    return;

  bool visited[MAX_VERTICES] = {false};
  Queue *q = create_queue();

  visited[start_vertex] = true;
  enqueue(q, start_vertex);

  printf("BFS Traversal starting from vertex %d: ", start_vertex);

  while (!is_queue_empty(q)) {
    int current_vertex = dequeue(q);
    printf("%d ", current_vertex);

    for (int i = 0; i < graph->num_vertices; i++) {
      if (graph->adjacency_matrix[current_vertex][i] == 1 && !visited[i]) {
        visited[i] = true;
        enqueue(q, i);
      }
    }
  }

  printf("\n");
  free_queue(q);
}

void dfs(Graph *graph, int start_vertex) {
  if (!graph || start_vertex < 0 || start_vertex >= graph->num_vertices)
    return;

  bool visited[MAX_VERTICES] = {false};
  printf("DFS Traversal starting from vertex %d: ", start_vertex);
  dfs_recursive(graph, start_vertex, visited);
  printf("\n");
}

void dfs_recursive(Graph *graph, int vertex, bool visited[]) {
  visited[vertex] = true;
  printf("%d ", vertex);

  for (int i = 0; i < graph->num_vertices; i++) {
    if (graph->adjacency_matrix[vertex][i] == 1 && !visited[i]) {
      dfs_recursive(graph, i, visited);
    }
  }
}

void free_graph(Graph *graph) {
  if (graph)
    free(graph);
}

// =============================================================================
// CUSTOM STRING LIBRARY IMPLEMENTATION
// =============================================================================

int string_length(const char *str) {
  if (!str)
    return 0;

  int length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

void string_copy(const char *from, char *to) {
  if (!from || !to)
    return;

  int i = 0;
  while (from[i] != '\0') {
    to[i] = from[i];
    i++;
  }
  to[i] = '\0';
}

int string_compare(const char *str1, const char *str2) {
  if (!str1 || !str2)
    return 0;

  int i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] < str2[i])
      return -1;
    if (str1[i] > str2[i])
      return 1;
    i++;
  }

  if (str1[i] == '\0' && str2[i] == '\0')
    return 0;
  if (str1[i] == '\0')
    return -1;
  return 1;
}

char *string_concatenate(const char *str1, const char *str2) {
  if (!str1 || !str2)
    return NULL;

  int len1 = string_length(str1);
  int len2 = string_length(str2);

  char *result = (char *)malloc(len1 + len2 + 1);
  if (!result)
    return NULL;

  // Copy first string
  int i;
  for (i = 0; str1[i] != '\0'; i++) {
    result[i] = str1[i];
  }

  // Copy second string
  for (int j = 0; str2[j] != '\0'; j++) {
    result[i + j] = str2[j];
  }

  // Null terminate
  result[len1 + len2] = '\0';

  return result;
}

char *integer_to_string(int num) {
  if (num == 0) {
    char *result = (char *)malloc(2);
    if (!result)
      return NULL;
    result[0] = '0';
    result[1] = '\0';
    return result;
  }

  bool negative = num < 0;
  if (negative)
    num = -num;

  int temp = num;
  int digits = 0;
  while (temp > 0) {
    digits++;
    temp /= 10;
  }

  char *result = (char *)malloc(digits + negative + 1);
  if (!result)
    return NULL;

  result[digits + negative] = '\0';

  for (int i = digits + negative - 1; i >= negative; i--) {
    result[i] = '0' + (num % 10);
    num /= 10;
  }

  if (negative) {
    result[0] = '-';
  }

  return result;
}

int string_to_integer(const char *str) {
  if (!str || !is_valid_integer(str))
    return 0;

  int result = 0;
  int sign = 1;
  int i = 0;

  if (str[0] == '-') {
    sign = -1;
    i = 1;
  } else if (str[0] == '+') {
    i = 1;
  }

  while (str[i] != '\0') {
    result = result * 10 + (str[i] - '0');
    i++;
  }

  return result * sign;
}

bool is_valid_integer(const char *str) {
  if (!str || str[0] == '\0')
    return false;

  int i = 0;
  if (str[0] == '-' || str[0] == '+') {
    i = 1;
    if (str[1] == '\0')
      return false;
  }

  while (str[i] != '\0') {
    if (str[i] < '0' || str[i] > '9')
      return false;
    i++;
  }

  return true;
}