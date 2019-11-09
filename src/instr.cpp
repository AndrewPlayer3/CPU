/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
*/

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu  */
/* refer to the readme for a list of the instructions                   */

#include "instr.hpp"

/* Breaks off each piece of the instructions and goes to the correct */
/* instruction function.                                             */
void CPU::exec(int inst) {
    int A = (inst >> 12); 
    int B = (inst >> 8 )&0xF; 
    int C = (inst >> 4 )&0xF; 
    int D = (inst >> 0 )&0xF;
    switch(A) {
        case 0x0: /* 0x0-9 are Labels */ break;             /* 0x0... */  
        case 0x1: /* for now          */ break;             /* 0x1... */ 
        case 0x2: /* ...              */ break;             /* 0x2... */ 
        case 0x3: /* ...              */ break;             /* 0x3... */ 
        case 0x4: /* ...              */ break;             /* 0x4... */ 
        case 0x5: /* ...              */ break;             /* 0x5... */ 
        case 0x6: /* ...              */ break;             /* 0x6... */ 
        case 0x7: /* ...              */ break;             /* 0x7... */ 
        case 0x8: /* ...              */ break;             /* 0x8... */ 
        case 0x9: /* ...              */ break;             /* 0x9... */ 
        case 0xA: arithmetic  (B, C, D); break;             /* 0xA... */
        case 0xB: bitwise     (B, C, D); break;             /* 0xB... */
        case 0xD: jumping     (B, C, D); break;             /* 0xD... */
        case 0xE: input       (B, C, D); break;             /* 0xE... */
        case 0xF: output      (B, C, D); break;             /* 0xF... */
        default:  error(regs[0xf], inst);break;
    }
}

/* Does arithmetic with registers and ints in memory */
void CPU::arithmetic(int B, int C, int D) {
    switch(B) {                                             /* Arithmetic:         */
        case 0x0: regs[C] += regs[D];          break;       /* r[C] += r[D] 0xA0.. */
        case 0x1: regs[C] -= regs[D];          break;       /* r[C] -= r[D] 0xA1.. */
        case 0x2: regs[C] *= regs[D];          break;       /* r[C] *= r[D] 0xA2.. */
        case 0x3: regs[C] %= regs[D];          break;       /* r[C] %= r[D] 0xA3.. */
        case 0x4: regs[D] += mem[regs[0xf]++]; break;       /* r[C] += int  0xA4.. */
        case 0x5: regs[D] -= mem[regs[0xf]++]; break;       /* r[C] -= int  0xA5.. */
        case 0x6: regs[D] *= mem[regs[0xf]++]; break;       /* r[C] *= int  0xA6.. */
        case 0x7: regs[D] %= mem[regs[0xf]++]; break;       /* r[C] %= int  0xA7.. */
        default: error(regs[0xf], 0xA, B, C, D);
    }
}

/* Does bitwise operations with registers and ints in memory */
void CPU::bitwise(int B, int C, int D) {
    switch(B) {                                             /* Bitwise:            */
        case 0x0: regs[C] &=  regs[D];          break;      /* r[C] &= r[D] 0xB0.. */
        case 0x1: regs[C] |=  regs[D];          break;      /* r[C] |= r[D] 0xB1.. */
        case 0x2: regs[C] <<= regs[D];          break;      /* r[C] << r[D] 0xB2.. */
        case 0x3: regs[C] >>= regs[D];          break;      /* r[C] >> r[D] 0xB3.. */
        case 0x4: regs[D] &=  mem[regs[0xf]++]; break;      /* r[C] &= int  0xB4.. */
        case 0x5: regs[D] |=  mem[regs[0xf]++]; break;      /* r[C] |= int  0xB5.. */
        case 0x6: regs[D] <<= mem[regs[0xf]++]; break;      /* r[C] << int  0xB6.. */
        case 0x7: regs[D] >>= mem[regs[0xf]++]; break;      /* r[C] >> int  0xB7.. */
        default: error(regs[0xf], 0xB, B, C, D);
    }
}

/* Sets program counter to the location of a label (+1) */
void CPU::jmp() {       
    int label = mem[regs[0xf]];
    int mem_size = space();
    for(int i = 0; i < mem_size; i++) {
        if(mem[i] == label && i != regs[0xf]) {
            regs[0xf] = i + 1; return;
        }
    }
    std::cout << "ERROR! No label found matching " 
	      << std::hex << mem[regs[0xf]] << std::endl;
}

/* Conditional Jumping, Jumping, and Compares */
void CPU::jumping(int B, int C, int D) {
    switch(B){                                              /* Jumping:   */
        case 0x0:                                           /* jmp 0xD000 */           
            jmp();
            break;
        case 0x1:                                           /* cmp 0xD1.. */
            if      (regs[C] == regs[D]) {_cmp_flag =  0;}  
            else if (regs[C] <  regs[D]) {_cmp_flag = -1;}  
            else if (regs[C] >  regs[D]) {_cmp_flag =  1;}
            break;
        case 0x2:                                           /* je  0xD200 */
            if      (_cmp_flag == 0)     {jmp()      ;}                  
            else                         {regs[0xf]++;}
            break;
        case 0x3:                                           /* jl  0xD300 */
            if      (_cmp_flag ==-1)     {jmp()      ;}       
            else                         {regs[0xf]++;}
            break;
        case 0x4:                                           /* jg  0xD400 */
            if      (_cmp_flag == 1)     {jmp()	     ;}       
            else                         {regs[0xf]++;}
            break;
        case 0x5:                                           /* jle 0xD500 */
            if      (_cmp_flag == 0 
                    ||_cmp_flag==-1)     {jmp()      ;}      
            else                         {regs[0xf]++;}
            break;
        case 0x6:                                           /* jge 0xD600 */
            if      (_cmp_flag == 0 
                    || _cmp_flag==1)     {jmp()      ;}       
            else                         {regs[0xf]++;}
            break;
        default: error(regs[0xf], 0xD, B, C, D);
    }
}

/* Move things into registers and memory */
void CPU::input(int B, int C, int D) {
    std::string input;
    switch(B) {                                             /* Input:                */
        case 0x0: regs[D] = mem[regs[0xf]];      break;     /* mov r[D],int  0xE0..  */
        case 0x1: regs[C] = regs[D];             break;     /* mov r[D],int  0xE1..  */
        case 0x2:                                           /* cin r[D],int  0xE20.  */
                  std::cout << ">> "; 
                  std::cin >>std::hex>> regs[D]; break;
        case 0x3: regs[D] = regs[0xf];                      /* mov r[D],str* 0xE30.  */
                  while(mem[regs[0xf]] != 0x5c30) {
                    regs[0xf]++;
                  }                              break;
        case 0x4: regs[C] = mem[regs[D]];        break;     /* mov r[D],mem  0xE4..  */
        case 0x5: mem[regs[C]] = mem[regs[D]];   break;     /* mov mem,r[C]  0xE5..  */
        case 0x6: mem[regs[0xf]] = mem[regs[D]]; break;     /* mov mem,r[D]  0xE6..  */
        case 0x7: regs[D] = mem[mem[regs[0xf]]]; break;     /* mov mem,int   0xE7..  */
        case 0x8: regs[D] = next_free_location;                       /* mov cin str   0xE80.  */
                  std::cin.clear();
                  std::cin.ignore(INT8_MAX, '\n');
                  std::cout << ">> " ;
                  std::getline(std::cin, input);
                  input += "\\0";
                  parse_string(input, &mem[0], next_free_location);           
                                                 break;
        case 0xF: regs[0xf] = _memory;           break;
        default: error(regs[0xf], 0xE, B, C, D);
    }
}

/* Outputs regs, strings, and dumps */
void CPU::output(int B, int C, int D) {
    std::string str = "";
    int loc;
    switch(B) {                                             /* Output:           */
        case 0x0:                                           /* cout r[D]  0xF00. */
            std::cout << std::hex << regs[D] << std::endl;
            break;
        case 0x1: mem_dump(); break;                        /* mem dump   0xF100 */
        case 0x2: reg_dump(); break;                        /* reg dump   0xF200 */
        case 0x3:                                           /* cout@ptr   0xF30. */
            loc = regs[D];
            std::cout<<read_memory(&mem[loc]);
            break;
        case 0x4:                                           /* cout@ptr\n 0xF40. */
            loc = regs[D];
            std::cout<<read_memory(&mem[loc])<<std::endl;
            break;
        case 0x5:                                           /* cout@mem   0xF50. */
            std::cout << std::hex 
            << mem[regs[D]] << std::endl;
            break;
        case 0x6:                                           /* new line   0xF600 */
            std::cout << std::endl; break;                                      
        default: error(regs[0xf], 0xF, B, C, D);
    }
}

/* Prints each memory location and its contents */
void CPU::mem_dump() {
    int mem_size = space();
    for(int i = 0; i < mem_size; i++) {
        /* Memory[x]: y */
        std::cout << "Memory[" << i << "]: " 
        << std::hex << mem[i] << std::endl;
    }
}

/* Prints each register and its contents */
void CPU::reg_dump() {
    for(int i = 0; i < 16; i++) {
        /* Register[x]: y */
        std::cout << "Register[" << std::hex << i << "]: " 
        << regs[i] << std::endl; 
    }
}

/* Excecutes each instruction in memory */
void CPU::run() {
    int prog_end = next_free_location;
    while(regs[0xf] < prog_end) {
        exec(mem[regs[0xf]++]);
    }
}

/* Reads strings from memory */
std::string read_memory(int* memory) {
    int loc = 0;
    int shift = 24;
    int mask = 0xFF;
    std::string str;
    while(memory[loc] != 0x5c30) {
        int chars = memory[loc];
        for(int i = shift; i >= 0; i-=8) {
            int character = (chars >> i)&mask;
            if(character != 0) str+=character;
        }
        ++loc;
    }
    return str;
}

/* Inserts strs into mem. 4 chars in each mem location.                          */
/* This uses \0 as the ending characters instead of just using str.size() or "". */
/* I have found this to be the simplest way that allows multiline strings.       */
void parse_string(std::string& str, int* mem, int& next_free_location) {
    int joinedChars = 0;
    int str_size = str.size();
    for(int i = 0; i < str_size; i++) {
        if(str[i-1] == '\\' && str[i] == '0') {
            int entry = (int) (str[i-1] << 8) | (str[i] << 0); 
            mem[next_free_location++] = entry;
            return;
        }
        if((i > 0 && i % 4 == 0) || (str[i] == '\\' && str[i+1] == '0')) {
            mem[next_free_location++] = joinedChars;
            joinedChars = (int) str[i];
        } else {
            if(joinedChars == 0) { 
                joinedChars = (int) str[i]; 
            } else {
                joinedChars = (joinedChars << 8) | ((int) str[i] << 0);
            }
        }
    }
}

/* Goes through the file and puts the opcodes and strings into memory */
void parse_file(std::string& filename, int* mem, int& next_free_location) {
    std::ifstream file(filename); 
    std::string line;
    while(std::getline(file, line)) {
        int line_pos = 0;
        /* For opcodes, preceeding whitespace is ignored */
        while(line[line_pos] == '\t' || line[line_pos] == ' ') {
            line_pos++;
        }
        /* Opcodes must start with a 0 */
        if(line[line_pos] == '0') {
            int opcode;
            std::istringstream ss(line);
            if(ss >> std::hex >> opcode) {
                mem[next_free_location++] = opcode;
            } 
        }
        /*Strings are entered in 4 char chunks into memory
          the chars are implicitly cast as ints           */
        else if(line[line_pos] != '#') { /* Lines that start with # are comments */
            parse_string(line, &mem[0], next_free_location);
            next_free_location++;
        }
    }
}

/* Prints the location and instruction and exits with code 1 */
void error(int loc, int A, int B, int C, int D) {
    int inst = (A << 12) | (B << 8) | (C << 4) | (D << 0);
    std::string err = "\n\nERROR! Invalid OPCODE:";
    
    std::cout << std::hex << err << " " << inst << 
    " at mem[" << loc << "]." << std::endl;
    
    std::cout << "Program Exited Unsuccessfully\n" << std::endl;
    exit(1);
}

/* Prints the location and instruction and exits with code 1 */
void error(int loc, int inst) {
    std::string err = "\n\nERROR! Invalid OPCODE:";
    
    std::cout << std::hex << err << " " << inst << 
    " at mem[" << loc << "]." << std::endl;
    
    std::cout << "Exiting Program with Code 1\n" << std::endl;
    exit(1);
}
