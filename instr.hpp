#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <limits>

class CPU {

public:

    int regs[16];
    int mem[1024];

    CPU() {
        for(int i = 0; i < 16; i++) {
            regs[i] = 0;
        }
        for(int i = 0; i < 1024; i++) {
            mem[i] = 0;
        }
    }

    bool arithmetic(int B, int C, int D);
    bool conditionals(int B, int C, int D);
    bool jumping(int B, int C, int D);
    bool output(int B, int C, int D);
    bool input(int B, int C, int D);
    void jmp();

    void dump();

    int exec(int inst, bool intp);
    int run();
    void print(int reg);

private:

    int _cmp_flag;
};