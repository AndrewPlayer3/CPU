# AssemblyProject

This is a 16-bit instruction set that operates on an "emulated" 32bit cpu,
along with an assembler for a custom assembly language.

There are several included demo programs in the programs folder. 

To build the program to you can run the build script, or run these commands:
"mkdir bin"
"g++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu"
"g++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler"

To assemble one(or more) of the test programs:
"bin/assembler programs/filename"

To run the compiled test programs:
"bin/cpu programs/filename.inst"

These are the instructions:

Arithmetic:

    0xA...

        /* r[C] += r[D] 0xA0.. */   /* add reg, int or reg */
        /* r[C] -= r[D] 0xA1.. */   /* sub reg, int or reg */
        /* r[C] *= r[D] 0xA2.. */   /* mul reg, int or reg */
        /* r[C] %= r[D] 0xA3.. */   /* mod reg, int or reg */
        /* r[C] += int  0xA4.. */
        /* r[C] -= int  0xA5.. */
        /* r[C] *= int  0xA6.. */
        /* r[C] %= int  0xA7.. */

Bitwise:

    0xC...

        /* r[C] &= r[D] 0xB0.. */
        /* r[C] |= r[D] 0xB1.. */
        /* r[C] << r[D] 0xB2.. */
        /* r[C] >> r[D] 0xB3.. */
        /* r[C] &= int  0xB4.. */
        /* r[C] |= int  0xB5.. */
        /* r[C] << int  0xB6.. */
        /* r[C] >> int  0xB7.. */

Jumping:

    Lable 0x1...

    0xD...

        /* jmp 0xD000 */
        /* cmp 0xD1.. */
        /* jl  0xD300 */
        /* jg  0xD400 */
        /* jle 0xD500 */
        /* jge 0xD600 */

Output:

    OxF...

    /* cout r[D]  0xF00. */
    /* mem dump   0xF100 */
    /* reg dump   0xF200 */
    /* cout@ptr   0xF30. */
    /* cout@ptr\n 0xF40. */
    /* cout@mem   0xF50. */
    /* new line   0xF600 */

Input:

    0xE...

        /* mov r[D],int    0xE0.. */
        /* mov r[D],int    0xE1.. */
        /* cin r[D],int    0xE20. */
        /* mov r[D],str*   0xE30. */
        /* mov r[D],mem    0xE4.. */
        /* mov mem,r[C]    0xE5.. */
        /* mov mem,r[D]    0xE6.. */
        /* mov mem,int     0xE7.. */
        /* cin r[D],str*   0xE80. */
        /* mov r[D],*int[] 0xE90. */


Labels:

    0x0 - 0x9 are labels for now

Registers:

    regs[0x0]->regs[0xF]
