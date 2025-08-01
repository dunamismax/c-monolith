#include "compressor.h"

// CRC32 lookup table
static uint32_t crc32_table[256];
static bool crc32_table_initialized = false;

static void init_crc32_table(void) {
    if (crc32_table_initialized) return;
    
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320UL;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
    crc32_table_initialized = true;
}

uint32_t calculate_crc32(const unsigned char *data, size_t length) {
    if (!data) return 0;
    
    init_crc32_table();
    
    uint32_t crc = 0xFFFFFFFFUL;
    for (size_t i = 0; i < length; i++) {
        crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    
    return crc ^ 0xFFFFFFFFUL;
}

void print_progress_bar(double percentage, const char *status) {
    const int bar_width = 50;
    int filled = (int)(percentage * bar_width / 100.0);
    
    printf("\r%s [", status);
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf("█");
        } else {
            printf("░");
        }
    }
    printf("] %.1f%%", percentage);
    fflush(stdout);
    
    if (percentage >= 100.0) {
        printf("\n");
    }
}

void print_compression_stats(size_t original_size, size_t compressed_size,
                           double elapsed_time) {
    double ratio = (double)compressed_size / original_size;
    double savings = (1.0 - ratio) * 100.0;
    double speed_mbps = (original_size / (1024.0 * 1024.0)) / elapsed_time;
    
    printf("\n");
    printf("Compression Statistics:\n");
    printf("  Original size:    %zu bytes\n", original_size);
    printf("  Compressed size:  %zu bytes\n", compressed_size);
    printf("  Compression ratio: %.2f:1\n", 1.0 / ratio);
    printf("  Space savings:    %.1f%%\n", savings);
    printf("  Processing speed: %.2f MB/s\n", speed_mbps);
    printf("  Time elapsed:     %.3f seconds\n", elapsed_time);
}

bool file_exists(const char *path) {
    if (!path) return false;
    return access(path, F_OK) == 0;
}

size_t get_file_size(const char *path) {
    if (!path) return 0;
    
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_size;
    }
    return 0;
}

char *generate_output_filename(const char *input_path, bool compress) {
    if (!input_path) return NULL;
    
    size_t input_len = strlen(input_path);
    char *output_path = NULL;
    
    if (compress) {
        // Add .comp extension
        output_path = malloc(input_len + 6);  // ".comp" + null terminator
        if (output_path) {
            strcpy(output_path, input_path);
            strcat(output_path, ".comp");
        }
    } else {
        // Remove .comp extension if present
        if (input_len > 5 && strcmp(input_path + input_len - 5, ".comp") == 0) {
            output_path = malloc(input_len - 4);  // Remove ".comp"
            if (output_path) {
                strncpy(output_path, input_path, input_len - 5);
                output_path[input_len - 5] = '\0';
            }
        } else {
            // Add .decompressed extension
            output_path = malloc(input_len + 14);  // ".decompressed" + null terminator
            if (output_path) {
                strcpy(output_path, input_path);
                strcat(output_path, ".decompressed");
            }
        }
    }
    
    return output_path;
}

void print_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

void print_usage(const char *program_name) {
    printf("Usage: %s [OPTIONS] INPUT_FILE [OUTPUT_FILE]\n\n", program_name);
    printf("A modern C compression tool with multiple algorithms\n\n");
    printf("Options:\n");
    printf("  -c, --compress     Compress the input file (default)\n");
    printf("  -d, --decompress   Decompress the input file\n");
    printf("  -a, --algorithm    Compression algorithm:\n");
    printf("                       huffman  - Huffman coding (default)\n");
    printf("                       lz77     - LZ77 compression\n");
    printf("                       hybrid   - LZ77 + Huffman\n");
    printf("  -l, --level        Compression level (1-9, default: 5)\n");
    printf("                       1 = fastest, 9 = best compression\n");
    printf("  -v, --verbose      Verbose output with statistics\n");
    printf("  -f, --force        Force overwrite output file\n");
    printf("  -k, --keep         Keep original file after compression/decompression\n");
    printf("  -t, --test         Test compressed file integrity\n");
    printf("  -i, --info         Display file information\n");
    printf("  -h, --help         Display this help message\n");
    printf("      --version      Display version information\n\n");
    printf("Examples:\n");
    printf("  %s file.txt                    # Compress using Huffman\n", program_name);
    printf("  %s -a lz77 -l 9 file.txt      # Best LZ77 compression\n", program_name);
    printf("  %s -d file.txt.comp           # Decompress file\n", program_name);
    printf("  %s -i file.txt.comp           # Show file info\n", program_name);
    printf("  %s -t file.txt.comp           # Test file integrity\n", program_name);
    printf("\nSupported file formats: All binary and text files\n");
    printf("Output format: Custom .comp format with integrity checking\n");
}

// Bit buffer functions
bit_buffer_t *create_bit_buffer(size_t initial_size) {
    bit_buffer_t *buffer = malloc(sizeof(bit_buffer_t));
    if (!buffer) return NULL;
    
    buffer->buffer = malloc(initial_size);
    if (!buffer->buffer) {
        free(buffer);
        return NULL;
    }
    
    buffer->buffer_size = initial_size;
    buffer->byte_pos = 0;
    buffer->bit_pos = 0;
    memset(buffer->buffer, 0, initial_size);
    
    return buffer;
}

void write_bits(bit_buffer_t *buffer, uint32_t value, int num_bits) {
    if (!buffer || num_bits <= 0 || num_bits > 32) return;
    
    for (int i = num_bits - 1; i >= 0; i--) {
        // Ensure we have space
        if (buffer->byte_pos >= buffer->buffer_size) {
            size_t new_size = buffer->buffer_size * 2;
            unsigned char *new_buffer = realloc(buffer->buffer, new_size);
            if (!new_buffer) return;
            
            memset(new_buffer + buffer->buffer_size, 0, buffer->buffer_size);
            buffer->buffer = new_buffer;
            buffer->buffer_size = new_size;
        }
        
        // Extract bit
        int bit = (value >> i) & 1;
        
        // Set bit in buffer
        if (bit) {
            buffer->buffer[buffer->byte_pos] |= (1 << (7 - buffer->bit_pos));
        }
        
        buffer->bit_pos++;
        if (buffer->bit_pos == 8) {
            buffer->bit_pos = 0;
            buffer->byte_pos++;
        }
    }
}

uint32_t read_bits(bit_buffer_t *buffer, int num_bits) {
    if (!buffer || num_bits <= 0 || num_bits > 32) return 0;
    
    uint32_t result = 0;
    
    for (int i = 0; i < num_bits; i++) {
        if (buffer->byte_pos >= buffer->buffer_size) {
            break;  // End of buffer
        }
        
        // Get bit
        int bit = (buffer->buffer[buffer->byte_pos] >> (7 - buffer->bit_pos)) & 1;
        result = (result << 1) | bit;
        
        buffer->bit_pos++;
        if (buffer->bit_pos == 8) {
            buffer->bit_pos = 0;
            buffer->byte_pos++;
        }
    }
    
    return result;
}

void free_bit_buffer(bit_buffer_t *buffer) {
    if (!buffer) return;
    
    free(buffer->buffer);
    free(buffer);
}