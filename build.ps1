# This script compiles the cpu emulator and assembler.
# They are compiled to the bin/ directory.
# Will require allowing execution of unsigned scripts.

# Create bin directory if it does not exist
# Dont print the "it already exists" error
mkdir bin -erroraction 'silentlycontinue'

# Remove the the existing execs just because
Remove-Item bin/cpu.exe
Remove-Item bin/assembler.exe

# Compile to bin/cpu
g++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
Write-Output "Compiled cpu to bin/cpu"

# Compile to bin/assembler
g++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
Write-Output "Compiled assembler to bin/assembler"