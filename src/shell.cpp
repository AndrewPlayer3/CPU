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
        } else if(input == "h"){
            std::cout << "r - reg dump" << std::endl;
            std::cout << "m - mem dump" << std::endl;
            std::cout << "n - pctr++"   << std::endl;
            std::cout << "b - pctr--"   << std::endl;
            std::cout << "d - cpu.exec(cpu.mem[cpu.regs[PCTR]++])" << std::endl;
            std::cout << "s - stck dump"<< std::endl;
            std::cout << "p - pctr = input" << std::endl;
            std::cout << "c - continue for i iterations" << std::endl;
            std::cout << "f - next_free_location" << std::endl;
            continue;
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
            std::cout << "Set Program Counter: ";
            std::cin >> std::hex >> new_value;
            cpu.regs[PCTR] = new_value;
            std::cin.clear();
            std::cin.ignore(INT8_MAX, '\n');
            continue;
        } else if(input == "c") {
            int end;
            std::cout << "Continue Until: ";
            std::cin >> std::hex >> end;
            std::cin.clear();
            std::cin.ignore(INT8_MAX, '\n');
            for(int i = 0; i < end; i++) {
                cpu.exec(cpu.mem[cpu.regs[PCTR]++]);
            }
            continue;
        } else if(input == "f") {
            std::cout << std::hex << cpu.next_free_location << std::endl;
            continue;
        } else if(input == "ets") {
            std::cout << cpu.end_text_section << std::endl;
            continue;
        } else if(input == "run") {
            cpu.loaded = true;
            cpu.run();
            cpu.loaded = false;
            cpu.regs[PCTR] = cpu.next_free_location;
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
