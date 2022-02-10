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
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

/* Using defines because I think they look nicer than enums */
#define REGISTER_COUNT 0x10
#define MEMORY_SIZE    0xFFF
#define STACK_SIZE     0x10

/* Program Counter Register will be the last register */
#define PCTR           REGISTER_COUNT - 1
/* Stack Pointer Register */
#define SP             0xE

bool parse_file(std::string& filename, int* mem, int& next_free_location, int& end_text_section);

/* 32-bit CPU emulator */
class CPU {

private:
    /* Flag after cmp -> 1 for >, 0 for ==, and -1 for <   */
    /* If we need more flags, we can make a flag register. */
    int _cmp_flag = 0;

public:
    /* reg count is hardcoded to 16 to have 1 digit reg numbers */
    int regs [REGISTER_COUNT];
    int mem  [MEMORY_SIZE];
    int* stck = &mem[MEMORY_SIZE - 1];

    /* The next free memory location to write to */
    int next_free_location = 0;

    /* The end of the text section. This is the   */
    /* value of next_free_location at the start   */
    int end_text_section = 0;

    /* This can be set to true when a program has */
    /* been fully loaded into memory.             */
    bool loaded = false;

    CPU() {
        /* Set mem and regs to 0 */
        for(int i = 0; i < REGISTER_COUNT; i++) { regs[i] = 0; }
        for(int i = 0; i < MEMORY_SIZE   ; i++) { mem [i] = 0; }
        next_free_location  = 0;
        _cmp_flag = 0;
        loaded = false;
    }

    CPU(std::string& filename) {
        /* Set mem and regs to 0 */
        for(int i = 0; i < REGISTER_COUNT; i++) { regs[i] = 0; }
        for(int i = 0; i < MEMORY_SIZE   ; i++) { mem [i] = 0; }
        next_free_location  = 0;
        end_text_section = 0;
        _cmp_flag = 0;
        loaded = parse_file(filename, &mem[0], next_free_location, end_text_section);
        run();
    }

    void arithmetic (int B, int C, int D); /* 0xA... */
    void bitwise    (int B, int C, int D); /* 0xB... */
    void jumping    (int B, int C, int D); /* 0xD... */
    void output     (int B, int C, int D); /* 0xF... */
    void input      (int B, int C, int D); /* 0xE... */
    void jmp        ();                    /* 0xD000 */
    void jmp        (int label);           /* 0xD000 */
    void mem_dump   (int until);           /* 0xF100 */
    void reg_dump   ();                    /* 0xF200 */
    void stack_dump ();                    /* 0xF800 */
    void push       (int reg);             /* 0xEB0. */
    void pop        (int reg);             /* 0xEC0. */
    void pusha      ();                    /* 0xED00 */
    void popa       ();                    /* 0xEE0. */
    void exec       (int inst);
    void run        ();
    bool free_memory();
    bool free_memory(int& location);
    bool free_memory(int& location, int& distance);
};

bool parse_file  (std::string& filename, int* mem, int& next_free_location, int& end_text_section);
bool parse_block (std::string& code, int* mem, int& next_free_location, int& end_text_section);
void parse_string(std::string& str,      int* mem, int& next_free_location);
std::string read_memory(int* memory);
void error(int loc, int inst);
void error(int loc, int A, int B, int C, int D);

#endif
