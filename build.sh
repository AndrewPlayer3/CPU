#!/bin/sh

# This script compiles the cpu emulator and assembler.
# They are compiled to the bin/ directory.
# Use "chmod +x build.sh" if you're getting permission denied.

# Create bin directory if it does not exist
# Dont print the "it already exists" error
mkdir bin 2>/dev/null

# Remove the the existing execs just because
rm bin/cpu.exe 2>/dev/null
rm bin/assembler.exe 2>/dev/null
rm bin/run.exe 2>/dev/null
rm bin/ish.exe 2>/dev/null

# New Line
echo ""

# Compile to bin/cpu
c++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
echo "Compiled cpu to bin/cpu"

# Compile to bin/assembler
c++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
echo "Compiled assembler to bin/assembler"

# Compile to bin/run
c++ -std=c++17 -Wall -O3 src/main.cpp src/instr.cpp src/assembler.cpp -o bin/run
echo "Compiled build+run to bin/run"

# Compile to bin/run
c++ -std=c++17 -Wall -O3 src/shell.cpp src/instr.cpp src/assembler.cpp -o bin/ish
echo "Compiled test shell to bin/ish"

# Compile to bin/test -- secret testing file
#c++ -std=c++17 -Wall -O3 src/testing.cpp src/instr.cpp src/assembler.cpp -o bin/test

# Print some instructions
echo ""
echo "To assemble the test files use this: \"bin/assembler filename\""
echo "To run the test files use this: \"bin/cpu filename.inst\""
echo "To assemble and run use this: \"bin/run filename\""
echo "Note: the cpu only runs the .inst files. These are what the assembler generates"
echo "Check the programs folder for the test programs."
echo ""
