#ifndef _ASSEMBLER_HPP_
#define _ASSEMBLER_HPP_

#include <regex>
#include <map>
#include "instr.hpp"

using std::string;
using std::pair;
using std::map;
using std::vector;

enum OPCODE_MNEMONIC {
    MOV = 0x1,
    JMP,
    JE,
    JL,
    JG,
    JLE,
    JGE,
    CMP,
    ADD,
    SUB,
    MUL,
    MOD,
    AND,
    OR,
    RSH,
    LSH,
    INI,
    INS,
    OUT,
    PUT,
    PLN,
    MDP,
    RDP,
    NOP
};

enum REGISTERS {
    R0  = 0x0,
    R1  = 0x1,
    R2  = 0x2,
    R3  = 0x3,
    R4  = 0x4,
    R5  = 0x5,
    R6  = 0x6,
    R7  = 0x7,
    R8  = 0x8,
    R9  = 0x9,
    R10 = 0xA,
    R11 = 0xB,
    R12 = 0xC,
    R13 = 0xD,
    R14 = 0xE,
    R15 = 0xF
};

enum ARG_TYPE {
    INTEGER = 0x1,
    OPCODE,
    REGISTER,
    POINTER,
    STRING,
    CHARACTER
};

map<string, OPCODE_MNEMONIC> str_to_op = {
    {"mov", MOV},
    {"jmp", JMP},
    {"je" , JE },
    {"jl" , JL },
    {"jg" , JG },
    {"jle", JLE},
    {"jge", JGE},
    {"cmp", CMP},
    {"add", ADD},
    {"sub", SUB},
    {"mul", MUL},
    {"mod", MOD},
    {"and", AND},
    {"or" , OR },
    {"rsh", RSH},
    {"lsh", LSH},
    {"ini", INI},
    {"ins", INS},
    {"out", OUT},
    {"put", PUT},
    {"pln", PLN},
    {"mdp", MDP},
    {"rdp", RDP},
    {"nop", NOP}
};

map<string, REGISTERS> str_to_reg = {
    {"r0",  R0 },
    {"r1",  R1 },
    {"r2",  R2 },
    {"r3",  R3 },
    {"r4",  R4 },
    {"r5",  R5 },
    {"r6",  R6 },
    {"r7",  R7 },
    {"r8",  R8 },
    {"r9",  R9 },
    {"r10", R10},
    {"r11", R11},
    {"r12", R12},
    {"r13", R13},
    {"r14", R14},
    {"r15", R15},
};

#endif


