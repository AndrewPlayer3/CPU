#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

// TODO: Add the actual numbers for these opcodes
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

// enum TOKEN_TYPE {
//     OPCODE = 0x1,
//     OPERAND,
//     LABEL
// };

#endif