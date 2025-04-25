BIN1_SRC := bin/bin1/main.cpp
BIN1_OBJ := build/bin1/main.o
BIN1_OUT := build/bin1/bin1

$(BIN1_OBJ): $(BIN1_SRC) | build/bin1
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN1_OUT): $(BIN1_OBJ) $(UTIL_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB_PATHS) -l:util.a -l:math.a

build/bin1:
	@mkdir -p $@