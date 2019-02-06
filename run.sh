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


# valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $TARGET -h1 10 -h2 10 -s 56 -o $OPFILE -c $CONFILE
$TARGET -h1 113 -h2 113 -s 4000000 -o $OPFILE -c $CONFILE
