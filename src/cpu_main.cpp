/*
    Andrew Player 2019
*/

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu   */
/* refer to the readme for a list of the instructions                    */
/* The machine code for this cpu can be generated from an assembly using */
/* the included assembler. (Note: that is a seperate program)            */

#include "instr.hpp"

int main(int argc, char *argv[])
{

    CPU cpu;

    if (argv[1] == nullptr)
    {
        std::cout << "\nERROR: Please enter a valid filename along with the"
                  << " executable in the command line. "
                  << "\ni.e. bin/cpu filename\n"
                  << "P.S. The test machine code files are in programs/  the ones WITH .inst\n\n"
                  << std::endl;
        exit(1);
    }

    std::string filename = argv[1];
    if (filename.substr(filename.size() - 5, 5) != ".inst")
    {
        std::cout << "\nWARNING: File is not a .inst file! Are you running the compiled version?\n"
                  << std::endl;
    }

    if (!parse_file(filename, &cpu.mem[0], cpu.next_free_location, cpu.end_text_section))
    {
        std::cout << "Error opening file: " << filename << std::endl;
    }

    std::cout << std::endl;

    cpu.loaded = true;

    cpu.run();

    std::cout << "\nProgram Exited Successfully :)" << std::endl;
    std::cout << std::endl;

    return 0;
}