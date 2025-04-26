SHAPE_SRC := lib/shape/shape.cpp
SHAPE_OBJ := build/lib/shape/shape.o
SHAPE_LIB := build/lib/shape/shape.a

$(SHAPE_OBJ): $(SHAPE_SRC) | build/lib/shape
	$(CXX) $(CXXFLAGS) -Ilib/shape -c $< -o $@

$(SHAPE_LIB): $(SHAPE_OBJ)
	ar rcs $@ $^

build/lib/shape:
	@mkdir -p $@