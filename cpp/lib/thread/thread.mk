THREAD_SRC := lib/thread/thread.cpp
THREAD_OBJ := .build/lib/thread/thread.o
THREAD_LIB := .build/lib/thread/thread.a

$(THREAD_OBJ): $(THREAD_SRC) | .build/lib/thread
	$(CXX) $(CXXFLAGS) -Ilib/thread -c $< -o $@

$(THREAD_LIB): $(THREAD_OBJ)
	ar rcs $@ $^

.build/lib/thread:
	@mkdir -p $@