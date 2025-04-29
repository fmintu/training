BUILD_DIR := .build/bin/bin1
BIN1_SRC := bin/bin1/main.cpp
BIN1_OBJ := $(BUILD_DIR)/main.o
BIN1_TARGET := $(BUILD_DIR)/bin1
LIB_PATHS := -L.build/common/util -L.build/lib/math
LINK_LIBS := -l:util.a -l:math.a

$(BIN1_OBJ): $(BIN1_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN1_TARGET): $(BIN1_OBJ) $(UTIL_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) $(LINK_LIBS)

$(BUILD_DIR):
	@mkdir -p $@


