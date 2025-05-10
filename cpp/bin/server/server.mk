# ===== Config =====
BUILD_DIR   := .build/bin/server
SRC_DIR     := bin/server
SERVER_TARGET    := $(BUILD_DIR)/server

# Source files
SERVER_SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
SERVER_OBJ := $(SERVER_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

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
SERVER_TARGET: $(SERVER_TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

clean:
	rm -rf $(BUILD_DIR)