# ===== CONFIG =====
BUILD_DIR   := .build/bin/bin3
SRC_DIR     := bin/bin3


# Source files
BIN3_SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
BIN3_OBJ := $(BIN3_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
BIN3_TARGET    := $(BUILD_DIR)/bin3

# Include paths
INC_PATHS   := -I.build/pkg/json -Icommon -Ilib

# Library paths & links
LIB_PATHS   := -L.build/common/util -L.build/lib/math -L.build/lib/shape -L.build/lib/thread
LINK_LIBS   := -l:util.a -l:math.a -l:shape.a -l:thread.a

# Compiler flags
CXXFLAGS   += -std=c++17 -Wall -Wextra $(INC_PATHS)

# ===== RULES =====
BIN3_TARGET: $(BIN3_TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(BIN3_OBJ): $(BIN3_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN3_TARGET): $(BIN3_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

clean:
	rm -rf $(BUILD_DIR)