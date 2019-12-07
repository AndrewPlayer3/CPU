/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
DOESN'T WORK WITH 32-BIT MINGW/32-BIT GCC
*/

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu   */
/* refer to the readme for a list of the instructions                    */
/* The machine code for this cpu can be generated from an assembly using */
/* the included assembler. (Note: that is a seperate program)            */

#include "instr.hpp"

int  main(int argc, char* argv[]) {

    CPU cpu;

    if(argv[1] == nullptr) {
        std::cout << "ERROR: Please enter a valid filename along with the"
            << " executable in the command line. \ni.e. ./executable filename" << std::endl;
            return 1;
    }

    std::string filename = argv[1];
    if(!parse_file(filename, &cpu.mem[0], cpu.next_free_location)) {
        std::cout << "Error opening file: " << filename << std::endl;
    }
    
    std::cout << std::endl;
    
    cpu.run();
    
    std::cout << "\nProgram Exited Successfully :)" << std::endl;
    std::cout << std::endl;

    return 0;
}