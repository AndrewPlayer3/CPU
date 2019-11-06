/*
INSTR Hex Instruction Set
Andrew, Robert, Gannon
October 18, 2019
*/

#include "instr.hpp"

int  main() {
    
    CPU cpu;
    
    std::string filename;
    std::cout << "Filename: ";
    std::cin >> filename;
    
    parse_file(cpu, filename);
    
    std::cout << std::endl;
    
    cpu.run();
    
    std::cout << "\nProgram Exited Successfully" << std::endl;
    std::cout << std::endl;
    
    return 0;
}