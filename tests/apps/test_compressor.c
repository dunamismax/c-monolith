#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Include the test framework
#include "../test_framework.h"

// Include the compressor library
#include "../../apps/cli/compressor/src/compressor.h"

// Test data
static const char *test_string_small = "Hello, World! This is a test string for compression.";
static const char *test_string_repetitive = "aaaaaabbbbbbccccccddddddeeeeeeffffffgggggghhhhhhiiiiiijjjjjjkkkkkk";

// Helper function to create temporary file
static char *create_temp_file(const char *content) {
    char *temp_path = malloc(256);
    strcpy(temp_path, "/tmp/compressor_test_XXXXXX");
    
    int fd = mkstemp(temp_path);
    if (fd == -1) {
        free(temp_path);
        return NULL;
    }
    
    if (content) {
        if (write(fd, content, strlen(content)) == -1) {
            close(fd);
            free(temp_path);
            return NULL;
        }
    }
    close(fd);
    
    return temp_path;
}

// Helper function to read file contents
static char *read_file_contents(const char *path, size_t *size) {
    FILE *file = fopen(path, "rb");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *content = malloc(*size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    
    if (fread(content, 1, *size, file) != *size) {
        free(content);
        fclose(file);
        return NULL;
    }
    content[*size] = '\0';
    fclose(file);
    
    return content;
}

// Test CRC32 calculation
int test_crc32_calculation(void) {
    const char *test_data = "Hello, World!";
    uint32_t crc = calculate_crc32((const unsigned char *)test_data, strlen(test_data));
    
    // CRC32 should not be zero for non-empty data
    if (crc == 0) return 0;
    
    // Test with empty data
    uint32_t empty_crc = calculate_crc32((const unsigned char *)"", 0);
    if (empty_crc != 0) return 0;
    
    // Test consistency
    uint32_t crc2 = calculate_crc32((const unsigned char *)test_data, strlen(test_data));
    if (crc != crc2) return 0;
    
    return 1;
}

// Test bit buffer operations
int test_bit_buffer_operations(void) {
    bit_buffer_t *buffer = create_bit_buffer(100);
    if (!buffer) return 0;
    
    // Test writing and reading bits
    write_bits(buffer, 0x5A, 8);  // 01011010
    write_bits(buffer, 0x3, 4);   // 0011
    
    // Reset for reading
    buffer->byte_pos = 0;
    buffer->bit_pos = 0;
    
    uint32_t value1 = read_bits(buffer, 8);
    uint32_t value2 = read_bits(buffer, 4);
    
    free_bit_buffer(buffer);
    
    if (value1 != 0x5A) return 0;
    if (value2 != 0x3) return 0;
    
    return 1;
}

// Test Huffman compression/decompression
int test_huffman_compression(void) {
    const unsigned char *input = (const unsigned char *)test_string_small;
    size_t input_size = strlen(test_string_small);
    
    unsigned char *compressed = NULL;
    size_t compressed_size = 0;
    
    // Test compression
    int result = huffman_compress(input, input_size, &compressed, &compressed_size);
    if (result != 0) return 0;
    if (!compressed) return 0;
    if (compressed_size == 0) return 0;
    
    // Test decompression
    unsigned char *decompressed = NULL;
    size_t decompressed_size = 0;
    
    result = huffman_decompress(compressed, compressed_size, &decompressed, &decompressed_size);
    if (result != 0) {
        free(compressed);
        return 0;
    }
    if (!decompressed) {
        free(compressed);
        return 0;
    }
    if (decompressed_size != input_size) {
        free(compressed);
        free(decompressed);
        return 0;
    }
    
    // Verify content
    int content_match = (memcmp(input, decompressed, input_size) == 0);
    
    free(compressed);
    free(decompressed);
    
    return content_match;
}

// Test file compression/decompression
int test_file_compression_decompression(void) {
    // Create temporary input file
    char *input_path = create_temp_file(test_string_small);
    if (!input_path) return 0;
    
    // Generate output paths
    char *compressed_path = malloc(strlen(input_path) + 10);
    char *decompressed_path = malloc(strlen(input_path) + 20);
    sprintf(compressed_path, "%s.comp", input_path);
    sprintf(decompressed_path, "%s.decomp", input_path);
    
    // Setup compression context
    compression_context_t ctx = {
        .algorithm = ALGO_HUFFMAN,
        .level = LEVEL_NORMAL,
        .progress_callback = NULL,
        .verbose = false,
        .force_overwrite = true,
        .keep_original = true
    };
    
    // Test compression
    int result = compress_file(input_path, compressed_path, &ctx);
    if (result != 0) {
        unlink(input_path);
        free(input_path);
        free(compressed_path);
        free(decompressed_path);
        return 0;
    }
    
    if (!file_exists(compressed_path)) {
        unlink(input_path);
        free(input_path);
        free(compressed_path);
        free(decompressed_path);
        return 0;
    }
    
    // Test decompression
    result = decompress_file(compressed_path, decompressed_path, &ctx);
    if (result != 0) {
        unlink(input_path);
        unlink(compressed_path);
        free(input_path);
        free(compressed_path);
        free(decompressed_path);
        return 0;
    }
    
    if (!file_exists(decompressed_path)) {
        unlink(input_path);
        unlink(compressed_path);
        free(input_path);
        free(compressed_path);
        free(decompressed_path);
        return 0;
    }
    
    // Verify content
    size_t original_size, decompressed_size;
    char *original_content = read_file_contents(input_path, &original_size);
    char *decompressed_content = read_file_contents(decompressed_path, &decompressed_size);
    
    int success = 1;
    if (!original_content || !decompressed_content) {
        success = 0;
    } else if (original_size != decompressed_size) {
        success = 0;
    } else if (memcmp(original_content, decompressed_content, original_size) != 0) {
        success = 0;
    }
    
    // Cleanup
    unlink(input_path);
    unlink(compressed_path);
    unlink(decompressed_path);
    free(input_path);
    free(compressed_path);
    free(decompressed_path);
    if (original_content) free(original_content);
    if (decompressed_content) free(decompressed_content);
    
    return success;
}

// Test utility functions
int test_utility_functions(void) {
    // Test file existence check
    char *temp_path = create_temp_file("test");
    if (!temp_path) return 0;
    
    if (!file_exists(temp_path)) {
        free(temp_path);
        return 0;
    }
    
    unlink(temp_path);
    if (file_exists(temp_path)) {
        free(temp_path);
        return 0;
    }
    
    // Test with NULL path
    if (file_exists(NULL)) {
        free(temp_path);
        return 0;
    }
    
    // Test file size
    temp_path = create_temp_file("Hello, World!");
    if (!temp_path) return 0;
    
    size_t size = get_file_size(temp_path);
    if (size != 13) {
        unlink(temp_path);
        free(temp_path);
        return 0;
    }
    
    unlink(temp_path);
    size = get_file_size(temp_path);
    if (size != 0) {
        free(temp_path);
        return 0;
    }
    
    free(temp_path);
    
    // Test filename generation
    char *output_name = generate_output_filename("test.txt", true);
    if (!output_name) return 0;
    if (!strstr(output_name, ".comp")) {
        free(output_name);
        return 0;
    }
    free(output_name);
    
    output_name = generate_output_filename("test.txt.comp", false);
    if (!output_name) return 0;
    if (strstr(output_name, ".comp")) {
        free(output_name);
        return 0;
    }
    free(output_name);
    
    return 1;
}

// Test error handling
int test_error_handling(void) {
    compression_context_t ctx = {
        .algorithm = ALGO_HUFFMAN,
        .level = LEVEL_NORMAL,
        .progress_callback = NULL,
        .verbose = false,
        .force_overwrite = false,
        .keep_original = true
    };
    
    // Test with non-existent input file
    int result = compress_file("/tmp/nonexistent_file_12345", "/tmp/output.comp", &ctx);
    if (result == 0) return 0;  // Should fail
    
    // Test with NULL parameters
    result = compress_file(NULL, "/tmp/output.comp", &ctx);
    if (result == 0) return 0;  // Should fail
    
    result = compress_file("/tmp/test", NULL, &ctx);
    if (result == 0) return 0;  // Should fail
    
    result = compress_file("/tmp/test", "/tmp/output.comp", NULL);
    if (result == 0) return 0;  // Should fail
    
    // Test Huffman with NULL data
    unsigned char *output = NULL;
    size_t output_size = 0;
    result = huffman_compress(NULL, 10, &output, &output_size);
    if (result == 0) return 0;  // Should fail
    
    // Test bit buffer with NULL
    free_bit_buffer(NULL);  // Should not crash
    write_bits(NULL, 0, 8);  // Should not crash
    uint32_t value = read_bits(NULL, 8);  // Should return 0
    if (value != 0) return 0;
    
    return 1;
}

// Main test runner
int main(void) {
    printf("Running Compressor Test Suite...\n\n");
    
    run_test(test_crc32_calculation, "CRC32 Calculation");
    run_test(test_bit_buffer_operations, "Bit Buffer Operations");
    run_test(test_huffman_compression, "Huffman Compression");
    run_test(test_file_compression_decompression, "File Compression/Decompression");
    run_test(test_utility_functions, "Utility Functions");
    run_test(test_error_handling, "Error Handling");
    
    return test_summary("Compressor");
}