# ===== CONFIG =====
TARGET      := bin2
BUILD_DIR   := ../../build/$(TARGET)
OUT         := $(BUILD_DIR)/$(TARGET)

SRC         := main.cpp
INC         := -I../../lib/shape
LIB_DIR     := -L../../lib/shape
LIBS        := -l:shape.a

CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Wextra $(INC)
LDFLAGS     := $(LIB_DIR) $(LIBS)

# ===== RULES =====
all: $(OUT)

$(BUILD_DIR):
	@mkdir -p $@

$(OUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OUT)