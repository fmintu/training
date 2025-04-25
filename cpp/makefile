# ===== Compiler and Flags =====
CXX := g++
INCLUDES := -Icommon -Ilib/math -Ilib/shape
LIB_PATHS := -Lbuild/common -Lbuild/pkg/math -Lbuild/pkg/shape
CXXFLAGS := -std=c++17 -Wall -Wextra $(INCLUDES)

# ===== Include modules =====
include common/util.mk
include lib/math/math.mk
include lib/shape/shape.mk

# ===== Include binaries =====
include bin/bin1/bin1.mk
include bin/bin2/bin2.mk
include bin/bin3/bin3.mk
include bin/bin4/bin4.mk

# ===== Targets =====
ALL_BINS := $(BIN1_OUT) $(BIN2_OUT) $(BIN3_OUT) $(BIN4_OUT)

.PHONY: all clean

all: $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB) $(ALL_BINS)

clean:
	rm -rf build/*