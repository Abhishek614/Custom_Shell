# ================================
# Custom Shell - Capstone Project
# Author: Abhishek Mohapatra
# Registration Number: 2241013303
# Batch: 11
# ================================

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Executable name
TARGET = myshell

# Source directory and files
SRC_DIR = src
SRC = $(SRC_DIR)/main.cpp \
      $(SRC_DIR)/parser.cpp \
      $(SRC_DIR)/executor.cpp \
      $(SRC_DIR)/job_control.cpp

# Build directory
BUILD_DIR = build

# Output binary path
OUT = $(BUILD_DIR)/$(TARGET)

# Default rule
all: $(BUILD_DIR) $(OUT)

# Build target
$(OUT): $(SRC)
	@echo "🔧 Compiling Custom Shell..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)
	@echo "✅ Build complete: $(OUT)"

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the shell
run: all
	@echo "🚀 Launching Custom Shell..."
	./$(OUT)

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(BUILD_DIR)
	@echo "✨ Clean complete."



