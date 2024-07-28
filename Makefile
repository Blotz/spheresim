# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/python2.7 -lpython2.7 -Ieigen-3.4.0
DEBUGFLAGS := -g

# Directories
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests
LIB_DIR := lib

# Sources and objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# Executable names
TARGET := spheresim
DEBUG_TARGET := spheresim_debug
TEST_TARGET := spheresim_test

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Build target
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build debug target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(BUILD_DIR)/$(DEBUG_TARGET)

$(BUILD_DIR)/$(DEBUG_TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build test target
test: $(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*

# Phony targets
.PHONY: all debug test clean
