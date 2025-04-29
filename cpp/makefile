# ===== Compiler and Flags =====
CXX := g++
INCLUDES := -Icommon -Ilib/math -Ilib/shape
# LIB_PATHS := -Lbuild/common/util -Lbuild/lib/math -Lbuild/lib/shape -Lbuild/lib/thread
CXXFLAGS := -std=c++17 -Wall -Wextra $(INCLUDES)

# ===== Include packets =====
include pkg/gtest.mk
include pkg/json.mk
# ===== Include modules =====
include common/util.mk
include lib/math/math.mk
include lib/shape/shape.mk
include lib/shape/shape.mk
include lib/thread/thread.mk
# ===== Include binaries =====
include bin/bin1/bin1.mk
include bin/bin2/bin2.mk
include bin/bin3/bin3.mk
include bin/bin4/bin4.mk

# ===== Targets =====
ALL_PKGS := json  #pkg_gtest

ALL_BINS := $(BIN1_OUT) $(BIN2_OUT) $(BIN3_OUT) $(BIN4_OUT)
ALL_LIBS := $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB) $(THREAD_LIB)


.PHONY: all clean build_pkg clear_pkg

all: $(ALL_PKGS) $(ALL_LIBS) $(ALL_BINS)

clean:
	rm -rf .build/*