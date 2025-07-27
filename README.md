<p align="center">
  <img src="https://github.com/dunamismax/images/blob/main/c/LLVMWyvern.png" alt="LLVM Wyvern C Programming" width="400" />
</p>

<p align="center">
  <a href="https://github.com/dunamismax/c-monolith">
    <img src="https://readme-typing-svg.demolab.com/?font=Fira+Code&size=24&pause=1000&color=3071A4&center=true&vCenter=true&width=1000&lines=C+Programming+Monolith;ARM64+Apple+Silicon+Optimized;Production+Ready+Build+System;Memory+Safe+Security+Hardened;Zero+Runtime+Dependencies;High+Performance+Applications;LLVM+Clang+Optimizations;Comprehensive+Test+Suite;Professional+Development+Tools;Cross+Platform+Compatibility;Make+Based+Build+System;Educational+and+Production+Ready;CLI+Tools+and+Interactive+Games;Static+Libraries+and+Applications;Security+Focused+Development;Modern+C11+Standards;Parallel+Build+Optimization;Link+Time+Optimization;Buffer+Overflow+Protection;MIT+Licensed+Open+Source" alt="Typing SVG" />
  </a>
</p>

<p align="center">
  <a href="https://clang.llvm.org/"><img src="https://img.shields.io/badge/Clang-17+-3071A4.svg?logo=llvm" alt="Clang Version"></a>
  <a href="https://developer.apple.com/documentation/apple-silicon"><img src="https://img.shields.io/badge/ARM64-Apple_Silicon-000000.svg?logo=apple" alt="ARM64 Optimization"></a>
  <a href="https://en.wikipedia.org/wiki/C11_(C_standard_revision)"><img src="https://img.shields.io/badge/C11-Standard-00599C.svg" alt="C11 Standard"></a>
  <a href="https://www.gnu.org/software/make/"><img src="https://img.shields.io/badge/Make-Build_System-3071A4.svg" alt="Make Build System"></a>
  <a href="https://en.wikipedia.org/wiki/Link-time_optimization"><img src="https://img.shields.io/badge/LTO-Optimized-FF6B35.svg" alt="Link Time Optimization"></a>
  <a href="https://clang.llvm.org/docs/AddressSanitizer.html"><img src="https://img.shields.io/badge/ASAN-Memory_Safety-FF0000.svg" alt="AddressSanitizer"></a>
  <a href="https://github.com/dunamismax/c-monolith/actions"><img src="https://img.shields.io/badge/Tests-Passing-28A745.svg" alt="Test Suite"></a>
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-green.svg" alt="MIT License"></a>
  <a href="https://github.com/dunamismax/c-monolith/actions/workflows/c.yml"><img src="https://github.com/dunamismax/c-monolith/actions/workflows/c.yml/badge.svg" alt="CI"></a>
</p>

---

# C Programming Monolith

A production-ready C monolith featuring ARM64-optimized applications, comprehensive security hardening, and a professional build system. Demonstrates modern C development practices with **zero runtime dependencies**, **memory safety**, and **high-performance optimizations**.

## Core Features

- **ARM64 Optimization**: Apple Silicon-specific compiler flags with LTO and vectorization
- **Security Hardened**: Buffer overflow protection, input validation, and memory safety
- **Professional Build System**: Enhanced Make-based system replacing all scripts
- **Comprehensive Testing**: Unit, integration, and security tests with coverage reporting
- **Production Ready**: Memory-safe applications with professional error handling
- **Zero Dependencies**: Self-contained binaries requiring no external libraries

<p align="center">
  <img src="https://github.com/dunamismax/images/blob/main/c/C-Logo.png" alt="C Programming Language Logo" width="150" />
</p>

## Project Structure

```sh
├── apps/                     # Applications (all categories)
│   ├── cli/                  # Command-line utilities
│   │   ├── calculator/       # Scientific calculator with advanced math
│   │   │   └── src/calculator.c
│   │   └── text_processor/   # String manipulation and text processing
│   │       └── src/text_processor.c
│   ├── games/                # Interactive gaming applications
│   │   ├── tic_tac_toe/      # AI-powered tic-tac-toe game
│   │   │   └── src/tic_tac_toe.c
│   │   └── number_guessing/  # Multi-difficulty guessing game
│   │       └── src/number_guessing.c
│   ├── network/              # Network programming and client-server apps
│   ├── system/               # Low-level system interaction projects
│   │   └── file_utils/       # Secure file operations and utilities
│   │       └── src/file_utils.c
│   ├── data/                 # Data management and record systems
│   └── embedded/             # Hardware interaction and microcontroller projects
├── libs/                     # High-performance shared libraries
│   ├── math_utils/           # Mathematical algorithms and utilities
│   │   ├── include/math_utils.h
│   │   └── src/math_utils.c
│   └── data_structures/      # Dynamic data structures
│       ├── include/          # Vector and generic data structures
│       │   ├── vector.h      # Integer vector implementation
│       │   └── generic_vector.h # Generic type-safe vector
│       └── src/
│           ├── vector.c      # Integer vector implementation
│           └── generic_vector.c # Generic vector implementation
├── tests/                    # Comprehensive test suite
│   ├── libs/                 # Library unit tests
│   │   ├── test_math_utils.c
│   │   ├── test_vector.c
│   │   └── test_generic_vector.c # Generic vector tests
│   ├── apps/                 # Application integration tests
│   │   └── test_integration.c
│   └── test_framework.h      # Thread-safe testing framework
├── Makefile                  # Professional build system
├── LICENSE                   # MIT License
└── README.md                 # This file
```

---

## Quick Start

**Prerequisites:** LLVM/Clang 15+ or GCC 11+

### Get Running in 3 Steps

```bash
# 1. Clone and navigate
git clone https://github.com/dunamismax/c-monolith.git
cd c-monolith

# 2. Build everything with optimizations
make

# 3. Run applications
make run-calculator     # Scientific calculator
make run-tic_tac_toe   # AI-powered game
```

The build system automatically detects your platform and applies optimal compiler flags.

<p align="center">
  <img src="https://github.com/dunamismax/images/blob/main/c/c-programming-typed.png" alt="C Programming Typed" width="300" />
</p>

## Why C for System Applications?

C provides unmatched control over system resources and memory management. This monolith demonstrates how modern C development can achieve both safety and performance through careful design and comprehensive testing.

| Aspect               | High-Level Languages | This C Monolith           |
| -------------------- | -------------------- | ------------------------- |
| Runtime Dependencies | Heavy                | Zero dependencies         |
| Memory Management    | Garbage collected    | Manual with safety checks |
| Performance          | Variable overhead    | Consistently optimal      |
| Binary Size          | Large                | Minimal footprint         |
| Startup Time         | Slow                 | Instant                   |
| Platform Control     | Limited              | Complete system access    |

## Make Build System

Professional ARM64-optimized build system with comprehensive targets:

### Build Targets

```bash
make                    # Build everything (release mode)
make libs              # Build libraries only
make apps              # Build applications only
make build-tests       # Build test suite
```

### Test Targets

```bash
make test              # Run all tests (same as test-run)
make test-run          # Full test suite with colored reporting (4 suites)
make test-quick        # Quick tests only (math, vector, generic vector)
make test-libs         # Test libraries only (all 3 library test suites)
make test-apps         # Test applications only (integration tests)
make test-coverage     # Run tests with coverage reporting
```

### Clean Targets

```bash
make clean             # Clean build artifacts
make clean-all         # Comprehensive cleanup (temp files, logs)
make clean-verify      # Clean and verify removal
```

### Development & Verification

```bash
make verify            # Production readiness check
make install           # Install to /usr/local
make format            # Format code with clang-format
make lint              # Run static analysis
```

### Build Modes & App Running

```bash
# Build modes
make MODE=release      # Maximum performance (default)
make MODE=debug        # Debug with sanitizers
make MODE=profile      # Profiling enabled

# Run individual apps
make run-<app>         # Run specific application
```

<p align="center">
  <img src="https://github.com/dunamismax/images/blob/main/c/logo-logo-modern.png" alt="Modern C Logo" width="200" />
</p>

## Technology Stack

- **Compiler**: LLVM Clang with ARM64-specific optimizations
- **Build System**: Professional Makefile with parallel builds and LTO
- **Testing**: Custom framework with comprehensive coverage
- **Security**: AddressSanitizer, UBSan, and stack protection
- **Optimization**: Link-time optimization with Apple Silicon tuning

## Platform Support

Automatically detects and optimizes for:

- **Apple Silicon** (M1/M2/M3) with specific CPU tuning
- **Intel x86_64** with native optimizations
- **Linux ARM64** and x86_64 support

## Application Categories

Explore comprehensive C programming projects organized by domain and complexity:

### 🖥️ [CLI Applications](https://github.com/dunamismax/c-monolith/blob/main/apps/cli/README.md)

Command-line utilities and tools for everyday system tasks and calculations.

### 🎮 [Interactive Games](https://github.com/dunamismax/c-monolith/blob/main/apps/games/README.md)

Console-based games and interactive entertainment applications.

### 🌐 [Network Applications](https://github.com/dunamismax/c-monolith/blob/main/apps/network/README.md)

Network programming projects including client-server applications and protocols.

### ⚙️ [System Programming](https://github.com/dunamismax/c-monolith/blob/main/apps/system/README.md)

Low-level system interaction projects including shells and system utilities.

### 📊 [Data Management](https://github.com/dunamismax/c-monolith/blob/main/apps/data/README.md)

Database and record management systems for handling structured data.

### 🔧 [Embedded & Hardware](https://github.com/dunamismax/c-monolith/blob/main/apps/embedded/README.md)

Hardware interaction and microcontroller programming projects.

## Shared Libraries

### Math Utils Library

High-performance mathematical operations optimized for ARM64:

- **Functions**: GCD/LCM, factorial, Fibonacci, prime checking, power
- **Optimization**: ARM64 vectorization, overflow detection
- **Safety**: Integer overflow protection, bounds checking

### Data Structures Library

Memory-safe dynamic data structures with comprehensive functionality:

- **Integer Vector**: High-performance dynamic arrays for integers
- **Generic Vector**: Type-safe dynamic arrays for any data type
- **Safety**: Bounds checking, null pointer validation, overflow protection
- **Performance**: Efficient memory management, optimal growth algorithms
- **Features**: Insert, remove, sort, find, copy operations with automatic resizing

## Testing & Quality Assurance

### Comprehensive Test Suite

```bash
# Test execution examples
make test                    # All tests with colored output
make test-coverage          # Generate coverage reports
make test-individual        # Detailed test output
```

**Test Coverage:**

- **Unit Tests**: Library functions with comprehensive edge case testing
  - Math utilities with overflow protection verification
  - Vector operations with bounds checking
  - Generic vector with complex data structure testing
- **Integration Tests**: Application functionality and security validation
- **Security Tests**: Buffer overflow, path traversal, format string protection
- **Thread Safety**: Enhanced test framework with proper state management
- **Performance Tests**: Benchmarking and optimization validation

### Security Features

- **Memory Safety**: AddressSanitizer and UndefinedBehaviorSanitizer with enhanced coverage
- **Buffer Protection**: Stack canaries, bounds checking, and overflow detection
- **Input Validation**: Comprehensive sanitization and validation across all applications
- **Path Security**: Directory traversal protection and canonical path validation
- **Integer Safety**: Robust overflow detection in mathematical operations
- **Format String Protection**: Prevention of format string vulnerabilities

## Development Workflow

```bash
# Complete development cycle
make clean-all              # Clean everything
make MODE=debug test        # Debug build with testing
make format lint           # Code formatting and analysis
make verify                # Production readiness check
make MODE=release          # Optimized production build
```

## Performance Optimization

### ARM64 Apple Silicon Tuning

```makefile
# Automatic optimization flags
-mcpu=apple-m1 -mtune=apple-m1 -arch arm64    # Apple Silicon
-O3 -flto=thin -ffast-math                    # Maximum optimization
-fvectorize -funroll-loops                    # Vectorization
-falign-functions=32 -falign-loops=32         # Memory alignment
```

### Parallel Building

- **Automatic Core Detection**: Uses all available CPU cores
- **Dependency Management**: Smart rebuild only when necessary
- **Link-Time Optimization**: Whole-program optimization

## Cross-Platform Support

- **macOS**: ARM64 and Intel with Apple-specific optimizations
- **Linux**: ARM64 and x86_64 with GCC and Clang support
- **Build System**: Automatic platform detection and flag adaptation

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

<p align="center">
  <a href="https://www.buymeacoffee.com/dunamismax">
    <img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" >
  </a>
</p>

<p align="center">
  <a href="https://twitter.com/dunamismax" target="_blank"><img src="https://img.shields.io/badge/Twitter-%231DA1F2.svg?&style=for-the-badge&logo=twitter&logoColor=white" alt="Twitter"></a>
  <a href="https://bsky.app/profile/dunamismax.bsky.social" target="_blank"><img src="https://img.shields.io/badge/Bluesky-blue?style=for-the-badge&logo=bluesky&logoColor=white" alt="Bluesky"></a>
  <a href="https://reddit.com/user/dunamismax" target="_blank"><img src="https://img.shields.io/badge/Reddit-%23FF4500.svg?&style=for-the-badge&logo=reddit&logoColor=white" alt="Reddit"></a>
  <a href="https://discord.com/users/dunamismax" target="_blank"><img src="https://img.shields.io/badge/Discord-dunamismax-7289DA.svg?style=for-the-badge&logo=discord&logoColor=white" alt="Discord"></a>
  <a href="https://signal.me/#p/+dunamismax.66" target="_blank"><img src="https://img.shields.io/badge/Signal-dunamismax.66-3A76F0.svg?style=for-the-badge&logo=signal&logoColor=white" alt="Signal"></a>
</p>

---

<p align="center">
  <strong>C Programming Monolith</strong><br>
  <sub>Memory Safe • High Performance • ARM64 Optimized • Production Ready • Zero Dependencies</sub>
</p>

<p align="center">
  <img src="https://github.com/dunamismax/images/blob/main/c/C-programming.png" alt="C Programming" width="400" />
</p>
