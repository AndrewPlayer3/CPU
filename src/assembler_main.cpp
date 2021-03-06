/*
    Andrew Player 2019
*/

#include "assembler.hpp"

/* This is an assembler that compiles a custom assembly into machine */
/* code for the cpu emulator.                                        */


int main(int argc, char* argv[]) {
    if(argv[1] == nullptr) {
        std::cout << "\nERROR: Please enter a valid filename along with the"
                  << " executable in the command line. " 
                  << "\ni.e. bin/assembler filename\n" 
                  << "P.S. The test assembly files are in programs/  the ones WITHOUT .inst\n" << std::endl;
            return 1;
    }
    std::cout << std::endl;
    for(int i = 1; argv[i] != nullptr; i++) {
        std::string filename = argv[i];
        std::ostringstream machine_code = gen_machine_code(filename);
        std::string ofilename = filename + ".inst";
        std::ofstream out_file(ofilename);
        out_file << machine_code.str();
        std::cout << "Program Compiled Successfully. Made: " << ofilename << std::endl;
    }
    std::cout << std::endl;
    return 0;
}