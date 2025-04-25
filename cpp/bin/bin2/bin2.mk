BIN2_SRC := bin/bin2/main.cpp
BIN2_OBJ := build/bin2/main.o
BIN2_OUT := build/bin2/bin2

$(BIN2_OBJ): $(BIN2_SRC) | build/bin2
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN2_OUT): $(BIN2_OBJ) $(UTIL_LIB) $(SHAPE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) -l:util.a -l:shape.a

build/bin2:
	@mkdir -p $@