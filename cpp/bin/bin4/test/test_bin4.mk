# === CONFIGURATION ===
TARGET        := test_bin4
BUILD_DIR     := ../../../build/bin4
OUT           := $(BUILD_DIR)/$(TARGET)

SRC           := test_main.cpp test_shape.cpp ../../../lib/shape/shape.cpp
INC           := -I../../../lib/shape \
								 -I../../../pkg/gtest/googletest/googletest/include \

LIB_DIRS      := -L../../../build/gtest
LIBS          := -lgtest -lgtest_main -lpthread

CXX           := g++
CXXFLAGS      := -std=c++17 -Wall -Wextra $(INC)
LDFLAGS       := $(LIB_DIRS) $(LIBS)

# === RULES ===
.PHONY: test_bin4 clean_test_bin4 run

test_bin4: $(OUT)

$(BUILD_DIR):
	@mkdir -p $@

$(OUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(OUT)
	@echo "Running unit tests..."
	@./$(OUT)

clean_test_bin4:
	rm -f $(OUT)