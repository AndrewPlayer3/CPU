/*
    Andrew Player 2019
*/

#ifndef _ASSEMBLER_HPP_
#define _ASSEMBLER_HPP_

/* This is an assembler that takes instr assembly and */
/* turns it into valid instr machine code.            */

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::pair;

/* These are all possible valid argument types */
enum ARG_TYPE {
    INTEGER = 0x1,
    OPCODE,
    REGISTER,
    STRING,
    POINTER,
    EMPTY
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

std::string trim(const std::string& s);
bool is_label(const std::string& line);
std::string trim_label(const std::string& label);
bool is_opcode(const std::string& str);
bool is_comment(const std::string& line);
bool is_register(const std::string& str);
bool is_instruction(const std::string& line);
bool is_pointer(const std::string& str);
bool is_int(const std::string& str);
int to_int(const std::string& str);
vector<std::string> parse_instruction(const std::string& line);
vector<pair<std::string, ARG_TYPE>> parse_arg_types(const vector<std::string>& arg_vector);
std::string builder(const vector<pair<std::string, ARG_TYPE>>& instr);
std::ostringstream gen_machine_code(const std::string& filename);
std::ostringstream gen_code_from_line(std::string& line);

#endif