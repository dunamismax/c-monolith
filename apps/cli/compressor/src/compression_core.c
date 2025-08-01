#include "compressor.h"
#include <getopt.h>
#include <signal.h>

int compress_file(const char *input_path, const char *output_path,
                  compression_context_t *ctx) {
  if (!input_path || !output_path || !ctx) {
    print_error("Invalid parameters for compression");
    return -1;
  }

  if (!file_exists(input_path)) {
    print_error("Input file does not exist");
    return -1;
  }

  if (file_exists(output_path) && !ctx->force_overwrite) {
    print_error("Output file exists (use -f to force overwrite)");
    return -1;
  }

  FILE *input_file = fopen(input_path, "rb");
  if (!input_file) {
    print_error("Cannot open input file");
    return -1;
  }

  // Get file size
  fseek(input_file, 0, SEEK_END);
  size_t input_size = ftell(input_file);
  fseek(input_file, 0, SEEK_SET);

  if (input_size == 0) {
    print_error("Input file is empty");
    fclose(input_file);
    return -1;
  }

  // Read input file
  unsigned char *input_data = malloc(input_size);
  if (!input_data) {
    print_error("Memory allocation failed");
    fclose(input_file);
    return -1;
  }

  if (fread(input_data, 1, input_size, input_file) != input_size) {
    print_error("Failed to read input file");
    free(input_data);
    fclose(input_file);
    return -1;
  }
  fclose(input_file);

  // Progress update
  if (ctx->progress_callback) {
    ctx->progress_callback(10.0, "Reading input file");
  }

  // Compress data based on algorithm
  unsigned char *compressed_data = NULL;
  size_t compressed_size = 0;
  int result = -1;

  clock_t start_time = clock();

  switch (ctx->algorithm) {
  case ALGO_HUFFMAN:
    if (ctx->progress_callback) {
      ctx->progress_callback(20.0, "Huffman compression");
    }
    result = huffman_compress(input_data, input_size, &compressed_data,
                              &compressed_size);
    break;

  case ALGO_LZ77:
    if (ctx->progress_callback) {
      ctx->progress_callback(20.0, "LZ77 compression");
    }
    result = lz77_compress(input_data, input_size, &compressed_data,
                           &compressed_size, ctx->level);
    break;

  case ALGO_HYBRID:
    if (ctx->progress_callback) {
      ctx->progress_callback(20.0, "LZ77 + Huffman compression");
    }
    // First LZ77, then Huffman
    unsigned char *lz77_data = NULL;
    size_t lz77_size = 0;
    result = lz77_compress(input_data, input_size, &lz77_data, &lz77_size,
                           ctx->level);
    if (result == 0) {
      if (ctx->progress_callback) {
        ctx->progress_callback(50.0, "Huffman post-processing");
      }
      result = huffman_compress(lz77_data, lz77_size, &compressed_data,
                                &compressed_size);
      free(lz77_data);
    }
    break;

  default:
    print_error("Invalid compression algorithm");
    free(input_data);
    return -1;
  }

  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  if (result != 0 || !compressed_data) {
    print_error("Compression failed");
    free(input_data);
    if (compressed_data)
      free(compressed_data);
    return -1;
  }

  if (ctx->progress_callback) {
    ctx->progress_callback(70.0, "Calculating checksum");
  }

  // Calculate CRC32
  uint32_t crc32 = calculate_crc32(input_data, input_size);

  // Create file header
  file_header_t header;
  header.magic = MAGIC_BYTES;
  header.version = VERSION;
  header.algorithm = ctx->algorithm;
  header.level = ctx->level;
  header.original_size = (uint32_t)input_size;
  header.compressed_size = (uint32_t)compressed_size;
  header.crc32 = crc32;
  header.timestamp = (uint64_t)time(NULL);

  // Extract filename
  const char *filename = strrchr(input_path, '/');
  filename = filename ? filename + 1 : input_path;
  strncpy(header.original_filename, filename, MAX_FILENAME_LEN - 1);
  header.original_filename[MAX_FILENAME_LEN - 1] = '\0';

  if (ctx->progress_callback) {
    ctx->progress_callback(80.0, "Writing output file");
  }

  // Write compressed file
  FILE *output_file = fopen(output_path, "wb");
  if (!output_file) {
    print_error("Cannot create output file");
    free(input_data);
    free(compressed_data);
    return -1;
  }

  // Write header
  if (fwrite(&header, sizeof(file_header_t), 1, output_file) != 1) {
    print_error("Failed to write file header");
    fclose(output_file);
    free(input_data);
    free(compressed_data);
    return -1;
  }

  // Write compressed data
  if (fwrite(compressed_data, 1, compressed_size, output_file) !=
      compressed_size) {
    print_error("Failed to write compressed data");
    fclose(output_file);
    free(input_data);
    free(compressed_data);
    return -1;
  }

  fclose(output_file);

  if (ctx->progress_callback) {
    ctx->progress_callback(100.0, "Compression complete");
  }

  // Print statistics if verbose
  if (ctx->verbose) {
    print_compression_stats(input_size, compressed_size + sizeof(file_header_t),
                            elapsed_time);
  }

  free(input_data);
  free(compressed_data);

  return 0;
}

int decompress_file(const char *input_path, const char *output_path,
                    compression_context_t *ctx) {
  if (!input_path || !output_path || !ctx) {
    print_error("Invalid parameters for decompression");
    return -1;
  }

  if (!file_exists(input_path)) {
    print_error("Input file does not exist");
    return -1;
  }

  if (file_exists(output_path) && !ctx->force_overwrite) {
    print_error("Output file exists (use -f to force overwrite)");
    return -1;
  }

  FILE *input_file = fopen(input_path, "rb");
  if (!input_file) {
    print_error("Cannot open input file");
    return -1;
  }

  // Read and validate header
  file_header_t header;
  if (fread(&header, sizeof(file_header_t), 1, input_file) != 1) {
    print_error("Failed to read file header");
    fclose(input_file);
    return -1;
  }

  if (header.magic != MAGIC_BYTES) {
    print_error("Invalid file format (not a .comp file)");
    fclose(input_file);
    return -1;
  }

  if (header.version > VERSION) {
    print_error("Unsupported file version");
    fclose(input_file);
    return -1;
  }

  if (ctx->progress_callback) {
    ctx->progress_callback(10.0, "Reading compressed data");
  }

  // Read compressed data
  unsigned char *compressed_data = malloc(header.compressed_size);
  if (!compressed_data) {
    print_error("Memory allocation failed");
    fclose(input_file);
    return -1;
  }

  if (fread(compressed_data, 1, header.compressed_size, input_file) !=
      header.compressed_size) {
    print_error("Failed to read compressed data");
    free(compressed_data);
    fclose(input_file);
    return -1;
  }
  fclose(input_file);

  // Decompress data
  unsigned char *decompressed_data = NULL;
  size_t decompressed_size = 0;
  int result = -1;

  clock_t start_time = clock();

  switch (header.algorithm) {
  case ALGO_HUFFMAN:
    if (ctx->progress_callback) {
      ctx->progress_callback(30.0, "Huffman decompression");
    }
    result = huffman_decompress(compressed_data, header.compressed_size,
                                &decompressed_data, &decompressed_size);
    break;

  case ALGO_LZ77:
    if (ctx->progress_callback) {
      ctx->progress_callback(30.0, "LZ77 decompression");
    }
    result = lz77_decompress(compressed_data, header.compressed_size,
                             &decompressed_data, &decompressed_size);
    break;

  case ALGO_HYBRID:
    if (ctx->progress_callback) {
      ctx->progress_callback(30.0, "Huffman decompression");
    }
    // First Huffman, then LZ77
    unsigned char *huffman_data = NULL;
    size_t huffman_size = 0;
    result = huffman_decompress(compressed_data, header.compressed_size,
                                &huffman_data, &huffman_size);
    if (result == 0) {
      if (ctx->progress_callback) {
        ctx->progress_callback(60.0, "LZ77 decompression");
      }
      result = lz77_decompress(huffman_data, huffman_size, &decompressed_data,
                               &decompressed_size);
      free(huffman_data);
    }
    break;

  default:
    print_error("Unsupported compression algorithm");
    free(compressed_data);
    return -1;
  }

  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  if (result != 0 || !decompressed_data) {
    print_error("Decompression failed");
    free(compressed_data);
    if (decompressed_data)
      free(decompressed_data);
    return -1;
  }

  // Verify size
  if (decompressed_size != header.original_size) {
    print_error("Size mismatch after decompression");
    free(compressed_data);
    free(decompressed_data);
    return -1;
  }

  if (ctx->progress_callback) {
    ctx->progress_callback(80.0, "Verifying integrity");
  }

  // Verify CRC32
  uint32_t calculated_crc =
      calculate_crc32(decompressed_data, decompressed_size);
  if (calculated_crc != header.crc32) {
    print_error("File integrity check failed (CRC32 mismatch)");
    free(compressed_data);
    free(decompressed_data);
    return -1;
  }

  if (ctx->progress_callback) {
    ctx->progress_callback(90.0, "Writing output file");
  }

  // Write decompressed file
  FILE *output_file = fopen(output_path, "wb");
  if (!output_file) {
    print_error("Cannot create output file");
    free(compressed_data);
    free(decompressed_data);
    return -1;
  }

  if (fwrite(decompressed_data, 1, decompressed_size, output_file) !=
      decompressed_size) {
    print_error("Failed to write decompressed data");
    fclose(output_file);
    free(compressed_data);
    free(decompressed_data);
    return -1;
  }
  fclose(output_file);

  if (ctx->progress_callback) {
    ctx->progress_callback(100.0, "Decompression complete");
  }

  // Print statistics if verbose
  if (ctx->verbose) {
    printf("\nDecompression successful!\n");
    printf("Original file: %s\n", header.original_filename);
    printf("File size: %u bytes\n", header.original_size);
    printf("Processing time: %.3f seconds\n", elapsed_time);

    // Print algorithm info
    const char *algo_name = "Unknown";
    switch (header.algorithm) {
    case ALGO_HUFFMAN:
      algo_name = "Huffman";
      break;
    case ALGO_LZ77:
      algo_name = "LZ77";
      break;
    case ALGO_HYBRID:
      algo_name = "Hybrid (LZ77+Huffman)";
      break;
    }
    printf("Algorithm: %s (level %u)\n", algo_name, header.level);

    // Convert timestamp
    time_t timestamp = (time_t)header.timestamp;
    printf("Compressed: %s", ctime(&timestamp));
  }

  free(compressed_data);
  free(decompressed_data);

  return 0;
}