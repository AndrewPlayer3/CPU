#ifndef _ASSEMBLER_HPP_
#define _ASSEMBLER_HPP_

#include <regex>
#include <map>
#include "instr.hpp"
#include "constants.hpp"

using std::string;
using std::pair;
using std::map;
using std::vector;

map<string, OPCODE_MNEMONIC> str_to_op = {
    {"mov", MOV},
    {"jmp", JMP},
    {"cmp", CMP},
    {"add", ADD},
    {"sub", SUB},
    {"mul", MUL},
    {"and", AND},
    {"or" , OR },
    {"rsh", RSH},
    {"lsh", LSH},
    {"out", OUT},
};

map<string, REGISTER> str_to_reg = {
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

std::string trim(std::string s);

class Lexer {

private:

    std::string _str;

public:

    Lexer(std::string s) {
        _str = trim(s);
    }

    void print_lexed() {
        std::cout << _str << std::endl;
    }

};

class Parser  {

};

class CodeGenerator {

};

#endif


