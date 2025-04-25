SHAPE_SRC := lib/shape/shape.cpp
SHAPE_OBJ := build/pkg/shape/shape.o
SHAPE_LIB := build/pkg/shape/shape.a

$(SHAPE_OBJ): $(SHAPE_SRC) | build/pkg/shape
	$(CXX) $(CXXFLAGS) -Ilib/shape -c $< -o $@

$(SHAPE_LIB): $(SHAPE_OBJ)
	ar rcs $@ $^

build/pkg/shape:
	@mkdir -p $@