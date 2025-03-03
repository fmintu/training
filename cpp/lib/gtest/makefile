# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -std=c++17

# Directories
GTEST_DIR = googletest/googletest
BUILD_DIR = .build
LIB_DIR = $(BUILD_DIR)/lib
INCLUDE_DIR = $(GTEST_DIR)/include

# Google Test sources
GTEST_SRCS = $(GTEST_DIR)/src/gtest-all.cc
GTEST_MAIN_SRCS = $(GTEST_DIR)/src/gtest_main.cc

# Output libraries
GTEST_LIB = $(LIB_DIR)/libgtest.a
GTEST_MAIN_LIB = $(LIB_DIR)/libgtest_main.a

# Object files
GTEST_OBJS = $(BUILD_DIR)/gtest-all.o
GTEST_MAIN_OBJS = $(BUILD_DIR)/gtest_main.o

# Test executable
TEST_SRC = test.cpp
TEST_EXE = test

# Build Google Test
all: $(GTEST_LIB) $(GTEST_MAIN_LIB) $(TEST_EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR): | $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

$(GTEST_OBJS): $(GTEST_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(GTEST_DIR) -c $(GTEST_SRCS) -o $@

$(GTEST_MAIN_OBJS): $(GTEST_MAIN_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(GTEST_DIR) -c $(GTEST_MAIN_SRCS) -o $@

$(GTEST_LIB): $(GTEST_OBJS) | $(LIB_DIR)
	ar rcs $@ $^

$(GTEST_MAIN_LIB): $(GTEST_MAIN_OBJS) | $(LIB_DIR)
	ar rcs $@ $^

# Compile test executable
$(TEST_EXE): $(TEST_SRC) $(GTEST_LIB) $(GTEST_MAIN_LIB)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< -o $@ $(GTEST_LIB) $(GTEST_MAIN_LIB) -pthread

# code coverage
code_coverage: $(TEST_SRC) $(GTEST_LIB) $(GTEST_MAIN_LIB)
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -I$(INCLUDE_DIR) $< -o $@ $(GTEST_LIB) $(GTEST_MAIN_LIB) -pthread

# Clean build files
clear_test:
	rm -rf $(TEST_EXE)

clean:
	rm -rf $(BUILD_DIR) $(TEST_EXE)

.PHONY: all clean
