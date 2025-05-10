# ===== CONFIG =====
BUILD_DIR   := .build/bin/client
SRC_DIR     := bin/client


# Source files
CLIENT_SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
CLIENT_OBJ := $(CLIENT_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CLIENT_TARGET    := $(BUILD_DIR)/client

# Include paths
INC_PATHS   := -I.build/pkg/json -Icommon -Ilib

# Library paths & links
LIB_PATHS   := -L.build/common/util -L.build/lib/math -L.build/lib/shape -L.build/lib/thread
LINK_LIBS   := -l:util.a -l:math.a -l:shape.a -l:thread.a

# Compiler flags
CXXFLAGS   += -std=c++17 -Wall -Wextra $(INC_PATHS)

# ===== RULES =====
CLIENT_TARGET: $(CLIENT_TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(CLIENT_OBJ): $(CLIENT_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

clean:
	rm -rf $(BUILD_DIR)