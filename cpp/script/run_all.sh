#!/bin/bash

set -e #

make all

# echo "Running bin1:"
# ./build/bin/bin1/bin1
# echo ""

# echo "Running bin2:"
# ./build/bin/bin2/bin2
# echo ""

echo "Running bin4:"
./.build/bin/bin4/bin4
echo ""

echo "Running bin3:"
./.build/bin/bin3/bin3
echo ""
