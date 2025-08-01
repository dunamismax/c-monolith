#include "compressor.h"
#include <getopt.h>
#include <signal.h>

static volatile bool interrupted = false;

static void signal_handler(int sig) {
  (void)sig;
  interrupted = true;
  printf("\nOperation interrupted by user.\n");
}

static void progress_callback(double percentage, const char *status) {
  if (interrupted)
    return;
  print_progress_bar(percentage, status);
}

static int show_file_info(const char *input_path) {
  if (!file_exists(input_path)) {
    print_error("File does not exist");
    return -1;
  }

  FILE *input_file = fopen(input_path, "rb");
  if (!input_file) {
    print_error("Cannot open file");
    return -1;
  }

  file_header_t header;
  if (fread(&header, sizeof(file_header_t), 1, input_file) != 1) {
    print_error("Failed to read file header");
    fclose(input_file);
    return -1;
  }
  fclose(input_file);

  if (header.magic != MAGIC_BYTES) {
    print_error("Not a valid .comp file");
    return -1;
  }

  // Display file information
  printf("Compressed File Information:\n");
  printf("  File format version: %u\n", header.version);
  printf("  Original filename:   %s\n", header.original_filename);
  printf("  Original size:       %u bytes\n", header.original_size);
  printf("  Compressed size:     %u bytes\n", header.compressed_size);
  printf("  Total file size:     %zu bytes\n",
         header.compressed_size + sizeof(file_header_t));

  double ratio = (double)header.compressed_size / header.original_size;
  printf("  Compression ratio:   %.2f:1\n", 1.0 / ratio);
  printf("  Space savings:       %.1f%%\n", (1.0 - ratio) * 100.0);

  const char *algo_name = "Unknown";
  switch (header.algorithm) {
  case ALGO_HUFFMAN:
    algo_name = "Huffman coding";
    break;
  case ALGO_LZ77:
    algo_name = "LZ77";
    break;
  case ALGO_HYBRID:
    algo_name = "Hybrid (LZ77+Huffman)";
    break;
  }
  printf("  Algorithm:           %s\n", algo_name);
  printf("  Compression level:   %u\n", header.level);
  printf("  CRC32 checksum:      0x%08X\n", header.crc32);

  time_t timestamp = (time_t)header.timestamp;
  printf("  Creation time:       %s", ctime(&timestamp));

  return 0;
}

static int test_file_integrity(const char *input_path) {
  printf("Testing file integrity: %s\n", input_path);

  compression_context_t ctx = {
      .algorithm = ALGO_HUFFMAN, // Will be overridden by file header
      .level = LEVEL_NORMAL,
      .progress_callback = progress_callback,
      .verbose = false,
      .force_overwrite = true,
      .keep_original = true};

  // Create temporary output path
  char temp_path[] = "/tmp/compressor_test_XXXXXX";
  int temp_fd = mkstemp(temp_path);
  if (temp_fd == -1) {
    print_error("Cannot create temporary file");
    return -1;
  }
  close(temp_fd);

  // Try to decompress
  int result = decompress_file(input_path, temp_path, &ctx);

  // Clean up
  unlink(temp_path);

  if (result == 0) {
    printf("✓ File integrity test passed\n");
  } else {
    printf("✗ File integrity test failed\n");
  }

  return result;
}

int main(int argc, char *argv[]) {
  // Install signal handler
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  // Default options
  bool compress = true;
  compression_context_t ctx = {.algorithm = ALGO_HUFFMAN,
                               .level = LEVEL_NORMAL,
                               .progress_callback = NULL,
                               .verbose = false,
                               .force_overwrite = false,
                               .keep_original = false};

  bool show_info = false;
  bool test_integrity = false;
  char *input_path = NULL;
  char *output_path = NULL;

  // Parse command line options
  static struct option long_options[] = {
      {"compress", no_argument, 0, 'c'},
      {"decompress", no_argument, 0, 'd'},
      {"algorithm", required_argument, 0, 'a'},
      {"level", required_argument, 0, 'l'},
      {"verbose", no_argument, 0, 'v'},
      {"force", no_argument, 0, 'f'},
      {"keep", no_argument, 0, 'k'},
      {"test", no_argument, 0, 't'},
      {"info", no_argument, 0, 'i'},
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, 'V'},
      {0, 0, 0, 0}};

  int c;
  while ((c = getopt_long(argc, argv, "cda:l:vfktihV", long_options, NULL)) !=
         -1) {
    switch (c) {
    case 'c':
      compress = true;
      break;
    case 'd':
      compress = false;
      break;
    case 'a':
      if (strcmp(optarg, "huffman") == 0) {
        ctx.algorithm = ALGO_HUFFMAN;
      } else if (strcmp(optarg, "lz77") == 0) {
        ctx.algorithm = ALGO_LZ77;
      } else if (strcmp(optarg, "hybrid") == 0) {
        ctx.algorithm = ALGO_HYBRID;
      } else {
        print_error("Invalid algorithm. Use: huffman, lz77, or hybrid");
        return 1;
      }
      break;
    case 'l':
      ctx.level = atoi(optarg);
      if (ctx.level < 1 || ctx.level > 9) {
        print_error("Compression level must be between 1 and 9");
        return 1;
      }
      break;
    case 'v':
      ctx.verbose = true;
      ctx.progress_callback = progress_callback;
      break;
    case 'f':
      ctx.force_overwrite = true;
      break;
    case 'k':
      ctx.keep_original = true;
      break;
    case 't':
      test_integrity = true;
      break;
    case 'i':
      show_info = true;
      break;
    case 'h':
      print_usage(argv[0]);
      return 0;
    case 'V':
      printf("Compressor v%d - Modern C Compression Tool\n", VERSION);
      printf("Algorithms: Huffman coding, LZ77, Hybrid (LZ77+Huffman)\n");
      printf("Built with ARM64 optimizations and memory safety\n");
      return 0;
    case '?':
      return 1;
    default:
      print_usage(argv[0]);
      return 1;
    }
  }

  // Get input file
  if (optind >= argc) {
    print_error("Input file required");
    print_usage(argv[0]);
    return 1;
  }
  input_path = argv[optind];

  // Handle special modes
  if (show_info) {
    return show_file_info(input_path) == 0 ? 0 : 1;
  }

  if (test_integrity) {
    return test_file_integrity(input_path) == 0 ? 0 : 1;
  }

  // Get output file (optional)
  if (optind + 1 < argc) {
    output_path = argv[optind + 1];
  } else {
    output_path = generate_output_filename(input_path, compress);
    if (!output_path) {
      print_error("Cannot generate output filename");
      return 1;
    }
  }

  // Perform compression or decompression
  int result;
  if (compress) {
    if (ctx.verbose) {
      printf("Compressing: %s -> %s\n", input_path, output_path);
      const char *algo_names[] = {"", "Huffman", "LZ77", "Hybrid"};
      printf("Algorithm: %s (level %d)\n", algo_names[ctx.algorithm],
             ctx.level);
    }
    result = compress_file(input_path, output_path, &ctx);
  } else {
    if (ctx.verbose) {
      printf("Decompressing: %s -> %s\n", input_path, output_path);
    }
    result = decompress_file(input_path, output_path, &ctx);
  }

  // Clean up
  if (optind + 1 >= argc) {
    free(output_path); // Only free if we generated it
  }

  // Remove original file if requested and operation succeeded
  if (result == 0 && !ctx.keep_original) {
    if (unlink(input_path) != 0) {
      fprintf(stderr, "Warning: Could not remove original file\n");
    }
  }

  return result == 0 ? 0 : 1;
}