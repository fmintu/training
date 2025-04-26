BUILD_DIR := build/bin/bin3
BIN3_SRC := bin/bin3/main.cpp
BIN3_OBJ := $(BUILD_DIR)/main.o
BIN3_OUT := $(BUILD_DIR)/bin3
LIB_PATHS := -Lbuild/common/util -Lbuild/lib/math -Lbuild/lib/shape
LINK_LIBS := -l:util.a -l:math.a -l:shape.a -l:thread.a

$(BIN3_OBJ): $(BIN3_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN3_OUT): $(BIN3_OBJ) $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

$(BUILD_DIR):
	@mkdir -p $@