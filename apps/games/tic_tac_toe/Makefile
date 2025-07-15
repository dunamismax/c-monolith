# Tic-Tac-Toe Game - ARM64 Optimized Makefile
# Author: dunamismax <dunamismax@tutamail.com>
# Optimized for Apple Silicon (ARM64) architecture

# =============================================================================
# BUILD CONFIGURATION
# =============================================================================

# Compiler and tools optimized for ARM64
CC := clang
STRIP := strip

# ARM64-specific optimization flags
BASE_CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wstrict-prototypes \
               -Wmissing-prototypes -Wold-style-definition
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
BUILD_ROOT := ../../../build
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

BIN_DIR := $(BUILD_ROOT)/$(BUILD_SUFFIX)/bin
OBJ_DIR := $(BUILD_ROOT)/$(BUILD_SUFFIX)/obj/tic_tac_toe
LIB_DIR := $(BUILD_ROOT)/$(BUILD_SUFFIX)/lib

# Final compiler flags
CFLAGS := $(BASE_CFLAGS) $(ARM64_OPTS) $(SECURITY_FLAGS) $(MODERN_FLAGS) $(OPT_FLAGS)

# =============================================================================
# SOURCE FILES AND TARGETS
# =============================================================================

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET := $(BIN_DIR)/tic_tac_toe

# =============================================================================
# BUILD TARGETS
# =============================================================================

# Default target
.PHONY: all
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "  LINK  $@"
	@$(CC) $(OPT_FLAGS) $(OBJECTS) -o $@
ifeq ($(MODE),release)
	@echo "  STRIP $@"
	@$(STRIP) $@
endif

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

# =============================================================================
# DEVELOPMENT TARGETS
# =============================================================================

# Format code
.PHONY: format
format:
	@echo "Formatting tic_tac_toe game..."
	@clang-format -i -style="{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 100}" \
		$(SRC_DIR)/*.c

# Static analysis
.PHONY: lint
lint:
	@echo "Analyzing tic_tac_toe game..."
	@if command -v clang-tidy >/dev/null 2>&1; then \
		clang-tidy $(SRC_DIR)/*.c -- $(CFLAGS); \
	else \
		echo "clang-tidy not found, skipping analysis"; \
	fi

# Run the game
.PHONY: run
run: $(TARGET)
	@echo "Running tic_tac_toe..."
	@$(TARGET)

# Run automated test game
.PHONY: test-run
test-run: $(TARGET)
	@echo "Testing tic_tac_toe with automated input..."
	@echo -e "3" | $(TARGET)

# =============================================================================
# INSTALL TARGETS
# =============================================================================

.PHONY: install install-local
install: $(TARGET)
	@echo "Installing tic_tac_toe to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/

install-local: $(TARGET)
	@echo "Installing tic_tac_toe to ~/local/bin..."
	@mkdir -p ~/local/bin
	@cp $(TARGET) ~/local/bin/

# =============================================================================
# CLEANUP TARGETS
# =============================================================================

.PHONY: clean
clean:
	@echo "Cleaning tic_tac_toe..."
	@rm -rf $(OBJ_DIR) $(TARGET)
	@find . -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" | xargs rm -f

.PHONY: distclean
distclean: clean
	@rm -rf $(BUILD_ROOT)

# =============================================================================
# UTILITY TARGETS
# =============================================================================

.PHONY: info
info:
	@echo "Tic-Tac-Toe Game Information:"
	@echo "  Sources: $(SOURCES)"
	@echo "  Objects: $(OBJECTS)"
	@echo "  Target: $(TARGET)"
	@echo "  Mode: $(MODE)"
	@echo "  CFLAGS: $(CFLAGS)"

.PHONY: help
help:
	@echo "Tic-Tac-Toe Game - ARM64 Optimized"
	@echo ""
	@echo "Available targets:"
	@echo "  all         - Build the game (default)"
	@echo "  run         - Build and run tic_tac_toe"
	@echo "  test-run    - Run automated test"
	@echo "  clean       - Remove build artifacts"
	@echo "  distclean   - Remove all generated files"
	@echo "  format      - Format source code"
	@echo "  lint        - Run static analysis"
	@echo "  install     - Install to /usr/local/bin"
	@echo "  install-local - Install to ~/local/bin"
	@echo "  info        - Show build information"
	@echo "  help        - Show this help"
	@echo ""
	@echo "Build modes:"
	@echo "  make MODE=debug   - Debug build (default)"
	@echo "  make MODE=release - Release build"
	@echo "  make MODE=profile - Profile build"

# Mark targets as phony
.PHONY: all run test-run clean distclean format lint install install-local info help