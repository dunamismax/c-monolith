#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Include the test framework
#include "../test_framework.h"

// Include the data structures header
#include "../../apps/data/data_structures_demo/src/data_structures.h"

// Test binary tree creation
int test_create_tree(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;
  if (tree->root != NULL)
    return 0;
  if (tree->size != 0)
    return 0;
  free_tree(tree);
  return 1;
}

// Test tree node creation
int test_create_tree_node(void) {
  TreeNode *node = create_tree_node(42);
  if (!node)
    return 0;
  if (node->data != 42)
    return 0;
  if (node->left != NULL)
    return 0;
  if (node->right != NULL)
    return 0;
  free(node);
  return 1;
}

// Test tree insertion
int test_tree_insert(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Insert root
  tree_insert(tree, 50);
  if (tree->size != 1 || tree->root->data != 50) {
    free_tree(tree);
    return 0;
  }

  // Insert left child
  tree_insert(tree, 30);
  if (tree->size != 2 || tree->root->left->data != 30) {
    free_tree(tree);
    return 0;
  }

  // Insert right child
  tree_insert(tree, 70);
  if (tree->size != 3 || tree->root->right->data != 70) {
    free_tree(tree);
    return 0;
  }

  // Insert duplicate (should still increment size)
  tree_insert(tree, 50);
  if (tree->size != 4) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test tree search
int test_tree_search(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Insert some values
  tree_insert(tree, 50);
  tree_insert(tree, 30);
  tree_insert(tree, 70);
  tree_insert(tree, 20);
  tree_insert(tree, 40);
  tree_insert(tree, 60);
  tree_insert(tree, 80);

  // Test successful searches
  if (!tree_search(tree, 50)) {
    free_tree(tree);
    return 0;
  }
  if (!tree_search(tree, 30)) {
    free_tree(tree);
    return 0;
  }
  if (!tree_search(tree, 80)) {
    free_tree(tree);
    return 0;
  }

  // Test unsuccessful searches
  if (tree_search(tree, 25)) {
    free_tree(tree);
    return 0;
  }
  if (tree_search(tree, 100)) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test tree deletion
int test_tree_delete(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Build a tree
  tree_insert(tree, 50);
  tree_insert(tree, 30);
  tree_insert(tree, 70);
  tree_insert(tree, 20);
  tree_insert(tree, 40);
  tree_insert(tree, 60);
  tree_insert(tree, 80);

  // Delete leaf node
  tree_delete(tree, 20);
  if (tree->size != 6) {
    free_tree(tree);
    return 0;
  }
  if (tree_search(tree, 20)) {
    free_tree(tree);
    return 0;
  }

  // Delete node with one child
  tree_delete(tree, 30);
  if (tree->size != 5) {
    free_tree(tree);
    return 0;
  }
  if (tree_search(tree, 30)) {
    free_tree(tree);
    return 0;
  }
  // 40 should now be in place of 30
  if (!tree_search(tree, 40)) {
    free_tree(tree);
    return 0;
  }

  // Delete node with two children (root)
  tree_delete(tree, 50);
  if (tree->size != 4) {
    free_tree(tree);
    return 0;
  }
  if (tree_search(tree, 50)) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test find_min_node helper function
int test_find_min_node(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Insert values
  tree_insert(tree, 50);
  tree_insert(tree, 30);
  tree_insert(tree, 70);
  tree_insert(tree, 20);
  tree_insert(tree, 40);
  tree_insert(tree, 60);
  tree_insert(tree, 80);

  // Find minimum in right subtree
  TreeNode *min_node = find_min_node(tree->root->right);
  if (!min_node || min_node->data != 60) {
    free_tree(tree);
    return 0;
  }

  // Find minimum in left subtree
  min_node = find_min_node(tree->root->left);
  if (!min_node || min_node->data != 20) {
    free_tree(tree);
    return 0;
  }

  // Find minimum from root
  min_node = find_min_node(tree->root);
  if (!min_node || min_node->data != 20) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test BST property maintenance
int test_bst_property(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Insert in random order
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
  int num_values = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < num_values; i++) {
    tree_insert(tree, values[i]);
  }

  // Verify BST property by checking inorder traversal would be sorted
  // We can't easily capture output, so we'll check specific searches
  for (int i = 0; i < num_values; i++) {
    if (!tree_search(tree, values[i])) {
      free_tree(tree);
      return 0;
    }
  }

  // Check that non-existent values are not found
  if (tree_search(tree, 15) || tree_search(tree, 75)) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test tree operations on empty tree
int test_empty_tree_operations(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Search in empty tree
  if (tree_search(tree, 50)) {
    free_tree(tree);
    return 0;
  }

  // Delete from empty tree (should handle gracefully)
  tree_delete(tree, 50);
  if (tree->size != 0) {
    free_tree(tree);
    return 0;
  }

  // Find min in empty tree (should handle gracefully)
  TreeNode *min_node = find_min_node(tree->root);
  if (min_node != NULL) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test single node tree operations
int test_single_node_operations(void) {
  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  tree_insert(tree, 42);

  // Search should find the single node
  if (!tree_search(tree, 42)) {
    free_tree(tree);
    return 0;
  }

  // Find min should return the single node
  TreeNode *min_node = find_min_node(tree->root);
  if (!min_node || min_node->data != 42) {
    free_tree(tree);
    return 0;
  }

  // Delete the single node
  tree_delete(tree, 42);
  if (tree->size != 0 || tree->root != NULL) {
    free_tree(tree);
    return 0;
  }

  free_tree(tree);
  return 1;
}

// Test edge cases and error handling
int test_edge_cases(void) {
  // Test with NULL tree
  tree_insert(NULL, 50);
  if (tree_search(NULL, 50))
    return 0;
  tree_delete(NULL, 50);

  BinaryTree *tree = create_tree();
  if (!tree)
    return 0;

  // Test repeated operations
  tree_insert(tree, 50);
  tree_insert(tree, 50);
  tree_insert(tree, 50);

  // Should still be searchable
  if (!tree_search(tree, 50)) {
    free_tree(tree);
    return 0;
  }

  // Delete should work multiple times
  tree_delete(tree, 50);
  tree_delete(tree, 50); // Should handle gracefully even if not found

  free_tree(tree);
  return 1;
}

int main(void) {
  printf("Data Structures Demo - Binary Tree Test Suite\n");
  printf("==============================================\n\n");

  run_test(test_create_tree, "test_create_tree");
  run_test(test_create_tree_node, "test_create_tree_node");
  run_test(test_tree_insert, "test_tree_insert");
  run_test(test_tree_search, "test_tree_search");
  run_test(test_tree_delete, "test_tree_delete");
  run_test(test_find_min_node, "test_find_min_node");
  run_test(test_bst_property, "test_bst_property");
  run_test(test_empty_tree_operations, "test_empty_tree_operations");
  run_test(test_single_node_operations, "test_single_node_operations");
  run_test(test_edge_cases, "test_edge_cases");

  return test_summary("Binary Tree Test Suite");
}