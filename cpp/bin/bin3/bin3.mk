# ========== CONFIG ==========
TARGET      := bin3
BUILD_DIR   := ../../build/$(TARGET)
OUT         := $(BUILD_DIR)/$(TARGET)

SRC         := main.cpp \
               ../../common/util.cpp \
               ../../lib/math/math.cpp

INC         := -I../../common \
							 -I../../lib/math \
							 -I../../lib/shape \

LIB_DIR     := -L../../lib/shape
LIBS        := -l:shape.a

CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Wextra $(INC)
LDFLAGS     := $(LIB_DIR) $(LIBS)

# ========== BUILD RULES ==========

all: $(OUT)

$(BUILD_DIR):
	@mkdir -p $@

$(OUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OUT)