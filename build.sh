#!/bin/bash

# This script compiles the cpu emulator and assembler.
# They are compiled to the bin/ directory.
# Use "chmod +x build.sh" if you're getting permission denied.

# Create bin directory if it does not exist
# Dont print the "it already exists" error
mkdir bin 2>/dev/null

# Remove the the existing execs just because
rm bin/cpu.exe 2>/dev/null
rm bin/assembler.exe 2>/dev/null

# Compile to bin/cpu
g++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
echo "Compiled cpu to bin/cpu"

# Compile to bin/assembler
g++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
echo "Compiled assembler to bin/assembler"