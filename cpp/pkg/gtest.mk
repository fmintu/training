# === CONFIGURATION ===
GTEST_SRC_DIR := pkg/gtest/googletest
GTEST_BUILD_DIR := $(GTEST_SRC_DIR)/build
GTEST_OUTPUT_DIR := build/pkg/gtest

.PHONY: gtest gtest-clean

# === gtest: clone + build + copy ===
gtest:
	@if [ ! -d "$(GTEST_SRC_DIR)" ]; then \
		echo "Cloning GoogleTest..."; \
		git clone https://github.com/google/googletest.git $(GTEST_SRC_DIR); \
	fi
	@mkdir -p $(GTEST_BUILD_DIR)
	cd $(GTEST_BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_GMOCK=ON -DBUILD_GTEST=ON
	$(MAKE) -C $(GTEST_BUILD_DIR) -j$(shell nproc)
	@mkdir -p $(GTEST_OUTPUT_DIR)
	cp $(GTEST_BUILD_DIR)/lib/*.a $(GTEST_OUTPUT_DIR)

# === gtest-clean: remove build artifacts ===
gtest-clean:
	rm -rf $(GTEST_BUILD_DIR)
	rm -rf $(GTEST_OUTPUT_DIR)