# CLI Applications

Command-line utilities and tools for everyday system tasks and calculations.

## Application Types

This directory contains command-line interface applications that provide useful utilities for:

- **Calculators**: Scientific and mathematical computation tools
- **File Utilities**: Secure file operations, manipulation, and analysis tools
- **Text Processors**: String manipulation, text parsing, and document processing
- **Compression Tools**: File compression and decompression with multiple algorithms
- **System Tools**: Command-line utilities for system administration and automation
- **Data Converters**: Format conversion and data transformation utilities
- **Configuration Tools**: System configuration and setup utilities

## Characteristics

- Interactive command-line interfaces
- Robust input validation and error handling
- Help systems and usage documentation
- Secure file operations with path validation
- Cross-platform compatibility
- Zero external dependencies

## Featured Applications

### Compressor
**Modern file compression tool with multiple algorithms**

```bash
make run-compressor
```

**Features:**
- **Multiple Algorithms**: Huffman coding, LZ77, and Hybrid (LZ77+Huffman)
- **Compression Levels**: 9 levels from fastest (1) to best compression (9)
- **File Integrity**: CRC32 checksums for data verification
- **Progress Reporting**: Real-time progress bars and compression statistics
- **Format Support**: Custom .comp format optimized for efficiency
- **Security**: Memory-safe implementation with comprehensive error handling

**Usage Examples:**
```bash
# Compress with Huffman coding (default)
./build/release/bin/compressor file.txt

# Best LZ77 compression with verbose output
./build/release/bin/compressor -a lz77 -l 9 -v file.txt

# Decompress file
./build/release/bin/compressor -d file.txt.comp

# Test file integrity
./build/release/bin/compressor -t file.txt.comp

# Show compressed file information
./build/release/bin/compressor -i file.txt.comp
```

**Technical Implementation:**
- **Huffman Coding**: Optimal prefix-free coding with frequency analysis
- **LZ77 Algorithm**: Sliding window compression with hash table optimization
- **Hybrid Mode**: LZ77 preprocessing followed by Huffman encoding
- **Bit-level Operations**: Efficient bit manipulation for minimal overhead
- **Memory Management**: Safe allocation with automatic cleanup
- **Cross-platform**: Works on macOS, Linux (ARM64 and x86_64)

**Performance:**
- ARM64 Apple Silicon optimized
- Multi-threaded capable architecture
- Memory-efficient sliding window implementation
- Optimized hash functions for pattern matching

### Calculator
**Scientific calculator with advanced mathematical operations**

```bash
make run-calculator
```

### Text Processor
**String manipulation and text processing utilities**

```bash
make run-text_processor
```

## Development Focus

- User-friendly CLI interfaces with clear prompts
- Comprehensive error handling and validation
- Memory-safe string operations
- Professional command-line argument parsing
- Consistent exit codes and status reporting
- Advanced algorithm implementations with educational value
