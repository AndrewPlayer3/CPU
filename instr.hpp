#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <limits>

class CPU {

private:
    // Flag after cmp -> 1 for >, 0 for ==, and -1 for <
    int _cmp_flag;
    enum{_memory=0x300};

public:

    int regs[16];
    int mem[_memory];

    CPU() {
        /* Set mem and regs to 0 */
        for(int i = 0; i < 16; i++)      { regs[i] = 0; }
        for(int i = 0; i < space(); i++) { mem[i]  = 0; }
    }

    bool arithmetic  (int B, int C, int D); /* 0xA... */
    bool bitwise     (int B, int C, int D); /* 0xB... */
    bool conditionals(int B, int C, int D); /* 0xC... */
    bool jumping     (int B, int C, int D); /* 0xD... */
    bool output      (int B, int C, int D); /* 0xF... */
    bool input       (int B, int C, int D); /* 0xE... */
    void jmp();                             /* 0xD000 */
    void mem_dump();                        /* 0xF100 */
    void reg_dump();                        /* 0xF200 */
    int space()      {return _memory;}
    int exec         (int inst);
    int run();
};