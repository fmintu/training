BUILD_DIR := .build/bin/bin4
BIN4_SRC := bin/bin4/main.cpp

BIN4_OBJ := $(BUILD_DIR)/main.o
BIN4_OUT := $(BUILD_DIR)/bin4
LIB_PATHS := -L.build/common/util -L.build/lib/math -L.build/lib/shape -L.build/lib/thread
LINK_LIBS := -l:util.a -l:math.a -l:shape.a -l:thread.a
INC_PATHS := -I.build/pkg/json


CXXFLAGS += -std=c++17 -Wall -Wextra $(INC_PATHS)

$(BIN4_OBJ): $(BIN4_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN4_OUT): $(BIN4_OBJ) $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

$(BUILD_DIR):
	@mkdir -p $@