#include "instr.hpp"
#include "assembler.hpp"

int main(int argc, char* argv[]) {
    if(argv[1] == nullptr) {
        std::cout << "\nERROR: Please enter a valid filename along with the"
                  << " executable in the command line. " 
                  << "\ni.e. bin/run filename\n" 
                  << "P.S. The test assembly files are in programs/  the ones WITHOUT .inst\n" << std::endl;
            return 1;
    }
    std::cout << std::endl;
    std::string filename = argv[1];
    std::ostringstream machine_code = gen_machine_code(filename);
    std::string ofilename = filename + ".inst";
    std::ofstream out_file(ofilename);
    out_file << machine_code.str();
    std::cout << "Program Compiled Successfully. Made: " << ofilename << std::endl;
    out_file.close();
    if(argv[2] != nullptr) {
        parse_file_into_file(ofilename);
    }
    CPU cpu;
    if(!parse_file(ofilename, &cpu.mem[0], cpu.next_free_location, cpu.end_text_section)) {
        std::cout << "Error opening file: " << filename << std::endl;
    }
    std::cout << std::endl;
    cpu.run();
    std::cout << "\nProgram Exited Successfully :)" << std::endl;
    std::cout << std::endl;
    return 0;
}