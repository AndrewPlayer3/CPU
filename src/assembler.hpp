/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
DOESN'T WORK WITH 32-BIT MINGW/32-BIT GCC
*/

#ifndef _ASSEMBLER_HPP_
#define _ASSEMBLER_HPP_

/* This is an assembler that takes instr assembly and */
/* turns it into valid instr machine code.            */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;
using std::pair;

/* These are all possible valid argument types */
enum ARG_TYPE {
    INTEGER = 0x1,
    OPCODE,
    REGISTER,
    STRING
};

/* These are all the registers, all are scratch except for r15 */
/* which is the program counter.                               */
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

/* map from opcode to vector of the digits that can be assembled            */
/* the last three digits are the possible second digits (aka A*CD)          */
/* when the last operand is a register, or integer, or string, respectively */
map<string, vector<int>> op_to_int = {
    {"mov", {0xE, 0x1, 0x0, 0x3}},
    {"jmp", {0xD, 0x0, 0x0, 0x0}},
    {"je" , {0xD, 0x2, 0x2, 0x2}},
    {"jl" , {0xD, 0x3, 0x3, 0x3}},
    {"jg" , {0xD, 0x4, 0x4, 0x4}},
    {"jle", {0xD, 0x5, 0x5, 0x5}},
    {"jge", {0xD, 0x6, 0x6, 0x6}},
    {"cmp", {0xD, 0x1, 0x1, 0x1}},
    {"add", {0xA, 0x0, 0x5, 0x0}},
    {"sub", {0xA, 0x1, 0x6, 0x0}},
    {"mul", {0xA, 0x2, 0x7, 0x0}},
    {"mod", {0xA, 0x3, 0x8, 0x0}},
    {"div", {0xA, 0x4, 0x9, 0x0}},
    {"and", {0xB, 0x0, 0x6, 0x0}},
    {"or" , {0xB, 0x1, 0x7, 0x0}},
    {"xor", {0xB, 0x4, 0xB, 0x0}},
    {"rsh", {0xB, 0x3, 0x9, 0x0}},
    {"lsh", {0xB, 0x2, 0x8, 0x0}},
    {"not", {0xB, 0x5, 0xA, 0x0}},
    {"ini", {0xE, 0x2, 0x2, 0x2}},
    {"ins", {0xE, 0x8, 0x8, 0x8}},
    {"out", {0xF, 0x0, 0x0, 0x0}},
    {"put", {0xF, 0x3, 0x3, 0x3}},
    {"pln", {0xF, 0x4, 0x4, 0x4}},
    {"mdp", {0xF, 0x1, 0x1, 0x1}},
    {"rdp", {0xF, 0x2, 0x2, 0x2}},
    {"nop", {0x0, 0x0, 0x0, 0x0}}
};

/* These are all the opcode mnemonics for now    */
vector<std::string> opcode_vector = {
    "mov",
    "jmp",
    "je" ,
    "jl" ,
    "jg" ,
    "jle",
    "jge",
    "cmp",
    "add",
    "sub",
    "mul",
    "mod",
    "div",
    "and",
    "or" ,
    "xor",
    "rsh",
    "lsh",
    "not",
    "ini",
    "ins",
    "out",
    "put",
    "pln",
    "mdp",
    "rdp",
    "nop"
};
/* btw                                           */
/* std::map::count() was giving us trouble thats */
/* why this exists along with the op_to_int map  */

std::string trim(const std::string& s);
bool is_label(const std::string& line);
std::string trim_label(const std::string& label);
bool is_opcode(const std::string& str);
bool is_comment(const std::string& line);
bool is_register(const std::string& str);
bool is_instruction(const std::string& line);
bool is_int(const std::string& str);
int to_int(const std::string& str);
vector<std::string> parse_instruction(const std::string& line);
vector<pair<std::string, ARG_TYPE>> parse_arg_types(const vector<std::string>& arg_vector);
std::string builder(const vector<pair<std::string, ARG_TYPE>>& instr);
std::ostringstream gen_machine_code(const std::string& filename);


#endif