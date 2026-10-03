//Osnova8 CPU c++ emulator library header file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
24. september, 2026.
2. October, 2026.
3. October, 2026.
*/

#include "LIB_osnova_utils.h"

#ifndef LIB_osnova_cpu
#define LIB_osnova_cpu

//classes
class CPU
{
    public:
        CPU(); //Constructor

        //Flags (Conditions)
        bool equ; //RA equals RB (Branch on RA == RB)
        bool flgxn; //Pin Flag X Negated (Branch on 0)
        bool intmn; //CPU's INTterrupt Mode Negated (Branch on IF == 1)
        bool carry; //ALU's operation results in a CARRY (Branch on RA ? RB > 255)
        bool neg; //ALU's result is negative (Branch on RA ? RB < 0)
        bool odd; //ALU's result is ODD (Branch on (RA ? RB)%2 == 1)
        bool NC; //No Condition, always branches (1)
        bool DONT; //DON'T, never branches (0)

        bool *cnd[16]; //Cnd arguments table

        //Pointers and pointer file
        pnt_t pc; //Program Counter
        pnt_t ap; //Address Pointer
        pnt_t pci; //Program Counter Interrupt
        pnt_t api; //Address Pointer Interrupt

        pnt_t *pf[4]; //Pointer file

        //Genereal purpose registers
        byte_t ra; //Register A
        byte_t rb; //Register B
        byte_t rc; //Register C
        byte_t resr; //Reserve Register (Predicted to be used by Interrupt Handler program only)

        //Special purpose Register
        byte_t dvr; //Direct Value Register
        byte_t ps; //Pointer Selector (4-bit register, connected to bus with higher 4 bits; 4 thru 7)

        //Values derived from other states
        byte_t pfl; //Pointer File Low
        byte_t pfh; //Pointer File High
        byte_t sf; //Sector File

        byte_t sfps; //Sector File (Lower part), Pointer Selector (Higher part)

        byte_t pra; //Pointer Read Address (PS's lower part)
        byte_t pwa; //Pointer Write Address (PS's higher part)

        byte_t alu; //Arithmetic Logic Unit

        byte_t add; //ADdress Device

        //Instruction Register and derived values
        byte_t ir;
        byte_t opc; //Opcode, higher 4-bit part of IR
        byte_t arg; //Argument, lower 4-bit part of IR

        //ALU
        int op; //(S0, S1, S2, S3) pins (Selection (Of operation)) 
        bool mode; //M pin (Mode, 1 is for logic operations, 0 is for arithmetic operations)
        int cin; //Cn pin (Carry in (Pin is inverted in ALU))

        int stage; //CPU Cycle stage, affected by clock ticks (Clock is ticked by calling "updateCPU" function)
        int i; //Don't touch ^^'

        //BUS arguments table (Data sources)
        byte_t *src[16];

        //Calculates ALU's operation result, ALU is 8-bit version of 74181 IC (equivalent of two 74181 cascaded)
        byte_t calc_alu(byte_t A, byte_t B, int op, bool mode, int cin);

        //Inputs and outputs
        byte_t *io_db; //Main CPU Data Bus
        bool *io_fx; //Flag X CPU pin (Inverted)
        bool *io_if; //CPU Interrupt mode Flag (Inverted)
        bool *io_itx; //Interrupt Trigger External (Triggers interrupt in other devices) (Inverted)
        bool *io_addw; //ADdress Device Write signal (Inverted)
        bool *io_addr; //ADdress Device Read signal (Inverted)
        pnt_t *io_adrb; //ADdRess Bus (4-bit sectors, each sector 16-bit address space (1048576B = 1MB memory)

        //CPU update (execution cycle update)
        void update();
};

#endif

