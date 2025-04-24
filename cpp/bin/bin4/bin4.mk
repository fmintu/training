# include test/test_bin4.mk

.PHONY: all clean test_bin4 clean_test_bin4
# ===== CONFIG =====
TARGET        := bin4
BUILD_DIR     := ../../build/$(TARGET)
OUT           := $(BUILD_DIR)/$(TARGET)

SRC           := main.cpp ../../common/util.cpp ../../lib/math/math.cpp
INC           := -I../../common \
								 -I../../lib/math \
								 -I../../lib/shape \

LIB_DIRS      := -L../../lib/shape
LIBS          := -l:shape.a

CXX           := g++
CXXFLAGS      := -std=c++17 -Wall -Wextra $(INC)
LDFLAGS       := $(LIB_DIRS) $(LIBS)

$(BUILD_DIR):
	@mkdir -p $@

all: $(OUT)

$(OUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

test_bin4:
	$(MAKE) -C test -f test_bin4.mk test_bin4

clean_test_bin4:
	$(MAKE) -C test -f test_bin4.mk clean_test_bin4

clean:
	rm -f $(OUT)



