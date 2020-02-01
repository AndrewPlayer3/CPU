#include "instr.hpp"
#include "assembler.hpp"

/*
void CPU::run() {
    int prog_end = next_free_location;
    while(mem[regs[PCTR]] != 0xFF00 && regs[PCTR] < prog_end) regs[PCTR]++;
    regs[PCTR]++;
    while(regs[PCTR] < prog_end) {
        exec(mem[regs[PCTR]++]);
    }
}
*/

int main() {

    CPU cpu;

    while(true) {
        std::cout << ">> ";
        std::string input = "";
        std::getline(std::cin, input);
        if(input == "quit()") {
            break;
        } else if(input == "r") {
            cpu.reg_dump();
            continue;
        } else if(input == "m") {
            cpu.mem_dump(cpu.next_free_location + 1);
            continue;
        } else if(input == "") {
            continue;
        } else if(input == "n") {
            cpu.regs[PCTR]++;
            continue;
        } else if(input == "b") {
	        cpu.regs[PCTR]--;
	        continue;
        } else if(input == "d") {
            cpu.exec(cpu.mem[cpu.regs[PCTR]++]);
            continue;
        }
        std::ostringstream os;
        os << input << '\n';
        std::ostringstream instruction = gen_code_from_line(input);
        std::string inst_string = instruction.str();
        parse_block(inst_string, &cpu.mem[0], cpu.next_free_location, cpu.end_text_section);
        cpu.exec(cpu.mem[cpu.regs[PCTR]++]);
    }
    
    std::cout << std::endl;
    std::cout << "Successful..." << std::endl;
    std::cout << std::endl;

    return 0;
}
