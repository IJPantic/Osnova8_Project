//Osnova computer c++ emulator test
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
26. september, 2026.
28. september, 2026.
29. september, 2026.
30. september, 2026.
*/

#include <stdio.h>
#include <cstring>
#include "LIB_osnova_utils.h"

#include "LIB_osnova_cpu.h"
#include "LIB_4kb_ram.h"

/*
TODO napravi daa je ovo glavni program ali sadrzi #include main (lib) koji di ti unda pises kakav ce bit harwaare
TODO da memoriju mos stvorit raznih veicina da nije fiksno 4kb
TODO da mos isprintat arg i opc i alu table
TODO jnapravi daa se sve ispisuje u hex, a upisuje u ecimalnom ili 0x pa bude hex, također napravi
TODO napravi command koji ispisuje hex vrijednost u dec i obratno
TODO napravi threading tako da mos odvojeno diraat harwaarev i runat ga pri staalnoj frekvenciji
*/

//Argument decompile table (Turns argument number into argument's name)
const char *decompileArg(byte arg)
{
    switch(arg)
    {
        case 0: return "alu"; break;
        case 1: return "rc "; break;
        case 2: return "sf "; break;
        case 3: return "pfl"; break;
        case 4: return "pfh"; break;
        case 5: return "resr"; break;
        case 6: return "add"; break;
        case 7: return "dvr"; break;
        case 8: return "abc"; break;
        case 9: return "x  "; break;
        case 10: return "x  "; break;
        case 11: return "x  "; break;
        case 12: return "x  "; break;
        case 13: return "x  "; break;
        case 14: return "x  "; break;
        case 15: return "x  "; break;
    }

    return "Err";
}

//Opcode decompile table (Turns opcode number into opcode's name)
const char *decompileOpc(byte opc)
{
    switch(opc)
    {
        case 0: return "jmp  "; break;
        case 1: return "rc   "; break;
        case 2: return "sf   "; break;
        case 3: return "pfl  "; break;
        case 4: return "pfh  "; break;
        case 5: return "resr "; break;
        case 6: return "add  "; break;
        case 7: return "dvrr "; break;
        case 8: return "dvrl "; break;
        case 9: return "dvrh "; break;
        case 10: return "ra   "; break;
        case 11: return "rb   "; break;
        case 12: return "rc   "; break;
        case 13: return "intsi"; break;
        case 14: return "intsx"; break;
        case 15: return "inte "; break;
    }

    return "Err";
}

//ALU operation decompile table (Turns ALU op number into argument's name)
const char *decompileAluOp(int op, bool mode, int cin)
{
    if(mode) //Logic operations
    {
        switch(op)
        {
            case 0: return "!A"; break;
            case 1: return "!(A || B)"; break;
            case 2: return "(!A) && B"; break;
            case 3: return "0"; break;
            case 4: return "!(A && B)"; break;
            case 5: return "!B"; break;
            case 6: return "A ^ B"; break;
            case 7: return "A && (!B)"; break;
            case 8: return "(!A) || B"; break;
            case 9: return "!(A ^ B)"; break;
            case 10: return "B"; break;
            case 11: return "A && B"; break;
            case 12: return "1"; break;
            case 13: return "A || (!B)"; break;
            case 14: return "A || B"; break;
            case 15: return "A"; break;
        }
    }else //Arithmetic operations
    {
        if(cin) //Without carry in
        {
            switch(op)
            {
                case 0: return "A"; break;
                case 1: return "(A || B)"; break;
                case 2: return "(A || (!B))"; break;
                case 3: return "255"; break;
                case 4: return "A+(A && (!B))"; break;
                case 5: return "(A || B)+(A && (!B))"; break;
                case 6: return "A-B-1"; break;
                case 7: return "(A && (!B))-1"; break;
                case 8: return "A+(A && B)"; break;
                case 9: return "A+B"; break;
                case 10: return "(A || (!B))+(A && B)"; break;
                case 11: return "(A && B)-1"; break;
                case 12: return "A+A"; break;
                case 13: return "(A || B)+A"; break;
                case 14: return "(A || (!B))+A"; break;
                case 15: return "A-1"; break;
            }

        }else //With carry in
        {
            switch(op)
            {
                case 0: return "A +1"; break;
                case 1: return "(A || B) +1"; break;
                case 2: return "(A || (!B)) +1"; break;
                case 3: return "0"; break;
                case 4: return "A+(A && (!B)) +1"; break;
                case 5: return "(A || B)+(A && (!B)) +1"; break;
                case 6: return "A-B"; break;
                case 7: return "(A && (!B))"; break;
                case 8: return "A+(A && B) +1"; break;
                case 9: return "A+B +1"; break;
                case 10: return "(A || (!B))+(A && B) +1"; break;
                case 11: return "(A && B)"; break;
                case 12: return "A+A +1"; break;
                case 13: return "(A || B)+A +1"; break;
                case 14: return "(A || (!B))+A +1"; break;
                case 15: return "A"; break;
            }
        }
    }

    return "Err";
}

//INITIALIZE HARDWARE
CPU MainCPU;
RAM MainRAM;

//Every clock tick
void update()
{
    MainRAM.update();
    MainCPU.update();
}

//CONNECT HARDWARE
int main()
{
    int mems_num = 1;
    byte *mems[mems_num]; //Napraavi da je zasebna funckija koju user moze pokrenuti
    int mems_size[mems_num];

    int cpus_num = 1;
    CPU *cpus[cpus_num]; //Napraavi da je zasebna funckija koju user moze pokrenuti

    //COMPUTER CONNECTION
    byte DataBus; //CPU's data bus
    pnt AdrBus; //CPU's ddress bus

    bool WE = 1; //CPU's write enable signal
    bool RE = 1; //CPU's read enable signal

    bool IntD; //Interrupt ADD line
    bool IntC = 1; //Interrupt CPU line
    bool FlgXN = 1; //CPU's flag x pin
    bool CE = 0; //RAM's chip enable signal

    //MAIN CPU CONNECTION
    cpus[0] = &MainCPU;

    MainCPU.Bus = &DataBus; //Data bus
    MainCPU.ADDB = &AdrBus; //Address bus

    MainCPU.IF = &IntC; //Interrupt CPU line
    MainCPU.ITX = &IntD; //Interrupt ADD line
    MainCPU.FX = &FlgXN; //CPU's flag x pin

    MainCPU.Addw = &WE; //Write enable signal
    MainCPU.Addr = &RE; //Read enable signal

    //MAIN RAM CONNECTION
    mems[0] = MainRAM.memory;
    mems_size[0] = 4096;

    MainRAM.dataBus = &DataBus; //Data bus
    MainRAM.adrBus = &AdrBus; //Address bus

    MainRAM.WE = &WE; //Write enable signal
    MainRAM.RE = &RE; //Read enable signal
    MainRAM.CE = &CE; //Chip enable signal

    //TODO OVO iznad nije dio emulator programa

    //Emulator commands
    char done[] = "done"; //Exits program
    char step[] = "step"; //Does one clock tick
    char cycle[] = "cycle"; //Does clock ticks until it finishes current cycle (Goes to the next multiple of 8)
    char dump[] = "dump"; //Prints (Dumps) memory contents
    char snap[] = "snap"; //Prints all CPU states
    char poke[] = "poke"; //Changes a value at a certain memory address
    char peek[] = "peek"; //Prints a value at a certain memory address
    char help[] = "help"; //Prints all commands and how to use them
    //char sim[] = "sim"; //?? TODO
    //char list[] = "list"; //?? TODO

    char cmd[16]; //Command (Emulator command)

    int arg1, arg2, arg3; //Command integer arguments
    //char str1[16]; //Command string arguments

    int stage = 0; //Cycle's stage

    //Short program notice
    printf("Osnova c++ emulator, type 'help' to list all commands and their arguments\n");
    printf("Created by Ivan Jonjic (IJPantic on gitgub), GPL V3\n \n");

    //EMULATOR CONTROLS
    while(true)
    {
        //Read command
        printf(" > ");
        scanf("%s", cmd);

        if(!strcmp(cmd, done)) //Exits program
        {
            return 0;

        }else if(!strcmp(cmd, step)) //Clock tick
        {
            update();

            stage++;
            if(stage > 7) stage = 0; //Bounds it's value between 0 and 7

        }else if(!strcmp(cmd, cycle)) //Finish CPU cycle
        {
            scanf("%d", &arg1); //Reads argument TIMES

            for(int times = 0; times < arg1; times++)
            {
                for(stage; stage < 7; stage++)
                {
                    update();
                }

                stage = 0;  
            }

        }else if(!strcmp(cmd, dump)) //dump memory contents
        {
            scanf("%d", &arg1); //Reads argument MEM

            byte *memory = mems[arg1]; //Selected memory to dump

            if(mems_size[arg1] > 65536) continue; //Stops command for executing if memory size is too big

            for(int y = 0; y < mems_size[arg1]; y += 64) //Print rows
            {
                //String spacing
                if(y < 10)
                {
                    printf("%d:     ", y);
                }else if(y < 100)
                {
                    printf("%d:    ", y);
                }else if(y < 1000)
                {
                    printf("%d:   ", y);
                }else if(y < 10000)
                {
                    printf("%d:  ", y);
                }else 
                    printf("%d: ", y);

                //Print rows
                for(int x = 0; x < 64; x++)
                {
                    byte value = memory[y+x]; //Value at a certain address in memory

                    //String spacing
                    if(memory[y+x] > 0xe) printf("%x ", value);
                    if(memory[y+x] < 0xf) printf("0%x ", value);
                }

                printf("\n");
            }

        }else if(!strcmp(cmd, snap)) //Snapshot CPU
        {
            scanf("%d", &arg1); //Reads argument CPU
            CPU cpu = *cpus[arg1];

            printf("REG;    RA:%d     RB:%d     RC:%d     RESR:%d  DVR:%d  \n", cpu.RA, cpu.RB, cpu.RC, cpu.RESR, cpu.DVR);
            printf("INST;   Stage:%d  IR:%d     Arg:%s Opc:%s           \n", stage, cpu.IR, decompileArg(cpu.Arg), decompileOpc(cpu.Opc));
            printf("PC;     PFL:%d    PFH:%d    SF:%d                      \n", trim(cpu.PC, 0, 8), trim(cpu.PC, 8, 16), trim(cpu.PC, 16, 20));
            printf("AP;     PFL:%d    PFH:%d    SF:%d                      \n", trim(cpu.AP, 0, 8), trim(cpu.AP, 8, 16), trim(cpu.AP, 16, 20));
            printf("PCI;    PFL:%d    PFH:%d    SF:%d                      \n", trim(cpu.PCI, 0, 8), trim(cpu.PCI, 8, 16), trim(cpu.PCI, 16, 20));
            printf("API;    PFL:%d    PFH:%d    SF:%d                      \n", trim(cpu.API, 0, 8), trim(cpu.API, 8, 16), trim(cpu.API, 16, 20));
            printf("PS;     SFPS:%d   PS:%d     PRA:%d    PWA:%d           \n", cpu.SFPS, cpu.PS, cpu.PRA, cpu.PWA);
            printf("ALU;    ALU:%d    Op:%s                                \n", cpu.ALU, decompileAluOp(cpu.op, cpu.mode, cpu.cin));
            printf("FLG;    EQU:%d    FLGXN:%d  INTMN:%d  NEG:%d   ODD:%d  \n", cpu.RA, cpu.RB, cpu.RC, cpu.RESR, cpu.DVR);
            printf("\n");

        }else if(!strcmp(cmd, poke)) //Poke memory address
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR
            scanf("%d", &arg3); //Reads argument VAL

            mems[arg1][arg2] = arg3;

        }else if(!strcmp(cmd, peek)) //Peek memory address
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR

            printf("%d \n", mems[arg1][arg2]);

        }else if(!strcmp(cmd, help)) //Display help list
        {
            printf("done -> Exits this program\n");
            printf("step -> Ticks clock once\n");
            printf("cycle TIMES -> Finishes whole CPU execution cycle TIMES times\n");
            printf("dump MEM -> Prints all contents of a MEM memory\n");
            printf("snap CPU -> Prints all current states CPU cpu states\n");
            printf("poke MEM ADR VAL-> Writes VAL value to a memory MEM at address ADR\n");
            printf("peek MEM ADR -> Prints a memory value from memory MEM at address ADR\n");
            printf("help -> Prints help list of all commnds (This is help command)\n");
            printf("sim ACT -> ACT 'run' starts simulation, ACT 'stop' stops simulation and ACT 'speed' has additional argument 'F' which sets simulation's frequency\n");
            printf("list TABLE -> Prints a table list of TABLE elements, TABLE 'opc' prints all opcodes, 'arg' prints all arguments, 'alu' prints all ALU operations\n");
            printf("\n");

        /*}else if(!strcmp(cmd, sim)) //
        {
            /*scanf("%d", &str1); //Reads argument ACT

            if(str1 == "run")

            printf("%d \n", mems[arg1][arg2]);

        }else if(!strcmp(cmd, list)) //
        {
            scanf("%15s", str1); //Reads argument ACT

            if(!strcmp(str1, "opc"))
            {
                for(int i = 0; i < 16; i++)
                    printf("%d %s \n", i, decompileArg(i));

            }else if(str1 == "arg")
            {


            }else if(str1 == "alu")
            {

            }*/

        }else printf("This command does not exist\n \n");
    }
}

