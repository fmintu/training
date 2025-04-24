# ===== CONFIG =====
TARGET       := shape
SRC_DIR      := .
BUILD_DIR    := ../../build/pkg/$(TARGET)
SRC          := shape.cpp
OBJ          := $(BUILD_DIR)/shape.o
OUT          := $(BUILD_DIR)/$(TARGET).a

CXX          := g++
CXXFLAGS     := -std=c++17 -Wall -Wextra -I$(SRC_DIR)
AR           := ar
ARFLAGS      := rcs

# ===== RULES =====

$(BUILD_DIR):
	@mkdir -p $@

$(OBJ): $(SRC_DIR)/shape.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

pkg_shape: $(OUT)

# clean:
# 	rm -rf $(BUILD_DIR)

.PHONY: all clean