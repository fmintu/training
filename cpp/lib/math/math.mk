MATH_SRC := lib/math/math.cpp
MATH_OBJ := build/pkg/math/math.o
MATH_LIB := build/pkg/math/math.a

$(MATH_OBJ): $(MATH_SRC) | build/pkg/math
	$(CXX) $(CXXFLAGS) -Ilib/math -c $< -o $@

$(MATH_LIB): $(MATH_OBJ)
	ar rcs $@ $^

build/pkg/math:
	@mkdir -p $@