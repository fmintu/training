# ===== CONFIG =====
TARGET      := bin1
BUILD_DIR   := ../../build/$(TARGET)
OUT         := $(BUILD_DIR)/$(TARGET)

SRC         := main.cpp \
               ../../common/util.cpp \
               ../../lib/math/math.cpp

INC         := -I../../common -I../../lib/math

CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Wextra $(INC)

# ===== RULES =====
all: $(OUT)

$(BUILD_DIR):
	@mkdir -p $@

$(OUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OUT)