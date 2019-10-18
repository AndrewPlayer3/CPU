#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <limits>

class CPU {

public:

    int regs[16];
    int mem[1024];

    CPU() {}

    bool arithmetic(int B, int C, int D);
    bool conditionals(int B, int C, int D);
    bool jumping(int B, int C, int D);
    bool output(int B, int C, int D);
    bool input(int B, int C, int D);

    int exec(int inst);
    void print(int reg);

private:

    int _cmp_flag;
};