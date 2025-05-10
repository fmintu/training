# ===== Compiler and Flags =====
CXX := g++
INCLUDES := -Icommon -Ilib/math -Ilib/shape
# LIB_PATHS := -Lbuild/common/util -Lbuild/lib/math -Lbuild/lib/shape -Lbuild/lib/thread
CXXFLAGS := -std=c++17 -Wall -Wextra $(INCLUDES)

# ===== Include packets =====
include pkg/gtest.mk
include pkg/json.mk
include pkg/sqlite.mk
# ===== Include modules =====
include common/util.mk
include lib/math/math.mk
include lib/shape/shape.mk
include lib/shape/shape.mk
include lib/thread/thread.mk
# ===== Include binaries =====
include bin/bin1/bin1.mk
include bin/bin2/bin2.mk
include bin/client/client.mk
include bin/server/server.mk

# ===== Targets =====
ALL_PKGS := json pkg_sqlite #pkg_gtest

ALL_BINS := $(BIN1_TARGET) $(BIN2_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET)
ALL_LIBS := $(UTIL_LIB) $(MATH_LIB) $(SHAPE_LIB) $(THREAD_LIB)


.PHONY: all clean build_pkg clear_pkg

all: $(ALL_PKGS) $(ALL_LIBS) $(ALL_BINS)

clean:
	rm -rf .build/*