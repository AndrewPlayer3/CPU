#include "instr.hpp"
#include "assembler.hpp"

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
        } else if(input == "s") {
            cpu.stack_dump();
            continue;
        } else if(input == "p") {
            int new_value;
            std::cout << ">> ";
            std::cin >> std::hex >> new_value;
            cpu.regs[PCTR] = new_value;
            std::cin.clear();
            std::cin.ignore(INT8_MAX, '\n');
            continue;
        } else if(input == "c") {
            int end;
            std::cout << ">> ";
            std::cin >> std::hex >> end;
            std::cin.clear();
            std::cin.ignore(INT8_MAX, '\n');
            for(int i = 0; i < end; i++) {
                cpu.exec(cpu.mem[cpu.regs[PCTR]++]);
            }
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
