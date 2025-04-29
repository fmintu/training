BUILD_DIR := .build/bin/bin2
BIN2_SRC := bin/bin2/main.cpp
BIN2_OBJ := $(BUILD_DIR)/main.o
BIN2_OUT := $(BUILD_DIR)/bin2
LIB_PATHS := -L.build/common/util -L.build/lib/shape
LINK_LIBS := -l:util.a -l:shape.a

$(BIN2_OBJ): $(BIN2_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN2_OUT): $(BIN2_OBJ) $(UTIL_LIB) $(SHAPE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

$(BUILD_DIR):
	@mkdir -p $@


