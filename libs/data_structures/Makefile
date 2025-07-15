# Data Structures Library - ARM64 Optimized Makefile
# Author: dunamismax <dunamismax@tutamail.com>
# Optimized for Apple Silicon (ARM64) architecture

# =============================================================================
# BUILD CONFIGURATION
# =============================================================================

# Compiler and tools optimized for ARM64
CC := clang
AR := ar
RANLIB := ranlib

# ARM64-specific optimization flags
BASE_CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wstrict-prototypes \
               -Wmissing-prototypes -Wold-style-definition -fPIC
ARM64_OPTS := -mcpu=apple-m1 -mtune=apple-m1 -arch arm64
SECURITY_FLAGS := -fstack-protector-strong -D_FORTIFY_SOURCE=2
MODERN_FLAGS := -Wformat=2 -Wformat-security -Wnull-dereference

# Build mode flags
DEBUG_FLAGS := -g3 -O0 -DDEBUG -fsanitize=address
RELEASE_FLAGS := -O3 -DNDEBUG -flto -ffast-math -funroll-loops
PROFILE_FLAGS := -O2 -g -pg -fprofile-arcs -ftest-coverage

# =============================================================================
# DIRECTORIES AND PATHS
# =============================================================================

SRC_DIR := src
INCLUDE_DIR := include
BUILD_ROOT := ../../build
MODE ?= debug

# Build-specific directories
ifeq ($(MODE),release)
    OPT_FLAGS := $(RELEASE_FLAGS)
    BUILD_SUFFIX := release
else ifeq ($(MODE),profile)
    OPT_FLAGS := $(PROFILE_FLAGS)
    BUILD_SUFFIX := profile
else
    OPT_FLAGS := $(DEBUG_FLAGS)
    BUILD_SUFFIX := debug
endif

OBJ_DIR := $(BUILD_ROOT)/$(BUILD_SUFFIX)/obj/data_structures
LIB_DIR := $(BUILD_ROOT)/$(BUILD_SUFFIX)/lib
COVERAGE_DIR := $(BUILD_ROOT)/coverage

# Final compiler flags
CFLAGS := $(BASE_CFLAGS) $(ARM64_OPTS) $(SECURITY_FLAGS) $(MODERN_FLAGS) $(OPT_FLAGS)
INCLUDES := -I$(INCLUDE_DIR)

# =============================================================================
# SOURCE FILES AND TARGETS
# =============================================================================

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)

# Library target
LIBRARY := $(LIB_DIR)/libdatastructures.a

# =============================================================================
# BUILD TARGETS
# =============================================================================

# Default target
.PHONY: all
all: $(LIBRARY)

# Create static library
$(LIBRARY): $(OBJECTS) | $(LIB_DIR)
	@echo "  AR    $@"
	@$(AR) rcs $@ $(OBJECTS)
	@$(RANLIB) $@

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create necessary directories
$(OBJ_DIR) $(LIB_DIR) $(COVERAGE_DIR):
	@mkdir -p $@

# =============================================================================
# DEVELOPMENT TARGETS
# =============================================================================

# Format code
.PHONY: format
format:
	@echo "Formatting data_structures library..."
	@clang-format -i -style="{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 100}" \
		$(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h

# Static analysis
.PHONY: lint
lint:
	@echo "Analyzing data_structures library..."
	@if command -v clang-tidy >/dev/null 2>&1; then \
		clang-tidy $(SRC_DIR)/*.c -- $(CFLAGS) $(INCLUDES); \
	else \
		echo "clang-tidy not found, skipping analysis"; \
	fi

# Generate coverage data
.PHONY: coverage
coverage: $(OBJECTS) | $(COVERAGE_DIR)
	@echo "Generating coverage for data_structures..."
	@gcov $(OBJECTS) > $(COVERAGE_DIR)/data_structures_coverage.txt 2>/dev/null || true

# =============================================================================
# TESTING TARGETS
# =============================================================================

# Unit test target (if test files exist)
.PHONY: test
test: $(LIBRARY)
	@echo "Testing data_structures library..."
	@if [ -f "test/test_vector.c" ]; then \
		$(CC) $(CFLAGS) $(INCLUDES) -L$(LIB_DIR) test/test_vector.c -ldatastructures -o test_vector && \
		./test_vector && rm -f test_vector; \
	fi

# =============================================================================
# INSTALL TARGETS
# =============================================================================

.PHONY: install install-local
install: $(LIBRARY)
	@echo "Installing data_structures library to /usr/local..."
	@sudo mkdir -p /usr/local/lib /usr/local/include
	@sudo cp $(LIBRARY) /usr/local/lib/
	@sudo cp $(INCLUDE_DIR)/*.h /usr/local/include/

install-local: $(LIBRARY)
	@echo "Installing data_structures library to ~/local..."
	@mkdir -p ~/local/lib ~/local/include
	@cp $(LIBRARY) ~/local/lib/
	@cp $(INCLUDE_DIR)/*.h ~/local/include/

# =============================================================================
# CLEANUP TARGETS
# =============================================================================

.PHONY: clean
clean:
	@echo "Cleaning data_structures library..."
	@rm -rf $(OBJ_DIR) $(LIBRARY)
	@find . -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" | xargs rm -f

.PHONY: distclean
distclean: clean
	@rm -rf $(BUILD_ROOT)

# =============================================================================
# UTILITY TARGETS
# =============================================================================

.PHONY: info
info:
	@echo "Data Structures Library Information:"
	@echo "  Sources: $(SOURCES)"
	@echo "  Objects: $(OBJECTS)"
	@echo "  Headers: $(HEADERS)"
	@echo "  Library: $(LIBRARY)"
	@echo "  Mode: $(MODE)"
	@echo "  CFLAGS: $(CFLAGS)"

.PHONY: help
help:
	@echo "Data Structures Library - ARM64 Optimized"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build the library (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  distclean - Remove all generated files"
	@echo "  format    - Format source code"
	@echo "  lint      - Run static analysis"
	@echo "  test      - Run unit tests"
	@echo "  coverage  - Generate coverage data"
	@echo "  install   - Install to /usr/local"
	@echo "  install-local - Install to ~/local"
	@echo "  info      - Show build information"
	@echo "  help      - Show this help"
	@echo ""
	@echo "Build modes:"
	@echo "  make MODE=debug   - Debug build (default)"
	@echo "  make MODE=release - Release build"
	@echo "  make MODE=profile - Profile build"

# Mark targets as phony
.PHONY: all clean distclean format lint test coverage install install-local info help