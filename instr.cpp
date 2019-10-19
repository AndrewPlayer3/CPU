/*
    INSTR Hex Instruction Set
    Andrew Player
    October 18, 2019
*/
#include "instr.hpp"

bool CPU::arithmetic(int B, int C, int D) {
    switch(B) {
        case 0x0: regs[C] += regs[D]; break;                /* r[C] += r[D] 0xA0.. */
        case 0x1: regs[C] -= regs[D]; break;                /* r[C] -= r[D] 0xA1.. */
        case 0x2: regs[C] *= regs[D]; break;                /* r[C] *= r[D] 0xA2.. */
        case 0x3: regs[C] %= regs[D]; break;                /* r[C] %= r[D] 0xA3.. */
        case 0x4: regs[D] += mem[regs[0xf]++]; break;       /* r[C] += int  0xA4.. */
        case 0x5: regs[D] -= mem[regs[0xf]++]; break;       /* r[C] -= int  0xA5.. */
        case 0x6: regs[D] *= mem[regs[0xf]++]; break;       /* r[C] *= int  0xA6.. */
        case 0x7: regs[D] %= mem[regs[0xf]++]; break;       /* r[C] %= int  0xA7.. */
        default: return false;
    }
    return true;
}

bool CPU::bitwise(int B, int C, int D) {
    switch(B) {
        case 0x0: regs[C] &= regs[D]; break;                /* r[C] &= r[D] 0xB0.. */
        case 0x1: regs[C] |= regs[D]; break;                /* r[C] |= r[D] 0xB1.. */
        case 0x2: regs[C] <<= regs[D]; break;               /* r[C] << r[D] 0xB2.. */
        case 0x3: regs[C] >>= regs[D]; break;               /* r[C] >> r[D] 0xB3.. */
        case 0x4: regs[D] &= mem[regs[0xf]++]; break;       /* r[C] &= int  0xB4.. */
        case 0x5: regs[D] |= mem[regs[0xf]++]; break;       /* r[C] |= int  0xB5.. */
        case 0x6: regs[D] <<= mem[regs[0xf]++]; break;      /* r[C] << int  0xB6.. */
        case 0x7: regs[D] >>= mem[regs[0xf]++]; break;      /* r[C] >> int  0xB7.. */
        default: return false;
    }
    return true;
}

bool CPU::conditionals(int B, int C, int D) {
    switch(B) {
        case 0x0: return regs[C] == regs[D]; break;         /* r[C] == r[D] 0xC0.. */
        case 0x1: return regs[C] != regs[D]; break;         /* r[C] != r[D] 0xC1.. */
        case 0x2: return regs[C] <= regs[D]; break;         /* r[C] <= r[D] 0xC2.. */
        case 0x3: return regs[C] >= regs[D]; break;         /* r[C] >= r[D] 0xC3.. */
        case 0x4: return regs[C] <  regs[D]; break;         /* r[C] <  r[D] 0xC4.. */
        case 0x5: return regs[C] >  regs[D]; break;         /* r[C] >  r[D] 0xC5.. */
        case 0x6: return regs[C] == mem[regs[0xf]++]; break;/* r[C] == r[D] 0xC6.. */
        case 0x7: return regs[C] != mem[regs[0xf]++]; break;/* r[C] != r[D] 0xC7.. */
        case 0x8: return regs[C] <= mem[regs[0xf]++]; break;/* r[C] <= r[D] 0xC8.. */
        case 0x9: return regs[C] >= mem[regs[0xf]++]; break;/* r[C] >= r[D] 0xC9.. */
        case 0xA: return regs[C] <  mem[regs[0xf]++]; break;/* r[C] <  r[D] 0xCA.. */
        case 0xB: return regs[C] >  mem[regs[0xf]++]; break;/* r[C] >  r[D] 0xCB.. */
        default: return false;
    }
}

void CPU::jmp() {
    bool found;          
    int lable = mem[regs[0xf]];
    for(int i = 0; i < space(); i++) {
        if(mem[i] == lable && i != regs[0xf]) {
            regs[0xf] = i + 1;
            found = true;
            break;
        }
    }
    if(!found) { std::cout << "Lable not found..." << std::endl; }
}

bool CPU::jumping(int B, int C, int D) {
    switch(B){
        case 0x0:                                           /* jmp 0xD000 */           
            jmp();
            break;
        case 0x1:                                           /* cmp 0xD1.. */
            if      (regs[C] == regs[D]) {_cmp_flag =  0;}  // _cmp_flag is set in cpu,
            else if (regs[C] <  regs[D]) {_cmp_flag = -1;}  // so jmp before next cmp.
            else                         {_cmp_flag =  1;}
            break;
        case 0x2:                                           /* je  0xD200 */
            if(_cmp_flag == 0)  {jmp();}                  
            else                {regs[0xf]++;}
            break;
        case 0x3:                                           /* jl  0xD300 */
            if(_cmp_flag == -1) {jmp();}       
            else                {regs[0xf]++;}
            break;
        case 0x4:                                           /* jg  0xD400 */
            if(_cmp_flag == 1)  {jmp();}       
            else                {regs[0xf]++;}
            break;
        case 0x5:                                           /* jle 0xD500 */
            if(_cmp_flag == 0 
            || _cmp_flag == -1) {jmp();}      
            else                {regs[0xf]++;}
            break;
        case 0x6:                                           /* jge 0xD600 */
            if(_cmp_flag == 0 
            || _cmp_flag == 1)  {jmp();}       
            else                {regs[0xf]++;}
            break;
        default: return false;
    }
    return true;
}

bool CPU::output(int B, int C, int D) {
    std::string str = "";
    int loc;
    switch(B) {
        case 0x0:                                           /* cout r[D]  0xF00D */
            std::cout << std::hex << regs[D] << std::endl;
            break;
        case 0x1: mem_dump(); break;                        /* mem dump   0xF100 */
        case 0x2: reg_dump(); break;                        /* reg dump   0xF200 */
        case 0x3:                                           /* cout@ptr   0xF30D */
            loc = regs[D];
            while(mem[loc] != '0') {
                str += (char)mem[loc];
                loc++;
            }
            std::cout << str;
            break;
        case 0x4:                                           /* cout@ptr\n 0xF30D */
            loc = regs[D];
            while(mem[loc] != '0') {
                str += (char)mem[loc];
                loc++;
            }
            std::cout << str << std::endl;
            break;
        default: return false;
    }
    return true;
}

bool CPU::input(int B, int C, int D) {
    switch(B) {
        case 0x0: regs[D] = mem[regs[0xf]]; break;          /* mov r[D],int  0xE0..  */
        case 0x1: regs[C] = regs[D];        break;          /* mov r[D],int  0xE1..  */
        case 0x2:                                           /* cin r[D],int  0xE20.  */
            std::cout << ">> "; 
            std::cin >> std::hex >> regs[D]; 
            break;
        case 0x3: regs[D] = regs[0xf];                      /* mov r[D],str* 0xE30.  */
            while(mem[regs[0xf]] != '0') {
                regs[0xf]++;
            } break;
        default: return false;
    }
    return true;
}

int CPU::exec(int inst, bool intp) {
    regs[0xf]++;
    int A = (inst >> 12); 
    int B = (inst >> 8 )&0xF; 
    int C = (inst >> 4 )&0xF; 
    int D = (inst >> 0 )&0xF;
    switch(A) {
        case 0x1: /* 0x1-9 are Lables */ break; /* 0x1... */ 
        case 0xA: arithmetic  (B, C, D); break; /* 0xA... */
        case 0xB: bitwise     (B, C, D); break; /* 0xB... */
        case 0xC: conditionals(B, C, D); break; /* 0xC... */
        case 0xD: jumping     (B, C, D); break; /* 0xD... */
        case 0xE: input       (B, C, D); break; /* 0xE... */
        case 0xF: output      (B, C, D); break; /* 0xF... */
        default: return 1;
    }
    return 0;
}

void CPU::mem_dump() {
    for(int i = 0; i < space(); i++) {
        std::cout << i << ": " << std::hex << mem[i] << std::endl;
    }
}

void CPU::reg_dump() {
    for(int i = 0; i < 16; i++) {
        std::cout << "Register[" << std::hex << i << "]: " << regs[i] << std::endl; 
    }
}

int CPU::run() {
    int start = regs[0xf];
    for(regs[0xf]; regs[0xf] < space();) {
        exec(mem[regs[0xf]], false);
    }
    return 0;
}

int main() {
    std::string choice;
    CPU cpu;
    std::string filename;
    std::cout << "Filename: ";
    std::cin >> filename;
    std::ifstream file(filename); 
    std::string line;
    int counter = 0;
    while(std::getline(file, line)) {
        if(line[0] == '0') {
            int opcode;
            std::istringstream ss(line);
            if(ss >> std::hex >> opcode) {
                cpu.mem[counter++] = opcode;
            } 
        }
        else if(line[0] != '#') {
            for(char c : line) {
                cpu.mem[counter++] = c;
            }
        }
    }
    std::cout << std::endl;
    cpu.run();
    std::cout << std::endl;
    return 0;
}