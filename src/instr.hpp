/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
*/

#ifndef _INSTR_H_
#define _INSTR_H_

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu  */
/* refer to the readme for a list of the instructions                   */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

/* 32-bit CPU emulator */
class CPU {

private:
    /* Flag after cmp -> 1 for >, 0 for ==, and -1 for <   */
    /* If we need more flags, we can make a flag register. */
    int _cmp_flag;

    /* Memory Size */
    enum{_memory=0xFF};


public:

    /* reg count is hardcoded to 16 to have 1 digit reg numbers */
    int regs[16];
    int mem[_memory];

    /* The next free memory location to write to */
    int next_free_location;

    CPU() {
        /* Set mem and regs to 0 */
        for(int i = 0; i < 16     ; i++) { regs[i] = 0; }
        for(int i = 0; i < _memory; i++) { mem [i] = 0; }
        next_free_location  = 0;
        _cmp_flag = 0;
    }

    void arithmetic  (int B, int C, int D); /* 0xA... */
    void bitwise     (int B, int C, int D); /* 0xB... */
    void jumping     (int B, int C, int D); /* 0xD... */
    void output      (int B, int C, int D); /* 0xF... */
    void input       (int B, int C, int D); /* 0xE... */
    void jmp         ();                    /* 0xD000 */
    void mem_dump    ();                    /* 0xF100 */
    void reg_dump    ();                    /* 0xF200 */
    int  space       (){return _memory ;}
    void exec        (int inst);
    void run         ();
};

void parse_file  (std::string& filename, int* mem, int& next_free_location);
void parse_string(std::string& str, int* mem, int& next_free_location);
std::string read_memory(int* memory);
void error(int loc, int inst);
void error(int loc, int A, int B, int C, int D);

#endif