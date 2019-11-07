/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
*/

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu  */
/* refer to the readme for a list of the instructions                   */

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