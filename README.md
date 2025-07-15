<p align="center">
  <img src="c.png" alt="C Monorepo Logo" width="200" />
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

- **Security Hardened**: Protection against buffer overflows, path traversal, format string attacks, and integer overflows
- **ARM64 Optimized**: Built specifically for Apple Silicon with aggressive compiler optimizations
- **Production Ready**: Memory-safe code with comprehensive input validation, error handling, and security controls
- **Professional Build System**: Advanced Makefile with 65+ targets orchestrating parallel builds and dependency management
- **Comprehensive Testing**: Unit tests, integration tests, security tests, and static analysis
- **Educational Excellence**: Working demo applications showcasing modern C programming techniques and security best practices
- **Developer Tools**: Complete scripts for building, testing, formatting, deployment automation, and security scanning
- **Enterprise Documentation**: Security guidelines, deployment guides, and maintenance procedures

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

<details>
<summary><strong>Project Architecture</strong></summary>

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
├── scripts/                   # Development automation
├── tests/                     # Test suite
├── docs/                      # Documentation
│   ├── SECURITY.md           # Security guidelines and best practices
│   ├── DEPLOYMENT.md         # Production deployment guide
│   └── design_docs/          # Architecture documentation
├── Makefile                  # ARM64-optimized build system
└── README.md                 # This file
```

### Technology Stack

- **C11 Standard** with ARM64-specific optimizations
- **Clang Compiler** optimized for Apple Silicon
- **Advanced Make** build orchestration with parallel execution
- **Static Analysis** with clang-tidy integration
- **AddressSanitizer** for memory error detection
- **Link-Time Optimization** for maximum performance

</details>

---

<details>
<summary><strong>Demo Applications</strong></summary>

### Calculator

Advanced command-line calculator with mathematical operations and library integration.

```bash
make run-calculator
calc> + 5 3        # Result: 8.00
calc> fact 5       # Result: 120
calc> prime 17     # Result: 17 is prime
```

### File Utils

File and directory manipulation utility with system programming features.

```bash
make run-file_utils
fileutils> info README.md        # File information
fileutils> count myfile.txt      # Line/word count
fileutils> copy src.txt dst.txt  # Safe file copying
```

### Text Processor

Text manipulation tool with string algorithms and data structures.

```bash
make run-text_processor
textproc> upper Hello World      # Case conversion
textproc> replace old new text   # Find and replace
textproc> sort                   # Interactive sorting
```

### Tic-Tac-Toe Game

Interactive game with intelligent AI opponent and game logic.

- Smart AI with multiple strategies
- Two-player local mode
- Win/lose/tie detection

### Number Guessing Game

Multi-difficulty guessing game with scoring and statistics.

- Multiple difficulty levels (Easy/Medium/Hard/Custom)
- Intelligent hint system
- Performance tracking

</details>

---

<details>
<summary><strong>Shared Libraries</strong></summary>

### Data Structures Library (`libdatastructures.a`)

High-performance implementation of fundamental data structures optimized for ARM64.

```c
#include "vector.h"

Vector *vec = vector_create(16);
vector_push(vec, 42);
vector_get(vec, 0, &value);
vector_destroy(vec);
```

Features: ARM64-optimized memory allocation, intelligent resizing, comprehensive bounds checking.

### Math Utils Library (`libmathutils.a`)

Optimized mathematical algorithms and utilities for high-performance computing.

```c
#include "math_utils.h"

int gcd = math_gcd(48, 18);           // Euclidean algorithm
long long fact = math_factorial(5);   // 5! = 120
int is_prime = math_is_prime(17);     // Primality test
```

Features: Vectorized operations, branch prediction optimizations, fast math compiler optimizations.

</details>

---

<details>
<summary><strong>Development Workflow</strong></summary>

### Code Quality Pipeline

```bash
make format lint security test coverage
```

### Performance Analysis

```bash
make profile benchmark MODE=release
```

### Code Style Guidelines

- Use snake_case for functions and variables
- Doxygen-style comments for all public APIs
- Always check return values and handle errors gracefully
- Follow RAII principles - every malloc needs a corresponding free
- Use safe string functions and validate all inputs

</details>

---

<details>
<summary><strong>Testing & Quality Assurance</strong></summary>

### Comprehensive Testing Suite

```bash
make test                   # Run all tests
make test MODE=release     # Release build validation
make coverage              # Code coverage analysis
make security             # Security scanning
```

### Static Analysis Integration

- **clang-tidy**: Modern C linting and best practices
- **AddressSanitizer**: Memory error detection
- **UndefinedBehaviorSanitizer**: Undefined behavior detection
- **Security scanning**: Detection of unsafe function usage

### Performance Benchmarking

```bash
make benchmark              # System-wide performance analysis
```

</details>

---

<details>
<summary><strong>Security Features</strong></summary>

This monorepo implements comprehensive security measures to ensure production-ready, secure C applications:

### Memory Safety

- Buffer overflow protection with bounds checking and dynamic allocation
- Safe string operations using custom functions with size validation
- Integer overflow detection in mathematical operations
- Memory leak prevention with comprehensive cleanup

### Input Validation

- Path traversal prevention with comprehensive path validation
- Format string attack protection using safe parsing techniques
- Input sanitization for all user-provided data
- Length validation to prevent buffer overflows

### Security Testing

- Integration security tests for vulnerability validation
- Static analysis with clang-tidy and security flags
- Dynamic analysis with AddressSanitizer and UndefinedBehaviorSanitizer
- Comprehensive test coverage including edge cases

### Secure Development

- Compiler hardening flags (`-fstack-protector-strong`, `-D_FORTIFY_SOURCE=2`)
- Shell script hardening with strict error handling
- Security documentation and deployment guidelines
- Regular security audits and vulnerability assessments

For detailed security information, see [`docs/SECURITY.md`](docs/SECURITY.md).

</details>

---

## Production Deployment

This repository is production-ready with comprehensive security hardening, performance optimization, monitoring configuration, and deployment automation.

For complete deployment instructions, see [`docs/DEPLOYMENT.md`](docs/DEPLOYMENT.md).

---

<details>
<summary><strong>Contributing</strong></summary>

### Development Setup

```bash
git clone https://github.com/dunamismax/c-monorepo.git
cd c-monorepo
make deps sysinfo
make format && make lint && make test
```

### Contribution Guidelines

1. Fork and create feature branches from `main`
2. Follow established style guide and pass all quality checks
3. Include comprehensive tests for new functionality
4. Update relevant documentation and API references
5. Ensure all code follows security best practices

### Quality Requirements

- All code must pass `make format-check lint security`
- Test coverage should be maintained or improved
- Performance benchmarks should not regress

</details>

---

## Support This Project

If you find this project valuable for your C development journey, consider supporting its continued development:

<p align="center">
  <a href="https://www.buymeacoffee.com/dunamismax" target="_blank">
    <img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" />
  </a>
</p>

---

## Let's Connect

<p align="center">
  <a href="https://twitter.com/dunamismax" target="_blank"><img src="https://img.shields.io/badge/Twitter-%231DA1F2.svg?&style=for-the-badge&logo=twitter&logoColor=white" alt="Twitter"></a>
  <a href="https://bsky.app/profile/dunamismax.bsky.social" target="_blank"><img src="https://img.shields.io/badge/Bluesky-blue?style=for-the-badge&logo=bluesky&logoColor=white" alt="Bluesky"></a>
  <a href="https://reddit.com/user/dunamismax" target="_blank"><img src="https://img.shields.io/badge/Reddit-%23FF4500.svg?&style=for-the-badge&logo=reddit&logoColor=white" alt="Reddit"></a>
  <a href="https://discord.com/users/dunamismax" target="_blank"><img src="https://img.shields.io/badge/Discord-dunamismax-7289DA.svg?style=for-the-badge&logo=discord&logoColor=white" alt="Discord"></a>
  <a href="https://signal.me/#p/+dunamismax.66" target="_blank"><img src="https://img.shields.io/badge/Signal-dunamismax.66-3A76F0.svg?style=for-the-badge&logo=signal&logoColor=white" alt="Signal"></a>
</p>

---

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

<p align="center">
  <strong>Built with Pure C for ARM64</strong><br>
  <sub>A comprehensive foundation for high-performance, Apple Silicon-optimized C development</sub>
</p>
