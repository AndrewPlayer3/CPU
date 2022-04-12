# AssemblyProject

This is a 16-bit instruction set that operates on an "emulated" 32bit cpu,
along with an assembler for a custom assembly language.

There are several included demo programs in the programs folder. 

### Example Program:
This is what an implementation of Collatz looks like in the assembly:
```asm
# Collatz Function

.main
    mov r10 Enter an Integer:\0
    pln r10
    ini r0
    mov r11 \0
    mov r1 1
    pln r11

    parity_test:
        out r0
        pln r11
        cmp r0 r1
        je finish_collatz
        mov r6 r0
        mov r7 0
        mod r6 2
        cmp r6 r7
        jg odd
        jmp even

    even:
        div r0 2
        jmp parity_test

    odd:
        mul r0 3
        add r0 1
        jmp parity_test


    finish_collatz:
        #we done now boys
        mov r0 10
        psh r0
```
And this is what the 'machine code' looks like:
```
<<<===Main===>>> 
0xff00
0xe30a
Enter an Integer:\0
0xf40a
0xe200
0xe30b
\0
0xe001
0x0001
0xf40b
# Label: parity_test
0xdf00
0x0001
0xf000
0xf40b
0xd101
0xd200
0x0002
0xe160
0xe007
0x0000
0xa806
0x0002
0xd167
0xd400
0x0003
0xd000
0x0004
# Label: even
0xdf00
0x0004
0xa900
0x0002
0xd000
0x0001
# Label: odd
0xdf00
0x0003
0xa700
0x0003
0xa500
0x0001
0xd000
0x0001
# Label: finish_collatz
0xdf00
0x0002
0xe000
0x000a
0xeb00
```
Note: Anything prepended with # is a comment and is ignored when loading the program, and strings such as 'Enter an Integer:\0' and parsed into their constituent bytes when the program is loaded.

### Build and Usage Instructions:

To build the program to you can run the build script, or run these commands:
```bash
mkdir bin
g++ -std=c++17 -O3 src/cpu_main.cpp src/instr.cpp -o bin/cpu
g++ -std=c++17 -O3 src/assembler_main.cpp src/assembler.cpp -o bin/assembler
```
To run (assemble and execute) a test program:
```
bin/run programs/filename
```
To run the sandbox assembly shell:
```
bin/ish
```
To assemble one of the test programs:
```
bin/assembler programs/filename
```
To run the compiled test programs:
```
bin/cpu programs/filename.inst
```

### Simple Instruction Overview:

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
