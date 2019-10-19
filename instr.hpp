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
        // Clean memory and registers
        for(int i = 0; i < 16; i++) {
            regs[i] = 0;
        }
        for(int i = 0; i < space(); i++) {
            mem[i] = 0;
        }
    }

    bool arithmetic(int B, int C, int D);
    bool bitwise(int B, int C, int D);
    bool conditionals(int B, int C, int D);
    bool jumping(int B, int C, int D);
    bool output(int B, int C, int D);
    bool input(int B, int C, int D);
    void jmp();
    void mem_dump();
    void reg_dump();
    int space() {return _memory;}
    int exec(int inst, bool intp);
    int run();
    void print(int reg);
};