# ===== Config =====
BUILD_DIR   := .build/bin/bin4
SRC_DIR     := bin/bin4
BIN4_TARGET    := $(BUILD_DIR)/bin4

# Source files
BIN4_SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
BIN4_OBJ := $(BIN4_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Include paths
INC_PATHS := -I.build/pkg/json -I$(SRC_DIR) -Icommon -Ilib

# Library linking
LIB_PATHS := -L.build/common/util
LIB_PATHS += -L.build/lib/math
LIB_PATHS += -L.build/lib/shape
LIB_PATHS += -L.build/lib/thread
LIB_PATHS += -L.build/pkg/sqlite

LINK_LIBS := -l:util.a -l:math.a -l:shape.a -l:thread.a -l:libsqlite.a

# Compiler flags
CXXFLAGS += -std=c++17 -Wall -Wextra $(INC_PATHS)

# ===== Rules =====
BIN4_TARGET: $(BIN4_TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN4_TARGET): $(BIN4_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

clean:
	rm -rf $(BUILD_DIR)