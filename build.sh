#!/bin/bash
g++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
echo "Compiled cpu to bin/cpu"
g++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
echo "Compiled assembler to bin/assembler"