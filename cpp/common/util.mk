UTIL_SRC := common/util.cpp
UTIL_OBJ := build/common/util/util.o
UTIL_LIB := build/common/util.a

$(UTIL_OBJ): $(UTIL_SRC) | build/common/util
	$(CXX) $(CXXFLAGS) -Icommon -c $< -o $@

$(UTIL_LIB): $(UTIL_OBJ)
	ar rcs $@ $^

build/common/util:
	@mkdir -p $@