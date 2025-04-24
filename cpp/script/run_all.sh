#!/bin/bash

set -e #

echo "=== Building bin1 ==="
make bin1
echo "Running bin1:"
./build/bin1/bin1
echo ""

echo "=== Building bin2 ==="
make bin2
echo "Running bin2:"
./build/bin2/bin2
echo ""

echo "=== Building bin3 ==="
make bin3
echo "Running bin3:"
./build/bin3/bin3
echo ""

echo "=== Building bin4 ==="
make bin4
echo "Running bin4:"
./build/bin4/bin4
echo ""

echo "=== Building and Running Test: test_bin4 ==="
make -C bin/bin4/test
make -C bin/bin4/test run
