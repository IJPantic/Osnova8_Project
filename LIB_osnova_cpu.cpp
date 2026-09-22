//Osnova8 CPU c++ emulator library
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
10. september, 2026.
12. september, 2026.
13. september, 2026.
18. september, 2026.
19. september, 2026.
20. september, 2026.
21. september, 2026.
22. september, 2026.
*/

#include <cstdint>
#include <cstdio>
#include <math.h>
#include "LIB_osnova_utils.h"

#include "LIB_osnova_cpu.h" //Header file of this library

//classes
class CPU;

//Defining CPU
class CPU
{
    public:

    //Inputs and outputs
    byte *Bus; //Main CPU bus
    bool *IF; //CPU Interrupt mode flag (Inverted)
    bool *ITX; //Interrupt trigger external (Triggers interrupt in other devices) (Inverted)
    bool *Addw; //Address Write signal (Inverted)
    bool *Addr; //Address Read signal (Inverted)
    pnt *ADDB; //ADdress DEvice Bus (4-bit sectors, each sector 16-bit address space (1048576B = 1MB memory)

    private:

    //Flags (Conditions)
    bool EQU; //RA equals RB (Branch on RA == RB)
    bool FLGXN; //Pin Flag X Negated (Branch on 0)
    bool INTMN; //CPU's INTterrupt Mode Negated (Branch on IF == 1)
    bool CARRY; //ALU's operation results in a CARRY (Branch on RA ? RB > 255)
    bool NEG; //ALU's result is negative (Branch on RA ? RB < 0)
    bool ODD; //ALU's result is ODD (Branch on (RA ? RB)%2 == 1)
    bool NC = 1; //No Condition, always branches (1)
    bool DONT = 0; //DON'T, never branches (0)

    bool *Cnd[16] = { //Cnd arguments table
        &NC, &EQU, &FLGXN, &INTMN, &CARRY, &NEG, &ODD, &DONT,
        &NC, &EQU, &FLGXN, &INTMN, &CARRY, &NEG, &ODD, &DONT //Table repeats after first 8 elements
    };

    //Pointers and pointer file
    pnt PC = 0; //Program Counter
    pnt AP = 0; //Address Pointer
    pnt PCI = 0; //Program Counter Interrupt
    pnt API = 0; //Address Pointer Interrupt

    pnt *Pointers[4] = {&PC, &AP, &PCI, &API}; //Pointer file

    //Genereal purpose registers
    byte RA = 0; //Register A
    byte RB = 0; //Register B
    byte RC = 0; //Register C
    byte RESR = 0; //Reserve Register (Predicted to be used by Interrupt Handler program only)

    //Special purpose Register
    byte DVR = 0; //Direct Value Register
    byte PS = 0; //Pointer Selector (4-bit register, connected to bus with higher 4 bits; 4 thru 7)

    //Values derived from other states
    byte PFL; //Pointer File Low
    byte PFH; //Pointer File High
    byte SF; //Sector File

    byte SFPS; //Sector File (Lower part), Pointer Selector (Higher part)

    byte PRA; //Pointer Read Address (PS's lower part)
    byte PWA; //Pointer Write Address (PS's higher part)

    byte ALU; //Arithmetic Logic Unit

    byte *ADD = Bus; //ADdress Device

    //Instruction Register and derived values
    byte IR;
    byte Opc; //Opcode, higher 4-bit part of IR
    byte Arg; //Argument, lower 4-bit part of IR

    //ALU
    int op; //(S0, S1, S2, S3) pins (Selection (Of operation)) 
    bool mode; //M pin (Mode, 1 is for logic operations, 0 is for arithmetic operations)
    int cin; //Cn pin (Carry in (Pin is inverted in ALU))

    int stage = 0; //CPU Cycle stage, affected by clock ticks (Clock is ticked by calling "updateCPU" function)
    int i = 0; //Don't touch ^^'

    //BUS arguments table (Data sources)
    byte *Src[16] = {
        &ALU, //0
        &RC, //1
        &SFPS, //2
        &PFL, //3
        &PFH, //4
        &RESR, //5
        ADD, //6, It is just a mirror value of the "*Bus" pointer
        &DVR, //7
        &FF, //8
        &FF, //9
        &FF, //10 (A)
        &FF, //11 (B)
        &FF, //12 (C)
        &FF, //13 (D)
        &FF, //14 (E)
        &FF, //15 (F)
    };

    //Calculates ALU's operation result, ALU is 8-bit version of 74181 IC (equivalent of two 74181 cascaded)
    int calcALU(int A, int B, int op, bool mode, int cin)
    {
        cin = !cin; //Inverting Cin pin of ALU

        if(mode) //Logic operations
        {
            switch(op)
            {
                case 0: return !A; break;
                case 1: return !(A || B); break;
                case 2: return (!A) && B; break;
                case 3: return 0; break;
                case 4: return !(A && B); break;
                case 5: return !B; break;
                case 6: return A ^ B; break;
                case 7: return A && (!B); break;
                case 8: return (!A) || B; break;
                case 9: return !(A ^ B); break;
                case 10: return B; break;
                case 11: return A && B; break;
                case 12: return 1; break;
                case 13: return A || (!B); break;
                case 14: return A || B; break;
                case 15: return A; break;
            }
        }else //Arithmetic operations
        {
            switch(op)
            {
                case 0: return A +cin; break;
                case 1: return (A || B) +cin; break;
                case 2: return (A || (!B)) +cin; break;
                case 3: return 255 +cin; break;
                case 4: return A+(A && (!B)) +cin; break;
                case 5: return (A || B)+(A && (!B)) +cin; break;
                case 6: return A-B-1 +cin; break;
                case 7: return (A && (!B))-1 +cin; break;
                case 8: return A+(A && B) +cin; break;
                case 9: return A+B +cin; break;
                case 10: return (A || (!B))+(A && B) +cin; break;
                case 11: return (A && B)-1 +cin; break;
                case 12: return A+A +cin; break;
                case 13: return (A || B)+A +cin; break;
                case 14: return (A || (!B))+A +cin; break;
                case 15: return A-1 +cin; break;
            }
        }
    }

    //CPU update, execution cycle update
    void updateCPU()
    {
        stage++; //Updating cycle stage

        //Exection cycle
        switch(stage)
        {
            //1. INSTRUCTION FETCH
            case 0: //Stage 0
                i = 0; //Interrupt mode check
                if(IF) i = 2;

                *ADDB = *Pointers[i]; //Setting address of a current instruction
                *Addr = 0; //Releasing PC (Or PCI, depending on interrupt flag state)
            break;

            case 1: //Stage 1
                IR = *Bus; //Fetching instruction into IR
                Opc = trim(IR, 4, 8); //Updating opcode value
                Arg = trim(IR, 0, 4); //Updating argument value
            break;

            case 2: //Stage 2
                //No changes
            break;

            //2. INSTRUCTION EXECUTE
            case 3: //Stage 3
                *Addr = 1; //Ending fetch stage

                //Setting address of a current pointer selected by PS
                PRA = trim(PS, 0, 2);
                PWA = trim(PS, 2, 4);
                *ADDB = *Pointers[PRA];

                PFL = trim(*Pointers[PRA], 0, 8); //Updating pointer file low source
                PFH = trim(*Pointers[PRA], 8, 16); //Updating pointer file high source
                SFPS = (trim(*Pointers[PRA], 16, 20) <<8 ) + trim(PS, 0, 4); //Updating sector file, pointer selector source

                //Updating ALU source
                op = trim(DVR, 0, 4);
                mode = trim(DVR, 4, 5);
                cin = trim(DVR, 5, 6);
                ALU = calcALU(RA, RB, op, mode, cin);

                //Decoding argument part of instruction
                if(Arg == 6) //Index of ADD data source
                    Addr = 0; //Gives signal to address device to output it's value to CPU'a data bus
                else
                    *Bus = *Src[Arg]; //Releases data sources from within CPU

                //Flags check
                EQU = RA == RB; //Are registers A and B equal?
                INTMN = IF; //Is CPU in interrupt mode?
                CARRY = RA+RB > FF; //Is there a carry?
                NEG = ALU >= 128; //Is MSB one?
                ODD = ALU%2 == 1; //Is LSB one?
            break;

            case 4: //Stage 4
                switch(Opc) //Decoding opcode part of instruction
                {
                    //JuMP (Branches on Cnd == 1), copies value from one pointer to another (if the Arg's MSB is 0, transfered pointer is incremented)
                    case 0: if(*Cnd[Arg]) *Pointers[PRA] = *Pointers[PWA] +trim(Arg, 3, 4); break;

                    case 1: RC = *Bus; break; //Register C (SRC -> RC)

                    case 2: SF = *Bus; break; //Sector File (SRC -> selected pointer of SF)

                    case 3: PFL = *Bus; break; //Pointer File Low (SRC -> selected pointer of PFL)

                    case 4: PFH = *Bus; break; //Pointer File High (SRC -> selected pointer of PFH)

                    case 5: RESR = *Bus; break; //REServed Register (SRC -> RESR)

                    case 6: Addw = 0; break; //Address device write (SRC -> selected address of ADD)

                    case 7: DVR = 0; break; //Direct Value Register Reset (DVR = 0), sets value to zero

                    case 8: DVR += DVR &(255-15) +Arg; break; //Direct Value Register Low (ARG part of IR -> low part of DVR), overwrites lower part of DVR

                    case 9: DVR += DVR &15 +Arg; break; //Direct Value Register High (ARG part of IR -> high part of DVR), overwrites higher part of DVR

                    case 10: RA = *Bus; break; //Register A (SRC -> RA)

                    case 11: RB = *Bus; break; //Register B (SRC -> RB)

                    case 12: PS = trim(*Bus, 4, 8); break; //Pointer Selector (SRC -> PS), note its size and the way it is connected

                    case 13: *IF = 0; break; //Interrupt Set Internal (Inverted Set), switches on CPU's interrupt mode

                    case 14: *ITX = 0; break; //Interrupt Set External (Inverted Set)

                    case 15: *IF = 1; break; //Interrupt Enable (Inverted Reset), switches off CPU's interrupt mode
                }
            break;

            case 5: //Stage 5
                //No changes
            break;

            //3. POINTER INCREMENT
            case 6: //Stage 6
                *Bus = FF; //Ending execution stage
                *ITX = 1;
                *Addw = 1;

                i = 0; //Interrupt mode check
                if(IF) i = 2;

                *Addr = 0; //Releasing PC (Or PCI, depending on interrupt flag state)
            break;

            case 7: //Stage 7
                *Pointers[i]++; //Incrementing PC (Or PCI, depending on interrupt flag state)
                if(*Pointers[i] > adrAREA) *Pointers[i] = 0; //Spill check (Counting can only occur in address area of ADD, first 16 bits)
                *ADDB = *Pointers[i]; //Setting address of the next instruction

                stage = 0; //Resets cycle
            break;
        }
    }
};

