# C Monorepo - ARM64 Optimized Makefile
# Author: dunamismax <dunamismax@tutamail.com>
# Architecture: ARM64 (Apple Silicon M-series optimized)
# Platform: macOS (Darwin) primary, with cross-platform support

# =============================================================================
# BUILD CONFIGURATION
# =============================================================================

# Default compiler optimized for ARM64/Apple Silicon
CC := clang
AR := ar
RANLIB := ranlib

# ARM64-specific compiler flags for maximum performance
BASE_CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wstrict-prototypes \
               -Wmissing-prototypes -Wold-style-definition -Wno-unused-parameter
ARM64_OPTS := -mcpu=apple-m1 -mtune=apple-m1 -arch arm64
SECURITY_FLAGS := -fstack-protector-strong -D_FORTIFY_SOURCE=2
MODERN_FLAGS := -Wformat=2 -Wformat-security -Wnull-dereference -Wstack-protector

# Build modes with ARM64 optimization
DEBUG_FLAGS := -g3 -O0 -DDEBUG -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS := -O3 -DNDEBUG -flto -ffast-math -funroll-loops -fomit-frame-pointer
PROFILE_FLAGS := -O2 -g -pg -fprofile-arcs -ftest-coverage

# Determine build mode
MODE ?= debug
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

# Final compiler flags
CFLAGS := $(BASE_CFLAGS) $(ARM64_OPTS) $(SECURITY_FLAGS) $(MODERN_FLAGS) $(OPT_FLAGS)

# =============================================================================
# DIRECTORY STRUCTURE
# =============================================================================

# Core directories
APPS_DIR := apps
LIBS_DIR := libs
BUILD_DIR := build
SCRIPTS_DIR := scripts
TESTS_DIR := tests
DOCS_DIR := docs

# Build output directories
BIN_DIR := $(BUILD_DIR)/$(BUILD_SUFFIX)/bin
LIB_DIR := $(BUILD_DIR)/$(BUILD_SUFFIX)/lib
OBJ_DIR := $(BUILD_DIR)/$(BUILD_SUFFIX)/obj
COVERAGE_DIR := $(BUILD_DIR)/coverage
DOCS_OUT_DIR := $(BUILD_DIR)/docs

# =============================================================================
# PROJECT DISCOVERY
# =============================================================================

# Automatically discover all libraries and applications
LIB_DIRS := $(shell find $(LIBS_DIR) -maxdepth 1 -type d ! -path $(LIBS_DIR))
CLI_DIRS := $(shell find $(APPS_DIR)/cli -maxdepth 1 -type d ! -path $(APPS_DIR)/cli 2>/dev/null || true)
GUI_DIRS := $(shell find $(APPS_DIR)/gui -maxdepth 1 -type d ! -path $(APPS_DIR)/gui 2>/dev/null || true)
GAME_DIRS := $(shell find $(APPS_DIR)/games -maxdepth 1 -type d ! -path $(APPS_DIR)/games 2>/dev/null || true)

ALL_APP_DIRS := $(CLI_DIRS) $(GUI_DIRS) $(GAME_DIRS)

# Extract project names for individual targets
LIB_NAMES := $(notdir $(LIB_DIRS))
APP_NAMES := $(notdir $(ALL_APP_DIRS))

# =============================================================================
# BUILD TARGETS
# =============================================================================

# Default target
.PHONY: all
all: banner system-check libs apps tests
	@echo "$(GREEN)✓ Build completed successfully for $(BUILD_SUFFIX) mode$(NC)"
	@echo "$(BLUE)Available executables:$(NC)"
	@ls -la $(BIN_DIR)/ 2>/dev/null | grep -E '^-.*x.*' | awk '{print "  " $$9}' || echo "  No executables found"

# Build libraries
.PHONY: libs $(LIB_NAMES)
libs: $(LIB_NAMES)

$(LIB_NAMES):
	@echo "$(YELLOW)Building library: $@$(NC)"
	@$(MAKE) -C $(LIBS_DIR)/$@ MODE=$(MODE) --no-print-directory

# Build applications
.PHONY: apps $(APP_NAMES)
apps: libs $(APP_NAMES)

$(APP_NAMES): libs
	@echo "$(CYAN)Building application: $@$(NC)"
	@if [ -d "$(APPS_DIR)/cli/$@" ]; then \
		$(MAKE) -C $(APPS_DIR)/cli/$@ MODE=$(MODE) --no-print-directory; \
	elif [ -d "$(APPS_DIR)/gui/$@" ]; then \
		$(MAKE) -C $(APPS_DIR)/gui/$@ MODE=$(MODE) --no-print-directory; \
	elif [ -d "$(APPS_DIR)/games/$@" ]; then \
		$(MAKE) -C $(APPS_DIR)/games/$@ MODE=$(MODE) --no-print-directory; \
	fi

# Build and run tests
.PHONY: tests test check
tests test check: libs
	@echo "$(MAGENTA)Running test suite...$(NC)"
	@if [ -f $(TESTS_DIR)/Makefile ]; then \
		$(MAKE) -C $(TESTS_DIR) MODE=$(MODE) run --no-print-directory; \
	else \
		echo "$(YELLOW)No tests found$(NC)"; \
	fi

# =============================================================================
# DEVELOPMENT TARGETS
# =============================================================================

# Quick development builds (minimal rebuild)
.PHONY: quick
quick:
	@echo "$(GREEN)Quick build (incremental)...$(NC)"
	@$(MAKE) --no-print-directory -j$(shell sysctl -n hw.ncpu)

# Install targets
.PHONY: install install-local
install: all
	@echo "$(GREEN)Installing to /usr/local...$(NC)"
	@sudo mkdir -p /usr/local/bin /usr/local/lib /usr/local/include
	@if [ -d "$(BIN_DIR)" ]; then sudo cp -f $(BIN_DIR)/* /usr/local/bin/ 2>/dev/null || true; fi
	@if [ -d "$(LIB_DIR)" ]; then sudo cp -f $(LIB_DIR)/* /usr/local/lib/ 2>/dev/null || true; fi
	@echo "$(GREEN)✓ Installation completed$(NC)"

install-local: all
	@echo "$(GREEN)Installing to ~/local...$(NC)"
	@mkdir -p ~/local/bin ~/local/lib ~/local/include
	@if [ -d "$(BIN_DIR)" ]; then cp -f $(BIN_DIR)/* ~/local/bin/ 2>/dev/null || true; fi
	@if [ -d "$(LIB_DIR)" ]; then cp -f $(LIB_DIR)/* ~/local/lib/ 2>/dev/null || true; fi
	@echo "$(GREEN)✓ Local installation completed$(NC)"

# =============================================================================
# QUALITY ASSURANCE
# =============================================================================

# Code formatting
.PHONY: format format-check
format:
	@echo "$(BLUE)Formatting code...$(NC)"
	@find $(LIBS_DIR) $(APPS_DIR) -name "*.c" -o -name "*.h" | \
		xargs clang-format -i -style="{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 100}"
	@echo "$(GREEN)✓ Code formatted$(NC)"

format-check:
	@echo "$(BLUE)Checking code format...$(NC)"
	@find $(LIBS_DIR) $(APPS_DIR) -name "*.c" -o -name "*.h" | \
		xargs clang-format -style="{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 100}" --dry-run --Werror

# Static analysis
.PHONY: lint analyze
lint analyze:
	@echo "$(BLUE)Running static analysis...$(NC)"
	@if command -v clang-tidy >/dev/null 2>&1; then \
		find $(LIBS_DIR) $(APPS_DIR) -name "*.c" | \
		xargs clang-tidy --checks='-*,readability-*,performance-*,bugprone-*,clang-analyzer-*'; \
	else \
		echo "$(YELLOW)clang-tidy not found, skipping analysis$(NC)"; \
	fi

# Security scanning
.PHONY: security
security:
	@echo "$(BLUE)Security scan...$(NC)"
	@echo "Checking for unsafe functions..."
	@! grep -r "gets\|strcpy\|strcat\|sprintf\|scanf" $(LIBS_DIR) $(APPS_DIR) --include="*.c" || \
		(echo "$(RED)Warning: Found potentially unsafe functions$(NC)" && false)
	@echo "$(GREEN)✓ Security scan completed$(NC)"

# =============================================================================
# PERFORMANCE AND ANALYSIS
# =============================================================================

# Performance profiling
.PHONY: profile
profile:
	@echo "$(BLUE)Building with profiling enabled...$(NC)"
	@$(MAKE) MODE=profile all --no-print-directory
	@echo "$(GREEN)✓ Profile build completed. Run applications to generate profiling data.$(NC)"

# Code coverage
.PHONY: coverage
coverage:
	@echo "$(BLUE)Generating code coverage...$(NC)"
	@$(MAKE) MODE=profile test --no-print-directory
	@mkdir -p $(COVERAGE_DIR)
	@gcov $(OBJ_DIR)/*/*.o > $(COVERAGE_DIR)/coverage.txt 2>/dev/null || true
	@echo "$(GREEN)✓ Coverage data generated in $(COVERAGE_DIR)$(NC)"

# Benchmarking
.PHONY: benchmark
benchmark: all
	@echo "$(BLUE)Running benchmarks...$(NC)"
	@for app in $(BIN_DIR)/*; do \
		if [ -x "$$app" ]; then \
			echo "Benchmarking $$(basename $$app)..."; \
			time "$$app" --version 2>/dev/null || time "$$app" < /dev/null || true; \
		fi; \
	done

# =============================================================================
# SYSTEM INFORMATION AND DIAGNOSTICS
# =============================================================================

# System information
.PHONY: sysinfo system-info system-check
sysinfo system-info system-check:
	@echo "$(BLUE)System Information:$(NC)"
	@echo "  OS: $$(uname -s) $$(uname -r)"
	@echo "  Architecture: $$(uname -m)"
	@echo "  CPU: $$(sysctl -n machdep.cpu.brand_string 2>/dev/null || echo 'Unknown')"
	@echo "  Cores: $$(sysctl -n hw.ncpu 2>/dev/null || echo 'Unknown')"
	@echo "  Memory: $$(echo $$(($$(sysctl -n hw.memsize 2>/dev/null || echo 0) / 1024 / 1024 / 1024))GB)"
	@echo "  Compiler: $$($(CC) --version | head -n1)"
	@echo "  Build Mode: $(MODE)"
	@echo "  CFLAGS: $(CFLAGS)"

# Dependency checking
.PHONY: deps check-deps
deps check-deps:
	@echo "$(BLUE)Checking dependencies...$(NC)"
	@echo "Required tools:"
	@command -v $(CC) >/dev/null && echo "  ✓ $(CC)" || echo "  ✗ $(CC) (missing)"
	@command -v make >/dev/null && echo "  ✓ make" || echo "  ✗ make (missing)"
	@command -v ar >/dev/null && echo "  ✓ ar" || echo "  ✗ ar (missing)"
	@echo "Optional tools:"
	@command -v clang-format >/dev/null && echo "  ✓ clang-format" || echo "  ○ clang-format (optional)"
	@command -v clang-tidy >/dev/null && echo "  ✓ clang-tidy" || echo "  ○ clang-tidy (optional)"
	@command -v doxygen >/dev/null && echo "  ✓ doxygen" || echo "  ○ doxygen (optional)"

# =============================================================================
# RUN TARGETS
# =============================================================================

# Create run targets for all applications
.PHONY: $(addprefix run-, $(APP_NAMES))
$(addprefix run-, $(APP_NAMES)): run-%: %
	@echo "$(GREEN)Running $*...$(NC)"
	@if [ -x "$(BIN_DIR)/$*" ]; then \
		$(BIN_DIR)/$*; \
	else \
		echo "$(RED)Executable $* not found. Try building first: make $*$(NC)"; \
		exit 1; \
	fi

# =============================================================================
# CLEANUP TARGETS
# =============================================================================

# Clean build artifacts
.PHONY: clean clean-libs clean-apps clean-tests
clean: clean-libs clean-apps clean-tests
	@echo "$(YELLOW)Cleaning build artifacts...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ Cleanup completed$(NC)"

clean-libs:
	@for lib in $(LIB_DIRS); do \
		if [ -f "$$lib/Makefile" ]; then \
			$(MAKE) -C "$$lib" clean --no-print-directory; \
		fi; \
	done

clean-apps:
	@for app in $(ALL_APP_DIRS); do \
		if [ -f "$$app/Makefile" ]; then \
			$(MAKE) -C "$$app" clean --no-print-directory; \
		fi; \
	done

clean-tests:
	@if [ -f $(TESTS_DIR)/Makefile ]; then \
		$(MAKE) -C $(TESTS_DIR) clean --no-print-directory; \
	fi

# Deep clean
.PHONY: distclean
distclean: clean
	@echo "$(YELLOW)Deep cleaning...$(NC)"
	@rm -rf $(BUILD_DIR) $(COVERAGE_DIR) $(DOCS_OUT_DIR)
	@find . -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" | xargs rm -f
	@find . -name ".DS_Store" | xargs rm -f
	@echo "$(GREEN)✓ Deep cleanup completed$(NC)"

# =============================================================================
# DOCUMENTATION
# =============================================================================

.PHONY: docs docs-open
docs:
	@echo "$(BLUE)Generating documentation...$(NC)"
	@mkdir -p $(DOCS_OUT_DIR)
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile 2>/dev/null || echo "$(YELLOW)Doxygen config not found$(NC)"; \
	else \
		echo "$(YELLOW)doxygen not found, generating simple docs$(NC)"; \
		find $(LIBS_DIR) -name "*.h" -exec echo "=== {} ===" \; -exec cat {} \; > $(DOCS_OUT_DIR)/api.txt; \
	fi
	@echo "$(GREEN)✓ Documentation generated in $(DOCS_OUT_DIR)$(NC)"

docs-open: docs
	@if [ -f "$(DOCS_OUT_DIR)/html/index.html" ]; then \
		open $(DOCS_OUT_DIR)/html/index.html; \
	elif [ -f "$(DOCS_OUT_DIR)/api.txt" ]; then \
		open $(DOCS_OUT_DIR)/api.txt; \
	fi

# =============================================================================
# UTILITY TARGETS
# =============================================================================

# Project banner
.PHONY: banner
banner:
	@echo "$(CYAN)"
	@echo "╔══════════════════════════════════════════════════════════════════════╗"
	@echo "║                    C Programming Monorepo                           ║"
	@echo "║                 ARM64 Optimized Build System                        ║"
	@echo "║                                                                      ║"
	@echo "║  Architecture: ARM64 (Apple Silicon)    Mode: $(MODE)$(shell printf '%*s' $$((17 - ${#MODE})) '')║"
	@echo "║  Compiler: $(CC)$(shell printf '%*s' $$((40 - ${#CC})) '')║"
	@echo "║  Author: dunamismax@tutamail.com                                    ║"
	@echo "╚══════════════════════════════════════════════════════════════════════╝"
	@echo "$(NC)"

# Version information
.PHONY: version
version:
	@echo "C Monorepo v1.0.0"
	@echo "Built for: ARM64 (Apple Silicon)"
	@echo "Author: dunamismax <dunamismax@tutamail.com>"
	@echo "Build mode: $(MODE)"

# Help system
.PHONY: help
help:
	@echo "$(CYAN)C Programming Monorepo - ARM64 Optimized Build System$(NC)"
	@echo ""
	@echo "$(YELLOW)Primary Targets:$(NC)"
	@echo "  all              Build everything (libraries + applications + tests)"
	@echo "  libs             Build all libraries"
	@echo "  apps             Build all applications"
	@echo "  tests            Build and run test suite"
	@echo "  quick            Fast incremental build"
	@echo ""
	@echo "$(YELLOW)Individual Targets:$(NC)"
	@echo "  Libraries:       $(LIB_NAMES)"
	@echo "  Applications:    $(APP_NAMES)"
	@echo "  Run apps:       $(addprefix run-, $(APP_NAMES))"
	@echo ""
	@echo "$(YELLOW)Quality Assurance:$(NC)"
	@echo "  format           Format code with clang-format"
	@echo "  format-check     Check code formatting"
	@echo "  lint             Static analysis with clang-tidy"
	@echo "  security         Security vulnerability scan"
	@echo "  coverage         Generate code coverage reports"
	@echo ""
	@echo "$(YELLOW)Build Modes:$(NC)"
	@echo "  make MODE=debug    Debug build with sanitizers (default)"
	@echo "  make MODE=release  Optimized release build with LTO"
	@echo "  make MODE=profile  Profile build with coverage"
	@echo ""
	@echo "$(YELLOW)System & Utilities:$(NC)"
	@echo "  sysinfo          Show system information"
	@echo "  deps             Check dependencies"
	@echo "  docs             Generate documentation"
	@echo "  install          Install to /usr/local"
	@echo "  install-local    Install to ~/local"
	@echo "  clean            Clean build artifacts"
	@echo "  distclean        Deep clean everything"
	@echo ""
	@echo "$(YELLOW)Examples:$(NC)"
	@echo "  make                           # Build everything in debug mode"
	@echo "  make MODE=release apps         # Build apps in release mode"
	@echo "  make calculator run-calculator # Build and run calculator"
	@echo "  make -j$$(sysctl -n hw.ncpu)            # Parallel build"
	@echo "  make format lint test          # Quality pipeline"

# =============================================================================
# DIRECTORY CREATION
# =============================================================================

$(BIN_DIR) $(LIB_DIR) $(OBJ_DIR) $(COVERAGE_DIR) $(DOCS_OUT_DIR):
	@mkdir -p $@

# =============================================================================
# COLOR DEFINITIONS
# =============================================================================

# ANSI color codes for beautiful output
RED     := \033[31m
GREEN   := \033[32m
YELLOW  := \033[33m
BLUE    := \033[34m
MAGENTA := \033[35m
CYAN    := \033[36m
WHITE   := \033[37m
NC      := \033[0m

# =============================================================================
# PARALLEL BUILD SUPPORT
# =============================================================================

# Enable parallel builds by default on macOS
MAKEFLAGS += -j$(shell sysctl -n hw.ncpu 2>/dev/null || echo 1)

# Mark all phony targets
.PHONY: all libs apps tests clean distclean help banner version sysinfo deps \
        format lint security coverage profile benchmark docs install \
        $(LIB_NAMES) $(APP_NAMES) $(addprefix run-, $(APP_NAMES))