//Osnova8 CPU c++ emulator library file
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
23. september, 2026.
24. september, 2026.
25. september, 2026.
30. september, 2026.
*/

#include "LIB_osnova_cpu.h" //Header file of this library

//Defining the Osnova8 CPU
CPU::CPU()
{
    //Flags (Conditions)
    EQU; //RA equals RB (Branch on RA == RB)
    FLGXN; //Pin Flag X Negated (Branch on 0)
    INTMN = 1; //CPU's INTterrupt Mode Negated (Branch on IF == 1)
    CARRY; //ALU's operation results in a CARRY (Branch on RA ? RB > 255)
    NEG; //ALU's result is negative (Branch on RA ? RB < 0)
    ODD; //ALU's result is ODD (Branch on (RA ? RB)%2 == 1)
    NC = 1; //No Condition, always branches (1)
    DONT = 0; //DON'T, never branches (0)

    //Cnd arguments table
    Cnd[0] = &NC;
    Cnd[1] = &EQU;
    Cnd[2] = &FLGXN;
    Cnd[3] = &INTMN;
    Cnd[4] = &CARRY;
    Cnd[5] = &NEG;
    Cnd[6] = &ODD;
    Cnd[7] = &DONT;
    Cnd[8] = &NC; //Table repeats after first 8 elements
    Cnd[9] = &EQU;
    Cnd[10] = &FLGXN;
    Cnd[11] = &INTMN;
    Cnd[12] = &CARRY;
    Cnd[13] = &NEG;
    Cnd[14] = &ODD;
    Cnd[15] = &DONT;

    //Pointers and pointer file
    PC = 0; //Program Counter
    AP = 0; //Address Pointer
    PCI = 0; //Program Counter Interrupt
    API = 0; //Address Pointer Interrupt

    //Pointer file
    Pointers[0] = &PC;
    Pointers[1] = &AP;
    Pointers[2] = &PCI;
    Pointers[3] = &API;

    //Genereal purpose registers
    RA = 0; //Register A
    RB = 0; //Register B
    RC = 0; //Register C
    RESR = 0; //Reserve Register (Predicted to be used by Interrupt Handler program only)

    //Special purpose Register
    DVR = 0; //Direct Value Register
    PS = 0; //Pointer Selector (4-bit register, connected to bus with higher 4 bits; 4 thru 7)

    //Values derived from other states
    PFL; //Pointer File Low
    PFH; //Pointer File High
    SF; //Sector File

    SFPS; //Sector File (Lower part), Pointer Selector (Higher part)

    PRA; //Pointer Read Address (PS's lower part)
    PWA; //Pointer Write Address (PS's higher part)

    ALU; //Arithmetic Logic Unit

    ADD; //ADdress Device

    //Instruction Register and derived values
    IR;
    Opc; //Opcode, higher 4-bit part of IR
    Arg; //Argument, lower 4-bit part of IR

    //ALU
    op; //(S0, S1, S2, S3) pins (Selection (Of operation)) 
    mode; //M pin (Mode, 1 is for logic operations, 0 is for arithmetic operations)
    cin; //Cn pin (Carry in (Pin is inverted in ALU))

    stage = 0; //CPU Cycle stage, affected by clock ticks (Clock is ticked by calling "updateCPU" function)
    i = 0; //Don't touch ^^'

    //BUS arguments table (Data sources)
    Src[0] = &ALU;
    Src[1] = &RC;
    Src[2] = &SFPS;
    Src[3] = &PFL;
    Src[4] = &PFH;
    Src[5] = &RESR;
    Src[6] = &ADD;
    Src[7] = &DVR;
    Src[8] = &FF;
    Src[9] = &FF;
    Src[10] = &FF;
    Src[11] = &FF;
    Src[12] = &FF;
    Src[13] = &FF;
    Src[14] = &FF;
    Src[15] = &FF;

    //Inputs and outputs
    *Bus; //Main CPU bus
    *FX; //Flag X CPU pin (Inverted)
    *IF; //CPU Interrupt mode flag (Inverted)
    *ITX; //Interrupt trigger external (Triggers interrupt in other devices) (Inverted)
    *Addw; //Address Write signal (Inverted)
    *Addr; //Address Read signal (Inverted)
    *ADDB; //ADdress DEvice Bus (4-bit sectors, each sector 16-bit address space (1048576B = 1MB memory)
};

//Calculates ALU's operation result, ALU is 8-bit version of 74181 IC (equivalent of two 74181 cascaded)
byte CPU::calcALU(byte A, byte B, int op, bool mode, int cin)
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

    return 0;
}

//CPU update (execution cycle update)
void CPU::update()
{
    stage++; //Updating cycle stage

    //Exection cycle
    switch(stage)
    {
        //1. INSTRUCTION FETCH
        case 0: //Stage 0
            i = 0; //Interrupt mode check
            if(INTMN) i = 2;

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
            ADD = *Bus;

            PFL = trim(*Pointers[PRA], 0, 8); //Updating pointer file low source
            PFH = trim(*Pointers[PRA], 8, 16); //Updating pointer file high source
            SFPS = (trim(*Pointers[PRA], 16, 20) <<8 ) + trim(PS, 0, 4); //Updating sector file, pointer selector source

            //Updating ALU source
            op = trim(DVR, 0, 4);
            mode = trim(DVR, 4, 5);
            cin = trim(DVR, 5, 6);
            ALU = calcALU(RA, RB, op, mode, cin);

            /*
            //Decoding argument part of instruction
            if(Arg == 6) //Index of ADD data source
                Addr = 0; //Gives signal to address device to output it's value to CPU'a data bus
            else
                *Bus = *Src[Arg]; //Releases data sources from within CPU
            */
            *Bus = *Src[Arg]; //Releases data sources from within CPU

            //Flags check
            EQU = RA == RB; //Are registers A and B equal?
            FLGXN = !*FX; //Is flag x CPU pin zero?
            INTMN = !*IF; //Is CPU not in interrupt mode?
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

                case 13: INTMN = 0; break; //Interrupt Set Internal (Inverted Set), switches on CPU's interrupt mode

                case 14: *ITX = 0; break; //Interrupt Set External (Inverted Set)

                case 15: INTMN = 1; break; //Interrupt Enable (Inverted Reset), switches off CPU's interrupt mode
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
            if(INTMN) i = 2;

            *Addr = 0; //Releasing PC (Or PCI, depending on interrupt flag state)
        break;

        case 7: //Stage 7
            (*Pointers[i])++; //Incrementing PC (Or PCI, depending on interrupt flag state)
            if(*Pointers[i] > adrAREA) *Pointers[i] = 0; //Spill check (Counting can only occur in address area of ADD, first 16 bits)
            *ADDB = *Pointers[i]; //Setting address of the next instruction

            stage = 0; //Resets cycle
        break;
    }
}

