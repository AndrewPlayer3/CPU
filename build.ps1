g++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
Write-Output "Compiled cpu to bin/cpu"
g++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
Write-Output "Compiled assembler to bin/assembler"