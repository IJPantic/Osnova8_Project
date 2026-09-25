//Osnova computer c++ emulator test
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
*/

/*
TODO napravi lib od ovoga, tako da kad emuliras bilo koji harwaare samo u main stavis funkciju od emulatora i unda hardawre u start i update prije main
TODO arg nek je opcionalan
TODO napravi funkciju s kojom korisnik moze odrediti raam koji ce korisiti ako napise poke ili dump npr
TODO dump command
TODO opc, arg i op table
*/

#include <stdio.h>
#include <cstring>
#include "LIB_osnova_utils.h"

#include "LIB_osnova_cpu.h"
#include "LIB_4kb_ram.h"

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
    //COMPUTER CONNECTION
    byte DataBus; //CPU's data bus
    pnt AdrBus; //CPU's ddress bus

    bool WE; //CPU's write enable signal
    bool RE; //CPU's read enable signal

    bool IntD; //Interrupt ADD line
    bool IntC = 1; //Interrupt CPU line
    bool FlgXN = 1; //CPU's flag x pin
    bool CE = 0; //RAM's chip enable signal

    //MAIN CPU CONNECTION
    MainCPU.Bus = &DataBus; //Data bus
    MainCPU.ADDB = &AdrBus; //Address bus

    MainCPU.IF = &IntC; //Interrupt CPU line
    MainCPU.ITX = &IntD; //Interrupt ADD line
    MainCPU.FXN = &FlgXN; //CPU's flag x pin

    MainCPU.Addw = &WE; //Write enable signal
    MainCPU.Addr = &RE; //Read enable signal

    //MAIN RAM CONNECTION
    MainRAM.dataBus = &DataBus; //Data bus
    MainRAM.adrBus = &AdrBus; //Address bus

    MainRAM.WE = &WE; //Write enable signal
    MainRAM.RE = &RE; //Read enable signal
    MainRAM.CE = &CE; //Chip enable signal



    //Emulator commands
    char done[] = "done"; //Exits program
    char step[] = "step"; //Does one clock tick
    char cycle[] = "cycle"; //Does clock ticks until it finishes current cycle (Goes to the next multiple of 8)
    char dump[] = "dump"; //Prints (Dumps) memory contents
    char snap[] = "snap"; //Prints all CPU states
    char poke[] = "poke"; //Changes a value at a certain memory address
    char peek[] = "peek"; //Prints a value at a certain memory address
    char help[] = "help"; //Prints all commands and how to use them

    char cmd[16]; //Command (Emulator)

    int arg1, arg2; //Comamnd argument

    int stage = 0; //Cycle's stage

    //Short program notice
    printf("Osnova c++ emulator, type 'help' to list all commands and their arguments\n");
    printf("Created by Ivan Jonjic (IJPantic on gitgub), GPL V3\n \n");

    while(true)
    {
        //EMULATOR CONTROLS
        printf(" > ");
        scanf("%s %d %d", cmd, &arg1, &arg2);

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
            

        }else if(!strcmp(cmd, snap)) //Snapshot CPU
        {
            printf("REG;    RA:%d     RB:%d     RC:%d     RESR:%d  DVR:%d  \n", MainCPU.RA, MainCPU.RB, MainCPU.RC, MainCPU.RESR, MainCPU.DVR);
            printf("INST;   Stage:%d  IR:%d     Arg:%s Opc:%s           \n", stage, MainCPU.IR, "TODO", "TODO");
            printf("PC;     PFL:%d    PFH:%d    SF:%d                      \n", trim(MainCPU.PC, 0, 8), trim(MainCPU.PC, 8, 16), trim(MainCPU.PC, 16, 20));
            printf("AP;     PFL:%d    PFH:%d    SF:%d                      \n", trim(MainCPU.AP, 0, 8), trim(MainCPU.PC, 8, 16), trim(MainCPU.PC, 16, 20));
            printf("PCI;    PFL:%d    PFH:%d    SF:%d                      \n", trim(MainCPU.PCI, 0, 8), trim(MainCPU.PC, 8, 16), trim(MainCPU.PC, 16, 20));
            printf("API;    PFL:%d    PFH:%d    SF:%d                      \n", trim(MainCPU.API, 0, 8), trim(MainCPU.PC, 8, 16), trim(MainCPU.PC, 16, 20));
            printf("PS;     SFPS:%d   PS:%d     PRA:%d    PWA:%d           \n", MainCPU.SFPS, MainCPU.PS, MainCPU.PRA, MainCPU.PWA);
            printf("ALU;    ALU:%d    Op:%s                                \n", MainCPU.ALU, "TODO");
            printf("FLG;    EQU:%d    FLGXN:%d  INTMN:%d  NEG:%d   ODD:%d  \n", MainCPU.RA, MainCPU.RB, MainCPU.RC, MainCPU.RESR, MainCPU.DVR);
            printf("\n");

        }else if(!strcmp(cmd, poke)) //Poke memory address
        {
            MainRAM.memory[arg1] = arg2;

        }else if(!strcmp(cmd, peek)) //Peek memory address
        {
            printf("%d \n", MainRAM.memory[arg1]);

        }else if(!strcmp(cmd, help)) //Display help list
        {
            printf("done -> Exits this program\n");
            printf("step -> Ticks clock once\n");
            printf("cycle TIMES -> Finishes whole CPU execution cycle TIMES times\n");
            printf("dump MEM -> Prints all contents of a MEM memory\n");
            printf("snap -> Prints all current states CPU states\n");
            printf("poke ADR -> Prints a memory value at address ADR\n");
            printf("peek ADR VAL -> Writes VAL value to a memory address ADR\n");
            printf("help -> Prints help list of all commnds (This is help command)\n");
            printf("\n");

        }else printf("This command does not exist\n \n");
    }
}

