# Tests Makefile

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99
INCLUDES := -I../libs/data_structures/include -I../libs/math_utils/include

# Directories
BUILD_DIR := ../build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj/tests
LIB_DIR := $(BUILD_DIR)/lib

# Build mode
MODE ?= debug
ifeq ($(MODE),release)
    CFLAGS += -O2 -DNDEBUG
else
    CFLAGS += -g -DDEBUG
endif

# Source files
LIB_TESTS := $(wildcard libs/*.c)
APP_TESTS := $(wildcard apps/*.c)

# Object files
LIB_TEST_OBJS := $(LIB_TESTS:libs/%.c=$(OBJ_DIR)/libs/%.o)
APP_TEST_OBJS := $(APP_TESTS:apps/%.c=$(OBJ_DIR)/apps/%.o)

# Test executables
LIB_TEST_EXES := $(LIB_TESTS:libs/%.c=$(BIN_DIR)/%)
APP_TEST_EXES := $(APP_TESTS:apps/%.c=$(BIN_DIR)/%)

# Libraries to link
LIBS := -L$(LIB_DIR) -ldatastructures -lmathutils

# Default target
.PHONY: all
all: lib-tests app-tests

# Build library tests
.PHONY: lib-tests
lib-tests: $(LIB_TEST_EXES)

# Build application tests
.PHONY: app-tests
app-tests: $(APP_TEST_EXES)

# Build and run all tests
.PHONY: run
run: all run-lib-tests run-app-tests

# Run library tests
.PHONY: run-lib-tests
run-lib-tests: lib-tests
	@echo "Running library tests..."
	@for test in $(LIB_TEST_EXES); do \
		if [ -f $$test ]; then \
			echo "Running $$test..."; \
			$$test || exit 1; \
			echo ""; \
		fi; \
	done

# Run application tests
.PHONY: run-app-tests
run-app-tests: app-tests
	@echo "Running application tests..."
	@for test in $(APP_TEST_EXES); do \
		if [ -f $$test ]; then \
			echo "Running $$test..."; \
			$$test || exit 1; \
			echo ""; \
		fi; \
	done

# Build library test executables
$(BIN_DIR)/test_%: $(OBJ_DIR)/libs/test_%.o | $(BIN_DIR)
	@echo "Linking library test: $@"
	@$(CC) $< $(LIBS) -o $@

# Build application test executables
$(BIN_DIR)/test_%: $(OBJ_DIR)/apps/test_%.o | $(BIN_DIR)
	@echo "Linking application test: $@"
	@$(CC) $< -o $@

# Compile library test source files
$(OBJ_DIR)/libs/%.o: libs/%.c | $(OBJ_DIR)/libs
	@echo "Compiling library test: $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile application test source files
$(OBJ_DIR)/apps/%.o: apps/%.c | $(OBJ_DIR)/apps
	@echo "Compiling application test: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Create directories
$(OBJ_DIR)/libs $(OBJ_DIR)/apps $(BIN_DIR):
	@mkdir -p $@

# Clean
.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR)/libs $(OBJ_DIR)/apps
	@rm -f $(LIB_TEST_EXES) $(APP_TEST_EXES)
	@echo "Cleaned tests"

# Show help
.PHONY: help
help:
	@echo "Test Suite"
	@echo "Available targets:"
	@echo "  all          - Build all tests (default)"
	@echo "  lib-tests    - Build library tests only"
	@echo "  app-tests    - Build application tests only"
	@echo "  run          - Build and run all tests"
	@echo "  run-lib-tests - Run library tests only"
	@echo "  run-app-tests - Run application tests only"
	@echo "  clean        - Remove test build artifacts"
	@echo "  help         - Show this help"