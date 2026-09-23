//Osnova8 CPU c++ emulator header file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
*/

#ifndef LIB_osnova_cpu
#define LIB_osnova_cpu

#include <cstdint>
#include <cstdio>
#include <math.h>
#include "LIB_osnova_utils.h"

//classes
class CPU
{
    private:

    //Flags (Conditions)
    bool EQU; //RA equals RB (Branch on RA == RB)
    bool FLGXN; //Pin Flag X Negated (Branch on 0)
    bool INTMN; //CPU's INTterrupt Mode Negated (Branch on IF == 1)
    bool CARRY; //ALU's operation results in a CARRY (Branch on RA ? RB > 255)
    bool NEG; //ALU's result is negative (Branch on RA ? RB < 0)
    bool ODD; //ALU's result is ODD (Branch on (RA ? RB)%2 == 1)
    bool NC; //No Condition, always branches (1)
    bool DONT; //DON'T, never branches (0)

    bool *Cnd[16]; //Cnd arguments table

    //Pointers and pointer file
    pnt PC; //Program Counter
    pnt AP; //Address Pointer
    pnt PCI; //Program Counter Interrupt
    pnt API; //Address Pointer Interrupt

    pnt *Pointers[4]; //Pointer file

    //Genereal purpose registers
    byte RA; //Register A
    byte RB; //Register B
    byte RC; //Register C
    byte RESR; //Reserve Register (Predicted to be used by Interrupt Handler program only)

    //Special purpose Register
    byte DVR; //Direct Value Register
    byte PS; //Pointer Selector (4-bit register, connected to bus with higher 4 bits; 4 thru 7)

    //Values derived from other states
    byte PFL; //Pointer File Low
    byte PFH; //Pointer File High
    byte SF; //Sector File

    byte SFPS; //Sector File (Lower part), Pointer Selector (Higher part)

    byte PRA; //Pointer Read Address (PS's lower part)
    byte PWA; //Pointer Write Address (PS's higher part)

    byte ALU; //Arithmetic Logic Unit

    byte *ADD; //ADdress Device

    //Instruction Register and derived values
    byte IR;
    byte Opc; //Opcode, higher 4-bit part of IR
    byte Arg; //Argument, lower 4-bit part of IR

    //ALU
    int op; //(S0, S1, S2, S3) pins (Selection (Of operation)) 
    bool mode; //M pin (Mode, 1 is for logic operations, 0 is for arithmetic operations)
    int cin; //Cn pin (Carry in (Pin is inverted in ALU))

    int stage; //CPU Cycle stage, affected by clock ticks (Clock is ticked by calling "updateCPU" function)
    int i; //Don't touch ^^'

    //BUS arguments table (Data sources)
    byte *Src[16];

    //Calculates ALU's operation result, ALU is 8-bit version of 74181 IC (equivalent of two 74181 cascaded)
    int calcALU(int A, int B, int op, bool mode, int cin);

    public:

    //Inputs and outputs
    byte *Bus; //Main CPU bus
    bool *IF; //CPU Interrupt mode flag (Inverted)
    bool *ITX; //Interrupt trigger external (Triggers interrupt in other devices) (Inverted)
    bool *Addw; //Address Write signal (Inverted)
    bool *Addr; //Address Read signal (Inverted)
    pnt *ADDB; //ADdress DEvice Bus (4-bit sectors, each sector 16-bit address space (1048576B = 1MB memory)

    //CPU update (execution cycle update)
    void update();
};

#endif

