#!/bin/bash

echo "==========="
echo "Compilation"
echo "==========="
echo

make


echo "========="
echo "Execution"
echo "========="
echo

TARGET="./build/werhauz"
OPFILE="./input_files/input6.txt"
CONFILE="./input_files/config_file.txt"

# valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $TARGET -h1 1 -h2 13 -s 80000 -o $OPFILE -c $CONFILE
$TARGET -h1 101 -h2 101 -s 100000 -o $OPFILE -c $CONFILE
