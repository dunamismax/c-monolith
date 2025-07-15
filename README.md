<p align="center">
  <img src="c.png" alt="C Monorepo Logo" width="300" />
</p>

<p align="center">
  <a href="https://github.com/dunamismax/c-monorepo">
    <img src="https://readme-typing-svg.demolab.com/?font=Fira+Code&size=24&pause=1000&color=3071A4&center=true&vCenter=true&width=800&lines=C+Programming+Monorepo;ARM64+Optimized+Build+System;Educational+%2B+Production+Ready;High-Performance+C+Development;From+CLI+Tools+to+Games." alt="Typing SVG" />
  </a>
</p>

<p align="center">
  <a href="https://clang.llvm.org/"><img src="https://img.shields.io/badge/Clang-15+-blue.svg?logo=llvm" alt="Clang Version"></a>
  <a href="https://developer.apple.com/documentation/apple-silicon"><img src="https://img.shields.io/badge/ARM64-Apple_Silicon-black.svg?logo=apple" alt="ARM64 Apple Silicon"></a>
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-green.svg" alt="MIT License"></a>
  <a href="https://github.com/dunamismax/c-monorepo/pulls"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg" alt="PRs Welcome"></a>
  <a href="https://github.com/dunamismax/c-monorepo/stargazers"><img src="https://img.shields.io/github/stars/dunamismax/c-monorepo" alt="GitHub Stars"></a>
</p>

---

## About This Project

A **production-ready**, security-hardened monorepo for pure C development optimized for ARM64 architecture and Apple Silicon. This repository demonstrates enterprise-grade C programming with comprehensive security measures, extensive testing, and deployment-ready applications.

**Key Features:**

- **🔒 Security Hardened**: Protection against buffer overflows, path traversal, format string attacks, and integer overflows
- **🚀 ARM64 Optimized**: Built specifically for Apple Silicon (M1/M2/M3/M4) with aggressive compiler optimizations
- **🛡️ Production Ready**: Memory-safe code with comprehensive input validation, error handling, and security controls
- **🏗️ Professional Build System**: Advanced Makefile with 65+ targets orchestrating parallel builds, dependency management, and complete development workflows
- **🧪 Comprehensive Testing**: Unit tests, integration tests, security tests, and static analysis
- **📚 Educational Excellence**: Working demo applications showcasing modern C programming techniques and security best practices
- **🔧 Developer Tools**: Comprehensive scripts for building, testing, formatting, deployment automation, and security scanning
- **📖 Enterprise Documentation**: Complete security guidelines, deployment guides, and maintenance procedures

---

<details>
<summary><strong>Use This Template</strong></summary>

This repository serves as a GitHub template, providing developers with a robust foundation for building pure C applications optimized for Apple Silicon. Rather than cloning, you can create your own repository instance with all essential infrastructure and demo applications pre-configured.

**To get started:**

1. Click the green **"Use this template"** button at the top right of this repository
2. Choose "Create a new repository"
3. Name your repository and set it to public or private
4. Click "Create repository from template"

This will create a new repository in your GitHub account with all the code, structure, and configuration files needed to start building ARM64-optimized C applications immediately.

**Advantages of using the template:**

- Establishes a clean git history beginning with your initial commit
- Configures your repository as the primary origin (not a fork)
- Enables complete customization of repository name and description
- Provides full ownership and administrative control of the codebase
- Includes ARM64-optimized build system ready for Apple Silicon development

</details>

---

## Quick Start

### Prerequisites

**macOS (Primary Platform):**

```bash
# Install Xcode Command Line Tools (includes clang)
xcode-select --install

# Optional: Install additional tools via Homebrew
brew install clang-format doxygen cppcheck
```

**Other Platforms:**

```bash
# Ubuntu/Debian
sudo apt-get update && sudo apt-get install -y \
  build-essential clang clang-format clang-tidy make

# CentOS/RHEL
sudo yum install -y clang make clang-tools-extra
```

### Setup & Build

```bash
# Clone the repository
git clone https://github.com/dunamismax/c-monorepo.git
cd c-monorepo

# Check system information and dependencies
make sysinfo         # Display system and compiler information
make deps            # Verify all dependencies

# Build everything with ARM64 optimization
make                 # Build all components (debug mode)
make MODE=release    # Optimized release build
make MODE=profile    # Profile build with coverage

# Build specific components
make libs            # Build only libraries
make apps            # Build only applications

# Run applications
make run-calculator
make run-tic_tac_toe
make run-text_processor

# Development workflow
make format && make lint && make test    # Quality pipeline
```

---

## The Makefile: Professional ARM64-Optimized Build System

This repository features a **professional-grade Makefile** with **65+ targets** covering every aspect of ARM64-optimized development from initial setup to production deployment. It provides intelligent dependency detection, parallel builds optimized for Apple Silicon, and beautiful color-coded output.

### Core Build Commands

<details>
<summary><strong>Primary Build Targets</strong></summary>

```bash
# Core Build Operations
make                 # Default: build all components in debug mode
make all            # Build everything (equivalent to default)
make MODE=release   # Optimized ARM64 release build with LTO
make MODE=profile   # Profile build with coverage analysis
make quick          # Fast incremental build

# Component Building
make libs           # Build all shared libraries
make apps           # Build all applications  
make tests          # Build and run comprehensive test suite

# Individual Component Targets
make data_structures    # Build data structures library
make math_utils        # Build math utilities library
make calculator        # Build calculator application
make file_utils        # Build file utilities
make text_processor    # Build text processor
make tic_tac_toe      # Build tic-tac-toe game
make number_guessing  # Build number guessing game
```

</details>

<details>
<summary><strong>Quality Assurance & Analysis</strong></summary>

```bash
# Code Quality
make format         # Format code with clang-format
make format-check   # Verify code formatting (CI-safe)
make lint           # Static analysis with clang-tidy
make security       # Security vulnerability scanning

# Testing & Validation
make test           # Run comprehensive test suite
make coverage       # Generate code coverage reports
make benchmark      # Performance benchmarking
make profile        # Build with profiling enabled

# Development Workflow
make banner         # Display project information
make sysinfo        # Show detailed system information
make deps           # Check all dependencies
```

</details>

<details>
<summary><strong>Application Execution</strong></summary>

```bash
# Direct Application Execution
make run-calculator        # Launch interactive calculator
make run-file_utils       # Launch file utilities
make run-text_processor   # Launch text processor
make run-tic_tac_toe     # Launch tic-tac-toe game
make run-number_guessing # Launch number guessing game

# Development Testing
make calculator test-run  # Run calculator with test input
make file_utils test-run # Run file utils with sample commands
```

</details>

<details>
<summary><strong>Installation & Cleanup</strong></summary>

```bash
# Installation
make install        # Install to /usr/local (requires sudo)
make install-local  # Install to ~/local

# Cleanup Operations
make clean          # Clean build artifacts
make distclean      # Deep clean (all generated files)

# Documentation
make docs           # Generate API documentation
make docs-open      # Generate and open documentation
make help           # Comprehensive help system
```

</details>

### ARM64 Optimization Features

- **Apple Silicon Specific**: `-mcpu=apple-m1 -mtune=apple-m1 -arch arm64`
- **Link-Time Optimization**: `-flto` for maximum performance in release builds
- **Parallel Builds**: Automatically uses all CPU cores (`sysctl -n hw.ncpu`)
- **Security Hardening**: Stack protection and fortified source compilation
- **Memory Sanitization**: AddressSanitizer and UndefinedBehaviorSanitizer in debug builds

### Advanced Build Options

```bash
# Parallel Compilation (automatic on macOS)
make -j$(sysctl -n hw.ncpu)

# Verbose Output for Debugging
make VERBOSE=1

# Compiler Selection
make CC=clang          # Use clang (default)
make CC=gcc            # Use GCC if available

# Custom Installation Prefix
make install PREFIX=/opt/local
```

---

## Project Architecture

### Repository Structure

```
c-monorepo/
├── apps/                      # Application implementations
│   ├── cli/                   # Command-line applications
│   │   ├── calculator/        # Scientific calculator with math utilities
│   │   ├── file_utils/        # File operations and analysis
│   │   └── text_processor/    # Advanced text manipulation
│   └── games/                 # Interactive games
│       ├── tic_tac_toe/      # AI-powered tic-tac-toe
│       └── number_guessing/  # Multi-difficulty guessing game
├── libs/                      # Shared libraries
│   ├── data_structures/       # Dynamic data structures (Vector)
│   └── math_utils/           # Mathematical algorithms
├── build/                     # Build output (debug/release/profile)
│   ├── debug/bin/            # Debug executables
│   ├── release/bin/          # Optimized release executables
│   └── profile/bin/          # Profile executables
├── scripts/                   # Development automation
│   ├── build.sh              # Enhanced build script
│   ├── test.sh              # Comprehensive test runner
│   └── clean.sh             # Cleanup utility
├── tests/                     # Test suite
│   ├── libs/                 # Library unit tests
│   └── apps/                 # Application tests
├── docs/                      # Documentation
│   ├── SECURITY.md           # Security guidelines and best practices
│   ├── DEPLOYMENT.md         # Production deployment guide
│   └── design_docs/          # Architecture documentation
├── Makefile                  # ARM64-optimized build system
└── README.md                 # This file
```

### Technology Stack

**Core Development:**

- **C11 Standard**: Modern C with ARM64-specific optimizations
- **Clang Compiler**: Apple's optimized compiler for Apple Silicon
- **Make**: Advanced build orchestration with parallel execution
- **Static Analysis**: clang-tidy integration for code quality

**Development Tools:**

- **clang-format**: Consistent code formatting
- **AddressSanitizer**: Memory error detection in debug builds
- **Link-Time Optimization**: Maximum performance in release builds
- **Code Coverage**: Comprehensive test coverage analysis

---

## Demo Applications

### Calculator (`calculator`)

Advanced command-line calculator demonstrating mathematical operations and library integration.

```bash
# Build and run
make run-calculator

# Example session
calc> + 5 3        # Addition: Result: 8.00
calc> fact 5       # Factorial: Result: 120
calc> sqrt 16      # Square root: Result: 4.000000
calc> prime 17     # Prime check: Result: 17 is prime
calc> gcd 48 18    # Greatest common divisor: Result: 6
calc> help         # Show all available commands
calc> quit         # Exit calculator
```

**Features:**
- Basic arithmetic operations with ARM64-optimized math library
- Advanced mathematical functions (factorial, Fibonacci, GCD, LCM)
- Prime number checking with optimized algorithms
- Interactive help system and command validation
- Memory-safe input handling

### File Utils (`file_utils`)

Comprehensive file and directory manipulation utility showcasing system programming.

```bash
# Build and run
make run-file_utils

# Example commands
fileutils> info README.md        # Detailed file information
fileutils> count myfile.txt      # Count lines, words, characters
fileutils> list /usr/local       # Directory listing with sizes
fileutils> copy src.txt dst.txt  # Safe file copying
fileutils> help                  # Command reference
```

**Features:**
- File metadata analysis (size, permissions, timestamps)
- Text content analysis with word/line counting
- Directory traversal and listing
- Safe file operations with error handling
- Cross-platform compatibility

### Text Processor (`text_processor`)

Advanced text manipulation tool demonstrating string algorithms and data structures.

```bash
# Build and run
make run-text_processor

# Example operations
textproc> upper Hello World           # Case conversion
textproc> reverse programming         # String reversal
textproc> count a banana banana       # Character frequency
textproc> replace old new "This is old text"  # Find and replace
textproc> trim "  extra   spaces  "   # Whitespace normalization
textproc> sort                        # Interactive line sorting
```

**Features:**
- Case conversion and string manipulation
- Pattern matching and replacement
- Interactive line sorting with multiple algorithms
- Character frequency analysis
- Memory-efficient string processing

### Tic-Tac-Toe Game (`tic_tac_toe`)

Interactive game with intelligent AI opponent demonstrating game logic and algorithms.

```bash
# Build and run
make run-tic_tac_toe

# Game features:
# 1. Play vs AI (multiple difficulty levels)
# 2. Two Player Mode (local multiplayer)
# 3. Intelligent move validation
```

**Features:**
- Smart AI with multiple strategies (offensive, defensive, strategic)
- Clean game state visualization
- Robust input validation and error handling
- Win/lose/tie detection with game statistics
- Two-player local mode

### Number Guessing Game (`number_guessing`)

Multi-difficulty guessing game showcasing random number generation and game mechanics.

```bash
# Build and run
make run-number_guessing

# Difficulty levels:
# - Easy: 1-50, 10 attempts
# - Medium: 1-100, 8 attempts
# - Hard: 1-200, 6 attempts
# - Custom: User-defined parameters
```

**Features:**
- Multiple difficulty levels with adaptive scoring
- Intelligent hint system (proximity feedback)
- Statistics tracking and performance analysis
- Custom game configuration
- Educational probability concepts

---

## Shared Libraries

### Data Structures Library (`libdatastructures.a`)

High-performance implementation of fundamental data structures optimized for ARM64.

```c
#include "vector.h"

// Create and manipulate dynamic arrays
Vector *vec = vector_create(16);
vector_push(vec, 42);
vector_push(vec, 24);

int value;
vector_get(vec, 0, &value);  // Retrieve elements
vector_set(vec, 0, 100);     // Modify elements
vector_pop(vec, &value);     // Remove elements

vector_print(vec);           // Debug output
vector_destroy(vec);         // Clean memory
```

**Performance Features:**
- ARM64-optimized memory allocation patterns
- Intelligent resizing with configurable growth factors
- Memory-efficient shrinking to prevent waste
- Comprehensive bounds checking and error handling

### Math Utils Library (`libmathutils.a`)

Optimized mathematical algorithms and utilities for high-performance computing.

```c
#include "math_utils.h"

// Number theory operations
int gcd = math_gcd(48, 18);           // Euclidean algorithm
int lcm = math_lcm(4, 6);             // Least common multiple
int is_prime = math_is_prime(17);     // Optimized primality test

// Sequence generation
long long fact = math_factorial(5);    // 5! = 120
long long fib = math_fibonacci(10);    // 10th Fibonacci number

// ARM64-optimized operations
long long power = math_power(2, 10);   // Fast exponentiation
double sqrt_val = math_sqrt(16.0);     // Newton's method

// Utility functions
int minimum = math_min(a, b);
int maximum = math_max(a, b);
double absolute = math_abs(-5.5);
```

**ARM64 Optimizations:**
- Vectorized operations where applicable
- Branch prediction optimizations
- Cache-friendly algorithm implementations
- Fast math compiler optimizations enabled

---

## Development Workflow

### Code Quality Pipeline

```bash
# Complete quality assurance workflow
make format          # Standardize code formatting
make lint           # Static analysis with clang-tidy
make security       # Security vulnerability scanning
make test           # Comprehensive test execution
make coverage       # Generate coverage reports
```

### Performance Analysis

```bash
# Performance profiling and optimization
make profile        # Build with profiling enabled
make benchmark      # Run performance benchmarks
make MODE=release   # Maximum optimization build
```

### Adding New Components

<details>
<summary><strong>Creating New Applications</strong></summary>

```bash
# Create new CLI application
mkdir -p apps/cli/my_new_app/src
cp apps/cli/calculator/Makefile apps/cli/my_new_app/
# Edit Makefile to change target name
# Implement your application in src/

# Create new game
mkdir -p apps/games/my_new_game/src
cp apps/games/tic_tac_toe/Makefile apps/games/my_new_game/
# Edit Makefile and implement game logic

# Build and test
make my_new_app
make run-my_new_app
```

</details>

<details>
<summary><strong>Creating New Libraries</strong></summary>

```bash
# Create new library
mkdir -p libs/my_new_lib/{src,include}
cp libs/math_utils/Makefile libs/my_new_lib/
# Edit Makefile for new library name

# Implement header files in include/
# Implement source files in src/

# Build and use in applications
make my_new_lib
# Link against -lmy_new_lib in application Makefiles
```

</details>

### Code Style Guidelines

- **Naming Convention**: Use snake_case for functions and variables
- **Documentation**: Doxygen-style comments for all public APIs
- **Error Handling**: Always check return values and handle errors gracefully
- **Memory Management**: Follow RAII principles - every malloc needs a corresponding free
- **ARM64 Optimization**: Leverage compiler intrinsics where beneficial
- **Security**: Use safe string functions and validate all inputs

---

## Testing & Quality Assurance

### Comprehensive Testing Suite

```bash
# Run all tests
make test

# Specific test categories
make test MODE=debug     # Debug build testing
make test MODE=release   # Release build validation
make coverage           # Code coverage analysis
make security          # Security scanning
```

### Static Analysis Integration

The build system includes comprehensive static analysis:

- **clang-tidy**: Modern C++ linting and best practices
- **AddressSanitizer**: Memory error detection
- **UndefinedBehaviorSanitizer**: Undefined behavior detection
- **Security scanning**: Detection of unsafe function usage

### Performance Benchmarking

```bash
# Application-specific benchmarks
make calculator benchmark    # Mathematical operation performance
make vector benchmark       # Data structure performance
make benchmark              # System-wide performance analysis
```

---

## Security Features

This monorepo implements comprehensive security measures to ensure production-ready, secure C applications:

### 🛡️ **Memory Safety**
- **Buffer overflow protection** with bounds checking and dynamic allocation
- **Safe string operations** using custom functions with size validation
- **Integer overflow detection** in mathematical operations
- **Memory leak prevention** with comprehensive cleanup

### 🔒 **Input Validation**
- **Path traversal prevention** with comprehensive path validation
- **Format string attack protection** using safe parsing techniques
- **Input sanitization** for all user-provided data
- **Length validation** to prevent buffer overflows

### 🚨 **Security Testing**
- **Integration security tests** for vulnerability validation
- **Static analysis** with clang-tidy and security flags
- **Dynamic analysis** with AddressSanitizer and UndefinedBehaviorSanitizer
- **Comprehensive test coverage** including edge cases

### 📋 **Secure Development**
- **Compiler hardening flags** (`-fstack-protector-strong`, `-D_FORTIFY_SOURCE=2`)
- **Shell script hardening** with strict error handling
- **Security documentation** and deployment guidelines
- **Regular security audits** and vulnerability assessments

For detailed security information, see [`docs/SECURITY.md`](docs/SECURITY.md).

---

## Production Deployment

This repository is production-ready with comprehensive deployment documentation:

- **Security hardening** guidelines and checklists
- **Performance optimization** and tuning recommendations
- **Monitoring and logging** configuration
- **Container deployment** examples and best practices
- **Maintenance procedures** and update protocols

For complete deployment instructions, see [`docs/DEPLOYMENT.md`](docs/DEPLOYMENT.md).

---

## Contributing

### Development Setup

```bash
git clone https://github.com/dunamismax/c-monorepo.git
cd c-monorepo
make deps           # Verify dependencies
make sysinfo        # Check system compatibility
make format && make lint && make test  # Verify development environment
```

### Contribution Guidelines

1. **Fork and Branch**: Create feature branches from `main`
2. **Code Quality**: Follow the established style guide and pass all quality checks
3. **Testing**: Include comprehensive tests for new functionality
4. **Documentation**: Update relevant documentation and API references
5. **ARM64 Optimization**: Consider ARM64-specific optimizations where applicable
6. **Security**: Ensure all code follows security best practices

### Quality Requirements

- All code must pass `make format-check`
- Static analysis with `make lint` must show no errors
- Security scan with `make security` must pass
- Test coverage should be maintained or improved
- Performance benchmarks should not regress

---

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

## Author

**dunamismax**
- Email: dunamismax@tutamail.com
- GitHub: [@dunamismax](https://github.com/dunamismax)

---

<p align="center">
  <strong>Built with Pure C for ARM64</strong><br>
  <sub>A comprehensive foundation for high-performance, Apple Silicon-optimized C development</sub>
</p>