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
    }
}

bool CPU::jumping(int B, int C, int D) {

    switch(B){
        case 0x0:
            
            break;
        case 0x1:
            if (regs[C] == regs[D]) {
                return 0;
            } else if(regs[C] < regs[D]) {
                return -1;
            } else {
                return 1;
            }
            break;
        case 0x2:
            
            break;
        case 0x3:
            
            break;
        case 0x4:
            
            break;
        case 0x5:
            
            break;
        case 0x6:
            
            break;
    }

    return true;
}

bool CPU::output(int B, int C, int D) {
    std::cout << std::hex << regs[D] << std::endl;
    return true;
}

bool CPU::input(int B, int C, int D) {
    switch(B) {
        case 0x0:
            std::cout << ">> ";
            std::cin >> regs[D];
            break;
        case 0x1:
            regs[C] = regs[D];
            break;
        default:
            return false;
    }
    return true;
}

int CPU::exec(int inst) {

    int A = (inst >> 12); 
    int B = (inst >> 8 )&0xF; 
    int C = (inst >> 4 )&0xF; 
    int D = (inst >> 0 )&0xF;

    switch(A) {
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
            std::cout << "CD" << std::endl;
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

void menu() {
    int inst;
    std::cout << ">> ";
    std::cin >> std::hex >> inst;
    std::cout << std::hex << inst << std::endl;
    CPU cpu;
    cpu.exec(inst);
}

int main() {
    
    while(1) {
        menu();
    }

    return 0;
}