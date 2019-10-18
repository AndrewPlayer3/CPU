#include "instr.hpp"

bool CPU::arithmetic(int B, int C, int D) {
    switch(B) {
        case 0x0:
            regs[C] += regs[D];
            break;
        case 0x1:
            regs[C] -= regs[D];
            break;
        case 0x2:
            regs[C] *= regs[D];
            break;
        case 0x3:
            regs[C] /= regs[D];
            break;
        default:
            return false;
    }

    return true;
}

bool CPU::conditionals(int B, int C, int D) {
    switch(B) {
        case 0x0:
            return regs[C] == regs[D];
            break;
        case 0x1:
            return regs[C] != regs[D];
            break;
        case 0x2:
            return regs[C] <= regs[D];
            break;
        case 0x3:
            return regs[C] >= regs[D];
            break;
        case 0x4:
            return regs[C] < regs[D];
            break;
        case 0x5:
            return regs[C] > regs[D];
            break;
        default:
            return false;
    }
}

void CPU::jmp() {
    int lable;          
    lable = mem[regs[0xf]];
    bool found;
    for(int i = 0; i < 1024; i++) {
        if(mem[i] == lable && i != regs[0xf]) {
            std::cout << std::hex << i << std::endl;
            regs[0xf] = i + 1;
            found = true;
            break;
        }
    }
    if(!found) {
        std::cout << "Lable not found..." << std::endl;
    }
}

bool CPU::jumping(int B, int C, int D) {
    // This stuff doesn't make much sense for interpreter.
    switch(B){
        case 0x0:           
            jmp();
            break;
        case 0x1:
            if      (regs[C] == regs[D]) {_cmp_flag =  0;} 
            else if (regs[C] < regs[D])  {_cmp_flag = -1;}
            else                         {_cmp_flag =  1;}
            break;
        case 0x2:
            if(_cmp_flag == 0) {jmp();}
            else {regs[0xf]++;}
            break;
        case 0x3:
            if(_cmp_flag == -1) {jmp();}
            else {regs[0xf]++;}
            break;
        case 0x4:
            if(_cmp_flag == 1) {jmp();}
            else {regs[0xf]++;}
            break;
        case 0x5:
            if(_cmp_flag == 0 || _cmp_flag == -1) {jmp();}
            else {regs[0xf]++;}
            break;
        case 0x6:
            if(_cmp_flag == 0 || _cmp_flag == 1) {jmp();}
            else {regs[0xf]++;}
            break;
        default:
            return false;
    }

    return true;
}

bool CPU::output(int B, int C, int D) {
    switch(B) {
        case 0x0:
            std::cout << std::hex << regs[D] << std::endl;
            break;
        case 0x1:
            dump();
            break;
    }
    return true;
}

bool CPU::input(int B, int C, int D) {
    switch(B) {
        case 0x0:
            regs[D] = mem[regs[0xf]];
            break;
        case 0x1:
            regs[C] = regs[D];
            break;
        case 0x2:
            std::cout << ">> ";
            std::cin >> regs[D];
            break;
        default:
            return false;
    }
    return true;
}

int CPU::exec(int inst, bool intp) {

    if(intp) {
        mem[regs[0xf]++] = inst;
    } else {
        regs[0xf]++;
    }

    int A = (inst >> 12); 
    int B = (inst >> 8 )&0xF; 
    int C = (inst >> 4 )&0xF; 
    int D = (inst >> 0 )&0xF;

    switch(A) {
        case 0x1:
            break;
        case 0xA:
            arithmetic(B, C, D);
            break;
        case 0xB:
            std::cout << "CB" << std::endl;
            break;
        case 0xC:
            std::cout << conditionals(B, C, D);
            break;
        case 0xD:
            jumping(B, C, D);
            break;
        case 0xE:
            input(B, C, D);
            break;
        case 0xF:
            output(B, C, D);
            break;
        default:
            return 1;
    }

    return 0;
}

void CPU::dump() {
    for(int i = 0; i < 1024; i++) {
        std::cout << i << ": " << std::hex << mem[i] << std::endl;
    }
}

int CPU::run() {
    int start = regs[0xf];
    for(regs[0xf]; regs[0xf] < 1024;) {
        //std::cout << "Running: " << std::hex << regs[0xf] + 1 << std::endl;
        exec(mem[regs[0xf]], false);
    }
}

void menu(CPU& cpu) {
    int inst;
    do {
        std::cout << ">> ";
        std::cin >> std::hex >> inst;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cout << "0xF: " << cpu.regs[0xF] << std::endl;
    } while(!std::cin);
    std::cout << std::hex << inst << std::endl;
    cpu.exec(inst, true);
}

int main() {
    std::string choice;
    std::cout << "Enter \"file\" to use a file or \"intp\" to use the interpreter: ";
    std::cin >> choice;

    CPU cpu;
    if(choice == "intp") {
        while(1) {
            menu(cpu);
        }
    } else if(choice == "file") {
        std::string filename;
        std::cout << "Filename: ";
        std::cin >> filename;
        std::ifstream file(filename); 
        std::string line;
        int counter = 0;
        while(std::getline(file, line)) {
            int opcode;
            std::istringstream ss(line);
            if(ss >> std::hex >> opcode) {
                //std::cout << std::hex << opcode << " ";
                cpu.mem[counter++] = opcode;
            }
        }
        std::cout << std::endl;
        cpu.run();
    } else {
        std::cout << "ERROR: Invalid Choice. Please run again." << std::endl;
    }

    return 0;


}