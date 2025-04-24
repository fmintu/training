# ===== CONFIG =====
TARGET       := util
SRC_DIR      := .
BUILD_DIR    := ../build/common/util
SRC          := util.cpp
OBJ          := $(BUILD_DIR)/$(TARGET).o

CXX          := g++
CXXFLAGS     := -std=c++17 -Wall -Wextra -I$(SRC_DIR)

# ===== RULES =====
$(BUILD_DIR):
	@mkdir -p $@

$(OBJ): $(SRC_DIR)/$(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(OBJ)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean