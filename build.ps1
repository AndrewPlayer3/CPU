# This script compiles the cpu emulator and assembler.
# They are compiled to the bin/ directory.
# Will require allowing execution of unsigned scripts.

# Create bin directory if it does not exist
# Dont print the "it already exists" error
mkdir bin -erroraction 'silentlycontinue' | Out-Null

# Remove the the existing execs just because
# Also doesn't print out their errors.
Remove-Item bin/cpu.exe -erroraction 'silentlycontinue'
Remove-Item bin/assembler.exe -erroraction 'silentlycontinue'
Remove-Item bin/run.exe -erroraction 'silentlycontinue'

# New Line
Write-Output ""
 
# Compile to bin/cpu
c++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
Write-Output "Compiled cpu to bin/cpu"

# Compile to bin/assembler
c++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
Write-Output "Compiled assembler to bin/assembler"

# Compile to bin/run
c++ -std=c++17 -Wall -O3 src/main.cpp src/instr.cpp src/assembler.cpp -o bin/run
Write-Output "Compiled build+run to bin/run"

# Compile to bin/ish
c++ -std=c++17 -Wall -O3 src/shell.cpp src/instr.cpp src/assembler.cpp -o bin/ish
Write-Output "Compiled test shell to bin/ish"

# Print some instructions
Write-Output ""
Write-Output "To assemble the test files use this: 'bin/assembler filename'"
Write-Output "To run the test files use this: 'bin/cpu filename'"
Write-Output "To assemble and run use this: 'bin/run filename\'"
Write-Output "Note: the cpu only runs the .inst files. These are what the assembler generates."
Write-Output "Check the programs folder for the test programs."
Write-Output ""
