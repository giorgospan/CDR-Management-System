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
OPFILE="./input_files/input2.txt"
CONFILE="./input_files/config_file.txt"

valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $TARGET -h1 37 -h2 37 -s 100000 -o $OPFILE -c $CONFILE
# $TARGET -h1 13 -h2 13 -s 1024 -o $OPFILE -c $CONFILE
