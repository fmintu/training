.PHONY: all clean bin1 bin2 bin3 bin4 fmt gtest test_bin4

# ========== INCLUDING  ==========
include pkg/gtest.mk
include pkg/fmt.mk

# ========== PACKET  ==========
gtest: gtest

gtest_clean:
	$(MAKE) -C pkg -f gtest.mk gtest-clean

# ========== BUILD BINARIES ==========

all: bin1 bin2 bin3 bin4

bin1:
	$(MAKE) -C bin/bin1 -f bin1.mk all

bin2:
	$(MAKE) -C bin/bin2 -f bin2.mk all

bin3:
	$(MAKE) -C bin/bin3 -f bin3.mk all

bin4:
	$(MAKE) -C bin/bin4 -f bin4.mk all

test_bin4: gtest
	$(MAKE) -C bin/bin4 -f bin4.mk test_bin4

# ========== CLEAN ==========

clean: gtest_clean
	$(MAKE) -C bin/bin1 -f bin1.mk clean
	$(MAKE) -C bin/bin2 -f bin2.mk clean
	$(MAKE) -C bin/bin3 -f bin3.mk clean
	$(MAKE) -C bin/bin4 -f bin4.mk clean

clean_test:
	$(MAKE) -C bin/bin4 -f bin4.mk clean_test_bin4

