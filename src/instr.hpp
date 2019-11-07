/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
*/

/* This is a 16-bit hex instruction-set that runs on an "emulated" cpu  */
/* refer to the readme for a list of the instructions                   */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>

/* 32-bit CPU emulator */
class CPU {

private:
    /* Flag after cmp -> 1 for >, 0 for ==, and -1 for <   */
    /* If we need more flags, we can make a flag register. */
    int _cmp_flag;

    /* Memory Size */
    enum{_memory=0xff};

    /* Current free memory location */
    int _mem_loc;


public:

    /* reg count is hardcoded to 16 to have 1 digit reg numbers */
    int regs[16];
    int mem[_memory];

    CPU() {
        /* Set mem and regs to 0 */
        for(int i = 0; i < 16     ; i++) { regs[i] = 0; }
        for(int i = 0; i < _memory; i++) { mem [i] = 0; }
        _mem_loc  = 0;
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
    int  space       (){return _memory;}
    void exec        (int inst);
    void run         ();
    void parse_file  (std::string& filename);
    void parse_string(std::string& str);
};

void error(int loc, int inst);
void error(int loc, int A, int B, int C, int D);
