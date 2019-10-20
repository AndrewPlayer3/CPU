/*
INSTR Hex Instruction Set
Andrew Player
October 18, 2019
*/
#include "instr.hpp"

int LINE_NUMBER = 0;

void error(int loc, int A, int B, int C, int D) {
    int inst = (A << 12) | (B << 8) | (C << 4) | (D << 0);
    std::string err = "\n\nERROR! Invalid OPCODE:";
    
    std::cout << std::hex << err << " " << inst << 
    ", mem[" << loc << "], at Line " << std::dec 
    << LINE_NUMBER << std::endl;
    
    std::cout << "Exiting Program with Code 1\n" << std::endl;
    exit(1);
}

void CPU::arithmetic(int B, int C, int D) {
    switch(B) {
        case 0x0: regs[C] += regs[D]; break;                /* r[C] += r[D] 0xA0.. */
        case 0x1: regs[C] -= regs[D]; break;                /* r[C] -= r[D] 0xA1.. */
        case 0x2: regs[C] *= regs[D]; break;                /* r[C] *= r[D] 0xA2.. */
        case 0x3: regs[C] %= regs[D]; break;                /* r[C] %= r[D] 0xA3.. */
        case 0x4: regs[D] += mem[regs[0xf]++]; break;       /* r[C] += int  0xA4.. */
        case 0x5: regs[D] -= mem[regs[0xf]++]; break;       /* r[C] -= int  0xA5.. */
        case 0x6: regs[D] *= mem[regs[0xf]++]; break;       /* r[C] *= int  0xA6.. */
        case 0x7: regs[D] %= mem[regs[0xf]++]; break;       /* r[C] %= int  0xA7.. */
        default: error(regs[0xf], 0xA, B, C, D);
    }
}

void CPU::bitwise(int B, int C, int D) {
    switch(B) {
        case 0x0: regs[C] &= regs[D]; break;                /* r[C] &= r[D] 0xB0.. */
        case 0x1: regs[C] |= regs[D]; break;                /* r[C] |= r[D] 0xB1.. */
        case 0x2: regs[C] <<= regs[D]; break;               /* r[C] << r[D] 0xB2.. */
        case 0x3: regs[C] >>= regs[D]; break;               /* r[C] >> r[D] 0xB3.. */
        case 0x4: regs[D] &= mem[regs[0xf]++]; break;       /* r[C] &= int  0xB4.. */
        case 0x5: regs[D] |= mem[regs[0xf]++]; break;       /* r[C] |= int  0xB5.. */
        case 0x6: regs[D] <<= mem[regs[0xf]++]; break;      /* r[C] << int  0xB6.. */
        case 0x7: regs[D] >>= mem[regs[0xf]++]; break;      /* r[C] >> int  0xB7.. */
        default: error(regs[0xf], 0xB, B, C, D);
    }
}

void CPU::jmp() {       
    int lable = mem[regs[0xf]];
    for(int i = 0; i < space(); i++) {
        if(mem[i] == lable && i != regs[0xf]) {
            regs[0xf] = i + 1; return;
        }
    }
    std::cout << "Lable not found..." << std::endl;
}

void CPU::jumping(int B, int C, int D) {
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
        default: error(regs[0xf], 0xD, B, C, D);
    }
}

void CPU::output(int B, int C, int D) {
    std::string str = "";
    int loc;
    switch(B) {
        case 0x0:                                           /* cout r[D]  0xF00. */
            std::cout << std::hex << regs[D] << std::endl;
            break;
        case 0x1: mem_dump(); break;                        /* mem dump   0xF100 */
        case 0x2: reg_dump(); break;                        /* reg dump   0xF200 */
        case 0x3:                                           /* cout@ptr   0xF30. */
            loc = regs[D];
            while(mem[loc] != '\\' && mem[loc+1] != '0') {
                str += (char)mem[loc];
                loc++;
            }
            std::cout << str;
            break;
        case 0x4:                                           /* cout@ptr\n 0xF40. */
            loc = regs[D];
            while(mem[loc] != '\\' && mem[loc+1] != '0') {
                str += (char)mem[loc];
                loc++;
            }
            std::cout << str << std::endl; break;
        case 0x5:                                           /* cout@mem   0xF50. */
            std::cout << std::hex << mem[regs[D]] << std::endl;
            break;
        default: error(regs[0xf], 0xF, B, C, D);
    }
}

void CPU::input(int B, int C, int D) {
    switch(B) {
        case 0x0: regs[D] = mem[regs[0xf]];      break;     /* mov r[D],int  0xE0..  */
        case 0x1: regs[C] = regs[D];             break;     /* mov r[D],int  0xE1..  */
        case 0x2:                                           /* cin r[D],int  0xE20.  */
            std::cout << ">> "; 
            std::cin >> std::hex >> regs[D]; 
            break;
        case 0x3: regs[D] = regs[0xf];                      /* mov r[D],str* 0xE30.  */
            while(mem[regs[0xf]-1] != '\\' 
                 && mem[regs[0xf]] != '0') {
                regs[0xf]++;
            } break;
        case 0x4: regs[C] = mem[regs[D]];        break;     /* mov r[D],mem  0xE4..  */
        case 0x5: mem[regs[C]] = mem[regs[D]];   break;     /* mov mem,r[C]  0xE5..  */
        case 0x6: mem[regs[0xf]] = mem[regs[D]]; break;     /* mov mem,r[D]  0xE5..  */
        case 0x7: regs[D] = mem[mem[regs[0xf]]]; break;     /* mov mem,int   0xE5..  */
        default: error(regs[0xf], 0xE, B, C, D);
    }
}

int CPU::exec(int inst) {
    regs[0xf]++;
    int A = (inst >> 12); 
    int B = (inst >> 8 )&0xF; 
    int C = (inst >> 4 )&0xF; 
    int D = (inst >> 0 )&0xF;
    switch(A) {
        case 0x0: /* 0x0-9 are Lables */ break;             /* 0x0... */  
        case 0x1: /* 0x0-9 are Lables */ break;             /* 0x1... */ 
        case 0x2: /* 0x0-9 are Lables */ break;             /* 0x2... */ 
        case 0x3: /* 0x0-9 are Lables */ break;             /* 0x3... */ 
        case 0x4: /* 0x0-9 are Lables */ break;             /* 0x4... */ 
        case 0x5: /* 0x0-9 are Lables */ break;             /* 0x5... */ 
        case 0x6: /* 0x0-9 are Lables */ break;             /* 0x6... */ 
        case 0x7: /* 0x0-9 are Lables */ break;             /* 0x7... */ 
        case 0x8: /* 0x0-9 are Lables */ break;             /* 0x8... */ 
        case 0x9: /* 0x0-9 are Lables */ break;             /* 0x9... */ 
        case 0xA: arithmetic  (B, C, D); break;             /* 0xA... */
        case 0xB: bitwise     (B, C, D); break;             /* 0xB... */
        case 0xD: jumping     (B, C, D); break;             /* 0xD... */
        case 0xE: input       (B, C, D); break;             /* 0xE... */
        case 0xF: output      (B, C, D); break;             /* 0xF... */
        default:  error(regs[0xf], 0xA, B, C, D); 
        return 1;
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

int  CPU::run() {
    int start = regs[0xf];
    int retcode;
    for(regs[0xf]; regs[0xf] < space();) {
        retcode = exec(mem[regs[0xf]]);
    }
    return retcode;
}

int  main() {
    std::string choice;
    CPU cpu;
    std::string filename;
    std::cout << "Filename: ";
    std::cin >> filename;
    std::ifstream file(filename); 
    std::string line;
    int counter = 0;
    while(std::getline(file, line)) {
        LINE_NUMBER++;
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
    int retcode = cpu.run();
    std::cout << "\nProgram Exiting with Code " << retcode << std::endl;
    std::cout << std::endl;
    return 0;
}