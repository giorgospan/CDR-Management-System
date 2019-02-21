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
OPFILE="./input_files/input3.txt"
CONFILE="./input_files/config_file.txt"

# valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $TARGET -h1 10 -h2 10 -s 1024 -o $OPFILE
$TARGET -h1 10 -h2 10 -s 1024 -o $OPFILE -c $CONFILE
