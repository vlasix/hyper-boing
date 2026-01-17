# Hyper Boing Makefile for Linux
# Requires SDL2, SDL2_image, SDL2_mixer, and SDL2_ttf

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++14 -Wall -Wextra
LDFLAGS :=

# Use pkg-config to get SDL2 flags
SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
SDL_LIBS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)

# Project settings
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Source files
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
DEPENDS := $(OBJECTS:.o=.d)

# Build type (default: release)
BUILD_TYPE ?= release

ifeq ($(BUILD_TYPE),debug)
    CXXFLAGS += -g -O0 -D_DEBUG
    BUILD_DIR := build/debug
else
    CXXFLAGS += -O3 -DNDEBUG
    BUILD_DIR := build/release
endif

# Update OBJ_DIR based on build type
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/hyper-boing

# Rebuild OBJECTS with correct path
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
DEPENDS := $(OBJECTS:.o=.d)

# Colors for output
COLOR_RESET := \033[0m
COLOR_BOLD := \033[1m
COLOR_GREEN := \033[32m
COLOR_BLUE := \033[34m
COLOR_YELLOW := \033[33m

# Default target
.PHONY: all
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)Linking$(COLOR_RESET) $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(SDL_LIBS)
	@echo "$(COLOR_BOLD)$(COLOR_GREEN)Build complete!$(COLOR_RESET) Output: $(TARGET)"

# Compile source files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "$(COLOR_BOLD)$(COLOR_BLUE)Compiling$(COLOR_RESET) $<..."
	@$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -MMD -MP -c $< -o $@

# Create build directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Include dependency files
-include $(DEPENDS)

# Run the game
.PHONY: run
run: $(TARGET)
	@echo "$(COLOR_BOLD)$(COLOR_YELLOW)Running$(COLOR_RESET) $(TARGET)..."
	@./$(TARGET)

# Debug build
.PHONY: debug
debug:
	@$(MAKE) BUILD_TYPE=debug

# Release build
.PHONY: release
release:
	@$(MAKE) BUILD_TYPE=release

# Clean build artifacts
.PHONY: clean
clean:
	@echo "$(COLOR_BOLD)Cleaning$(COLOR_RESET) build artifacts..."
	@rm -rf build $(TARGET)
	@echo "$(COLOR_GREEN)Clean complete!$(COLOR_RESET)"

# Clean and rebuild
.PHONY: rebuild
rebuild: clean all

# Install (optional - copies to /usr/local/bin)
.PHONY: install
install: $(TARGET)
	@echo "$(COLOR_BOLD)Installing$(COLOR_RESET) $(TARGET) to /usr/local/bin..."
	@install -m 755 $(TARGET) /usr/local/bin/
	@echo "$(COLOR_GREEN)Installation complete!$(COLOR_RESET)"

# Check SDL2 dependencies
.PHONY: check-deps
check-deps:
	@echo "$(COLOR_BOLD)Checking SDL2 dependencies...$(COLOR_RESET)"
	@pkg-config --exists sdl2 && echo "$(COLOR_GREEN)✓$(COLOR_RESET) SDL2 found" || echo "$(COLOR_YELLOW)✗$(COLOR_RESET) SDL2 not found"
	@pkg-config --exists SDL2_image && echo "$(COLOR_GREEN)✓$(COLOR_RESET) SDL2_image found" || echo "$(COLOR_YELLOW)✗$(COLOR_RESET) SDL2_image not found"
	@pkg-config --exists SDL2_mixer && echo "$(COLOR_GREEN)✓$(COLOR_RESET) SDL2_mixer found" || echo "$(COLOR_YELLOW)✗$(COLOR_RESET) SDL2_mixer not found"
	@pkg-config --exists SDL2_ttf && echo "$(COLOR_GREEN)✓$(COLOR_RESET) SDL2_ttf found" || echo "$(COLOR_YELLOW)✗$(COLOR_RESET) SDL2_ttf not found"

# Help target
.PHONY: help
help:
	@echo "$(COLOR_BOLD)Hyper Boing Makefile$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BOLD)Available targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)all$(COLOR_RESET)         - Build the project (default, release mode)"
	@echo "  $(COLOR_GREEN)debug$(COLOR_RESET)       - Build with debug symbols and no optimization"
	@echo "  $(COLOR_GREEN)release$(COLOR_RESET)     - Build with optimizations (default)"
	@echo "  $(COLOR_GREEN)run$(COLOR_RESET)         - Build and run the game"
	@echo "  $(COLOR_GREEN)clean$(COLOR_RESET)       - Remove build artifacts"
	@echo "  $(COLOR_GREEN)rebuild$(COLOR_RESET)     - Clean and rebuild"
	@echo "  $(COLOR_GREEN)install$(COLOR_RESET)     - Install to /usr/local/bin (requires sudo)"
	@echo "  $(COLOR_GREEN)check-deps$(COLOR_RESET)  - Check if SDL2 dependencies are installed"
	@echo "  $(COLOR_GREEN)help$(COLOR_RESET)        - Show this help message"
	@echo ""
	@echo "$(COLOR_BOLD)Examples:$(COLOR_RESET)"
	@echo "  make              # Build release version"
	@echo "  make debug        # Build debug version"
	@echo "  make run          # Build and run"
	@echo "  make check-deps   # Verify SDL2 libraries are installed"
