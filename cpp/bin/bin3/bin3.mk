BIN3_SRC := bin/bin3/main.cpp
BIN3_OBJ := build/bin3/main.o
BIN3_OUT := build/bin3/bin3

$(BIN3_OBJ): $(BIN3_SRC) | build/bin3
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN3_OUT): $(BIN3_OBJ) $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) -l:util.a -l:math.a -l:shape.a

build/bin3:
	@mkdir -p $@