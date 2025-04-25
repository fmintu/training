BIN4_SRC := bin/bin4/main.cpp
BIN4_OBJ := build/bin4/main.o
BIN4_OUT := build/bin4/bin4

$(BIN4_OBJ): $(BIN4_SRC) | build/bin4
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN4_OUT): $(BIN4_OBJ) $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) -l:util.a -l:math.a -l:shape.a

build/bin4:
	@mkdir -p $@