#include "compressor.h"
#include <limits.h>

static int compare_nodes(const void *a, const void *b) {
  huffman_node_t *node_a = *(huffman_node_t **)a;
  huffman_node_t *node_b = *(huffman_node_t **)b;

  if (node_a->frequency != node_b->frequency) {
    return (node_a->frequency > node_b->frequency) ? 1 : -1;
  }

  // Tie-breaking: prefer leaf nodes
  if (node_a->is_leaf != node_b->is_leaf) {
    return node_a->is_leaf ? -1 : 1;
  }

  return 0;
}

static huffman_node_t *create_huffman_node(unsigned char symbol,
                                           unsigned long frequency,
                                           bool is_leaf) {
  huffman_node_t *node = malloc(sizeof(huffman_node_t));
  if (!node) {
    return NULL;
  }

  node->symbol = symbol;
  node->frequency = frequency;
  node->is_leaf = is_leaf;
  node->left = NULL;
  node->right = NULL;

  return node;
}

huffman_node_t *build_huffman_tree(unsigned long *frequencies) {
  huffman_node_t **nodes = malloc(256 * sizeof(huffman_node_t *));
  if (!nodes) {
    return NULL;
  }

  int node_count = 0;

  // Create leaf nodes for all symbols with non-zero frequency
  for (int i = 0; i < 256; i++) {
    if (frequencies[i] > 0) {
      nodes[node_count] =
          create_huffman_node((unsigned char)i, frequencies[i], true);
      if (!nodes[node_count]) {
        // Cleanup on failure
        for (int j = 0; j < node_count; j++) {
          free(nodes[j]);
        }
        free(nodes);
        return NULL;
      }
      node_count++;
    }
  }

  // Handle edge case: empty input or single character
  if (node_count == 0) {
    free(nodes);
    return NULL;
  } else if (node_count == 1) {
    // Single character - create a dummy parent node
    huffman_node_t *root = create_huffman_node(0, nodes[0]->frequency, false);
    if (root) {
      root->left = nodes[0];
      root->right = NULL;
    }
    free(nodes);
    return root;
  }

  // Build the Huffman tree using a min-heap approach
  while (node_count > 1) {
    // Sort nodes by frequency (simple sort for clarity)
    qsort(nodes, node_count, sizeof(huffman_node_t *), compare_nodes);

    // Take the two nodes with lowest frequency
    huffman_node_t *left = nodes[0];
    huffman_node_t *right = nodes[1];

    // Create new internal node
    huffman_node_t *parent =
        create_huffman_node(0, left->frequency + right->frequency, false);
    if (!parent) {
      // Cleanup on failure
      for (int i = 0; i < node_count; i++) {
        free_huffman_tree(nodes[i]);
      }
      free(nodes);
      return NULL;
    }

    parent->left = left;
    parent->right = right;

    // Replace the first two nodes with the new parent
    nodes[0] = parent;
    for (int i = 1; i < node_count - 1; i++) {
      nodes[i] = nodes[i + 1];
    }
    node_count--;
  }

  huffman_node_t *root = nodes[0];
  free(nodes);
  return root;
}

void generate_huffman_codes(huffman_node_t *root, huffman_code_t *codes,
                            char *current_code, int depth) {
  if (!root) {
    return;
  }

  if (root->is_leaf) {
    if (depth == 0) {
      // Single character case - assign a default code
      strcpy(codes[root->symbol].code, "0");
      codes[root->symbol].length = 1;
    } else {
      strncpy(codes[root->symbol].code, current_code, depth);
      codes[root->symbol].code[depth] = '\0';
      codes[root->symbol].length = depth;
    }
    return;
  }

  if (depth >= 255) {
    // Prevent buffer overflow
    return;
  }

  // Traverse left (add '0')
  if (root->left) {
    current_code[depth] = '0';
    generate_huffman_codes(root->left, codes, current_code, depth + 1);
  }

  // Traverse right (add '1')
  if (root->right) {
    current_code[depth] = '1';
    generate_huffman_codes(root->right, codes, current_code, depth + 1);
  }
}

int huffman_compress(const unsigned char *input, size_t input_size,
                     unsigned char **output, size_t *output_size) {
  if (!input || input_size == 0 || !output || !output_size) {
    return -1;
  }

  // Calculate frequency table
  unsigned long frequencies[256] = {0};
  for (size_t i = 0; i < input_size; i++) {
    frequencies[input[i]]++;
  }

  // Build Huffman tree
  huffman_node_t *root = build_huffman_tree(frequencies);
  if (!root) {
    return -1;
  }

  // Generate Huffman codes
  huffman_code_t codes[256];
  memset(codes, 0, sizeof(codes));
  char current_code[256];
  generate_huffman_codes(root, codes, current_code, 0);

  // Create bit buffer for output
  bit_buffer_t *bit_buf = create_bit_buffer(input_size / 2 + 1024);
  if (!bit_buf) {
    free_huffman_tree(root);
    return -1;
  }

  // Write frequency table to output (for decompression)
  for (int i = 0; i < 256; i++) {
    write_bits(bit_buf, (uint32_t)frequencies[i], 32);
  }

  // Compress the data
  for (size_t i = 0; i < input_size; i++) {
    unsigned char symbol = input[i];
    const char *code = codes[symbol].code;
    int code_len = codes[symbol].length;

    // Write the Huffman code bit by bit
    for (int j = 0; j < code_len; j++) {
      write_bits(bit_buf, (code[j] == '1') ? 1 : 0, 1);
    }
  }

  // Pad the last byte if necessary
  if (bit_buf->bit_pos > 0) {
    write_bits(bit_buf, 0, 8 - bit_buf->bit_pos);
  }

  // Prepare output
  *output_size = bit_buf->byte_pos;
  *output = malloc(*output_size);
  if (!*output) {
    free_bit_buffer(bit_buf);
    free_huffman_tree(root);
    return -1;
  }

  memcpy(*output, bit_buf->buffer, *output_size);

  // Cleanup
  free_bit_buffer(bit_buf);
  free_huffman_tree(root);

  return 0;
}

int huffman_decompress(const unsigned char *input, size_t input_size,
                       unsigned char **output, size_t *output_size) {
  if (!input || input_size < 256 * 4 || !output || !output_size) {
    return -1;
  }

  // Read frequency table
  unsigned long frequencies[256];
  bit_buffer_t *bit_buf = create_bit_buffer(input_size);
  if (!bit_buf) {
    return -1;
  }

  memcpy(bit_buf->buffer, input, input_size);

  for (int i = 0; i < 256; i++) {
    frequencies[i] = read_bits(bit_buf, 32);
  }

  // Calculate original size
  size_t original_size = 0;
  for (int i = 0; i < 256; i++) {
    original_size += frequencies[i];
  }

  if (original_size == 0) {
    free_bit_buffer(bit_buf);
    return -1;
  }

  // Build Huffman tree
  huffman_node_t *root = build_huffman_tree(frequencies);
  if (!root) {
    free_bit_buffer(bit_buf);
    return -1;
  }

  // Allocate output buffer
  *output = malloc(original_size);
  if (!*output) {
    free_bit_buffer(bit_buf);
    free_huffman_tree(root);
    return -1;
  }

  // Decompress the data
  size_t decoded_bytes = 0;
  huffman_node_t *current = root;

  while (decoded_bytes < original_size &&
         (bit_buf->byte_pos < input_size || bit_buf->bit_pos > 0)) {

    if (current->is_leaf) {
      (*output)[decoded_bytes++] = current->symbol;
      current = root; // Reset to root for next symbol
      continue;
    }

    // Handle single character case
    if (!current->right && current->left && current->left->is_leaf) {
      (*output)[decoded_bytes++] = current->left->symbol;
      current = root;
      continue;
    }

    // Read next bit
    uint32_t bit = read_bits(bit_buf, 1);
    if (bit_buf->byte_pos >= input_size && bit_buf->bit_pos == 0) {
      break; // End of input
    }

    // Navigate tree
    if (bit == 0 && current->left) {
      current = current->left;
    } else if (bit == 1 && current->right) {
      current = current->right;
    } else {
      // Invalid path
      free(*output);
      *output = NULL;
      free_bit_buffer(bit_buf);
      free_huffman_tree(root);
      return -1;
    }
  }

  *output_size = decoded_bytes;

  // Cleanup
  free_bit_buffer(bit_buf);
  free_huffman_tree(root);

  return 0;
}

void free_huffman_tree(huffman_node_t *root) {
  if (!root) {
    return;
  }

  free_huffman_tree(root->left);
  free_huffman_tree(root->right);
  free(root);
}