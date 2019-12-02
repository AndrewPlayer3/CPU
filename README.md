# AssemblyProject

This is a 16-bit instruction set that operates on an "emulated" 32bit cpu.
There are two included test programs: boxprint and factorial.

These are the instructions:

Arithmetic:

    0xA...

        /* r[C] += r[D] 0xA0.. */
        /* r[C] -= r[D] 0xA1.. */
        /* r[C] *= r[D] 0xA2.. */
        /* r[C] %= r[D] 0xA3.. */
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
