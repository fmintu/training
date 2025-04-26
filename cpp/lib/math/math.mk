MATH_SRC := lib/math/math.cpp
MATH_OBJ := build/lib/math/math.o
MATH_LIB := build/lib/math/math.a

$(MATH_OBJ): $(MATH_SRC) | build/lib/math
	$(CXX) $(CXXFLAGS) -Ilib/math -c $< -o $@

$(MATH_LIB): $(MATH_OBJ)
	ar rcs $@ $^

build/lib/math:
	@mkdir -p $@