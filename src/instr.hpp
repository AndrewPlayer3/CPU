/*
    Andrew Player 2019
*/

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu   */
/* refer to the readme for a list of the instructions                    */
/* The machine code for this cpu can be generated from an assembly using */
/* the included assembler. (Note: that is a seperate program)            */

#ifndef _INSTR_H_
#define _INSTR_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

/* Using defines because I think they look nicer than enums */
#define REGISTER_COUNT 0x10
#define MEMORY_SIZE    0x1FF
#define STACK_SIZE     0x10

/* Program Counter Register will be the last register */
#define PCTR           REGISTER_COUNT - 1
/* Stack Pointer Register */
#define SP             0xE

/* 32-bit CPU emulator */
class CPU {

private:
    /* Flag after cmp -> 1 for >, 0 for ==, and -1 for <   */
    /* If we need more flags, we can make a flag register. */
    int _cmp_flag;

public:

    /* reg count is hardcoded to 16 to have 1 digit reg numbers */
    int regs [REGISTER_COUNT];
    int mem  [MEMORY_SIZE];
    int* stck = &mem[MEMORY_SIZE - 1];

    /* The next free memory location to write to */
    int next_free_location;
    /* The end of the text section. This is the   */
    /* value of next_free_location at the start   */
    int end_text_section;

    CPU() {
        /* Set mem and regs to 0 */
        for(int i = 0; i < REGISTER_COUNT; i++) { regs[i] = 0; }
        for(int i = 0; i < MEMORY_SIZE   ; i++) { mem [i] = 0; }
        next_free_location  = 0;
        _cmp_flag = 0;
    }

    void arithmetic(int B, int C, int D); /* 0xA... */
    void bitwise   (int B, int C, int D); /* 0xB... */
    void jumping   (int B, int C, int D); /* 0xD... */
    void output    (int B, int C, int D); /* 0xF... */
    void input     (int B, int C, int D); /* 0xE... */
    void jmp       ();                    /* 0xD000 */
    void mem_dump  (int until);           /* 0xF100 */
    void reg_dump  ();                    /* 0xF200 */
    bool free_memory();
    bool free_memory(int& location);
    bool free_memory(int& location, int& distance);
    void push(int reg);
    void pop(int reg);
    void pusha();
    void popa();
    void exec      (int inst);
    void run       ();
};

bool parse_file  (std::string& filename, int* mem, int& next_free_location, int& end_text_section);
void parse_string(std::string& str,      int* mem, int& next_free_location);
std::string read_memory(int* memory);
void error(int loc, int inst);
void error(int loc, int A, int B, int C, int D);

#endif