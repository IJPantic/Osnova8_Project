//Osnova computer c++ emulator test
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
*/

/*
TODO napravi lib od ovoga, tako da kad emuliras bilo koji harwaare samo u main stavis funkciju od emulatora i unda hardawre u start i update prije main
TODO arg nek je opcionalan
TODO napravi funkciju s kojom korisnik moze odrediti raam koji ce korisiti ako napise poke ili dump npr
TODO dump command
TODO opc, arg i op table
TODO zasto PC ne broji?
TODO nadodj u snap command i flaagove
*/

#include <stdio.h>
#include <cstring>
#include "LIB_osnova_utils.h"

#include "LIB_osnova_cpu.h"
#include "LIB_4kb_ram.h"

//INITIALIZE HARDWARE
CPU MainCPU;
RAM MainRAM;

//Once on start
void start()
{
    //COMPUTER CONNECTION
    byte DataBus; //Computer data bus
    pnt AdrBus; //Computer ddress bus

    bool WE; //CPU write enable signal
    bool RE; //CPU read enable signal

    bool CIntD; //CPU interrupts ADD
    bool DIntC; //ADD interrupts CPU

    //MAIN CPU CONNECTION
    MainCPU.Bus = &DataBus; //Data bus
    MainCPU.ADDB = &AdrBus; //Address bus

    MainCPU.IF = &DIntC; //Device's interrupt to CPU
    MainCPU.ITX = &CIntD; //CPU's interrupt to device

    MainCPU.Addw = &WE; //Write enable signal
    MainCPU.Addr = &RE; //Read enable signal

    //MAIN RAM CONNECTION
    MainRAM.dataBus = &DataBus; //Data bus
    MainRAM.adrBus = &AdrBus; //Address bus

    MainRAM.WE = &WE; //Write enable signal
    MainRAM.RE = &RE; //Read enable signal
}

//Every clock tick
void update()
{
    MainRAM.update();
    MainCPU.update();
}

//GLOBALS



//MAIN
int main()
{
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
            void update();

            stage++;
            if(stage > 7) stage = 0; //Bounds it's value between 0 and 7

        }else if(!strcmp(cmd, cycle)) //Finish CPU cycle
        {
            for(int times = 0; arg1 < times; times++)
            {
                for(stage; stage < 7; stage++)
                {
                    void update();
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

        }else printf("This command does not exist\n");
    }
}

