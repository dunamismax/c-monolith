#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILENAME_LEN 256
#define BUFFER_SIZE 8192
#define MAX_MATCH_LENGTH 258
#define WINDOW_SIZE 32768
#define MIN_MATCH_LENGTH 3
#define HASH_TABLE_SIZE 65536
#define MAGIC_BYTES 0x434F4D50  // "COMP"
#define VERSION 1

// Compression algorithms
typedef enum {
    ALGO_HUFFMAN = 1,
    ALGO_LZ77 = 2,
    ALGO_HYBRID = 3  // LZ77 + Huffman
} compression_algorithm_t;

// Compression level
typedef enum {
    LEVEL_FAST = 1,
    LEVEL_NORMAL = 5,
    LEVEL_BEST = 9
} compression_level_t;

// File header structure
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t algorithm;
    uint32_t level;
    uint32_t original_size;
    uint32_t compressed_size;
    uint32_t crc32;
    uint64_t timestamp;
    char original_filename[MAX_FILENAME_LEN];
} __attribute__((packed)) file_header_t;

// Huffman tree node
typedef struct huffman_node {
    unsigned char symbol;
    unsigned long frequency;
    bool is_leaf;
    struct huffman_node *left;
    struct huffman_node *right;
} huffman_node_t;

// Huffman code table entry
typedef struct {
    char code[256];
    int length;
} huffman_code_t;

// LZ77 match structure
typedef struct {
    uint16_t distance;
    uint16_t length;
    unsigned char next_char;
} lz77_match_t;

// Hash table entry for LZ77
typedef struct hash_entry {
    uint32_t position;
    struct hash_entry *next;
} hash_entry_t;

// Bit buffer for efficient bit manipulation
typedef struct {
    unsigned char *buffer;
    size_t buffer_size;
    size_t byte_pos;
    int bit_pos;
} bit_buffer_t;

// Progress callback function type
typedef void (*progress_callback_t)(double percentage, const char *status);

// Compression context
typedef struct {
    compression_algorithm_t algorithm;
    compression_level_t level;
    progress_callback_t progress_callback;
    bool verbose;
    bool force_overwrite;
    bool keep_original;
} compression_context_t;

// Function prototypes

// Main compression/decompression functions
int compress_file(const char *input_path, const char *output_path, 
                 compression_context_t *ctx);
int decompress_file(const char *input_path, const char *output_path,
                   compression_context_t *ctx);

// Huffman coding functions
huffman_node_t *build_huffman_tree(unsigned long *frequencies);
void generate_huffman_codes(huffman_node_t *root, huffman_code_t *codes, 
                           char *current_code, int depth);
int huffman_compress(const unsigned char *input, size_t input_size,
                    unsigned char **output, size_t *output_size);
int huffman_decompress(const unsigned char *input, size_t input_size,
                      unsigned char **output, size_t *output_size);
void free_huffman_tree(huffman_node_t *root);

// LZ77 compression functions
int lz77_compress(const unsigned char *input, size_t input_size,
                 unsigned char **output, size_t *output_size, int level);
int lz77_decompress(const unsigned char *input, size_t input_size,
                   unsigned char **output, size_t *output_size);
lz77_match_t find_longest_match(const unsigned char *data, size_t pos,
                               size_t data_size, hash_entry_t **hash_table);
uint32_t hash_function(const unsigned char *data);

// Utility functions
uint32_t calculate_crc32(const unsigned char *data, size_t length);
void print_progress_bar(double percentage, const char *status);
void print_compression_stats(size_t original_size, size_t compressed_size,
                           double elapsed_time);
bool file_exists(const char *path);
size_t get_file_size(const char *path);
char *generate_output_filename(const char *input_path, bool compress);

// Bit buffer functions
bit_buffer_t *create_bit_buffer(size_t initial_size);
void write_bits(bit_buffer_t *buffer, uint32_t value, int num_bits);
uint32_t read_bits(bit_buffer_t *buffer, int num_bits);
void free_bit_buffer(bit_buffer_t *buffer);

// Error handling
void print_error(const char *message);
void print_usage(const char *program_name);

#endif // COMPRESSOR_H