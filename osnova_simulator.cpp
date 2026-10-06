//Osnova computer c++ simulator, version 1, GPL-V3
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
2. October, 2026.
3. October, 2026.
4. October, 2026.
5. October, 2026.
6. October, 2026.
*/

#include <stdio.h>
#include <cstring>
#include <thread>
#include <chrono>
#include "LIB_osnova_utils.h"

#include "computer_def.h"

/*
TODO objasni vamo kako se koristi ovo

PROSLJEDI
init
update update()
connection wire()
*/

int stage = 0; //CPU's exectuion cycle stage

int mems_size[mems_num];
byte_t *mems[mems_num];
CPU *cpus[cpus_num];

//SIMULATION THREAD

std::thread sim_thread; //Simulation thread

bool sim_running; //Thread control
int freq = 1; //Thread freqency (In Hz, max 1000Hz)

//Every clock tick
void update_thread(int freq)
{
    int period;

    if(freq > 1000) //Checks if freq is exceeding it's max value
        period = 1;
    else
        period = 1000 / freq;

    while(sim_running)
    {
        update();

        stage++;
        if(stage > 7) stage = 0; //Bounds it's value between 0 and 7

        std::this_thread::sleep_for(std::chrono::milliseconds(period)); //Simulation execution rate
    }
}

//SIMULATOR

int main()
{
    wire(); //External user-defined function that sets wiring of the computer

    //SIMULATOR COMMANDS

    char done[] = "done"; //Exits program
    char step[] = "step"; //Does one clock tick
    char cycle[] = "cycle"; //Does clock ticks until it finishes current cycle (Goes to the next multiple of 8)
    char dump[] = "dump"; //Prints (Dumps) memory contents
    char snap[] = "snap"; //Prints all CPU states
    char poke[] = "poke"; //Changes a value at a certain memory address
    char peek[] = "peek"; //Prints a value at a certain memory address
    char help[] = "help"; //Prints all commands and how to use them
    char dec[] = "dec"; //takes hex value, prints dec value
    char hex[] = "hex"; //takes dec value, prints hex value
    char list[] = "list"; //Lists opc,arg, alu
    char sim[] = "sim"; //Simulation start, stop, frequency
    char insert[] = "insert"; //Inserts string of values into memory at a certain memory address

    char cmd[16]; //Command (Simulator command)

    int arg1, arg2, arg3; //Command integer arguments
    char str1[16]; //Command string arguments

    //Short program's notice
    printf("Osnova c++ simulator version 1, type 'help' to list all commands and their arguments\n");
    printf("Created by Ivan Jonjic (IJPantic on github), GPL-V3\n \n");

    //SIMULATOR CONTROLS

    while(true)
    {
        //Read command
        printf(" > ");
        scanf("%15s", cmd);

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
                for(stage; stage < 8; stage++)
                {
                    update();
                }

                stage = 0;  
            }

        }else if(!strcmp(cmd, dump)) //dump memory contents
        {
            scanf("%d", &arg1); //Reads argument MEM

            byte_t *memory = mems[arg1]; //Selected memory to dump

            if(mems_size[arg1] > 65536) continue; //Stops command for executing if memory size is too big

            for(int y = 0; y < mems_size[arg1]; y += 32) //Print rows
            {
                printf("0xXX%04x: ", (pnt_t)y);

                //Print rows
                for(int x = 0; x < 32; x++)
                {
                    byte_t value = memory[y+x]; //Value at a certain address in memory

                    printf("0x%02x ", value);
                }

                printf("\n");
            }

        }else if(!strcmp(cmd, snap)) //Snapshot CPU
        {
            scanf("%d", &arg1); //Reads argument CPU
            CPU cpu = *cpus[arg1];

            printf("REG;   RA: 0x%02x    RB: 0x%02x   RC: 0x%02x    RESR: 0x%02x  DVR: 0x%02x \n", cpu.ra, cpu.rb, cpu.rc, cpu.resr, cpu.dvr);
            printf("INST;  Stage: %1d    IR: 0x%02x   Opc: %-5s  Arg: %-10s                   \n", stage, cpu.ir, dcmp_opc(cpu.opc), dcmp_arg(cpu.opc, cpu.arg));
            printf("PC;    SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.pc, 16, 20), trim(cpu.pc, 8, 16), trim(cpu.pc, 0, 8));
            printf("AP;    SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.ap, 16, 20), trim(cpu.ap, 8, 16), trim(cpu.ap, 0, 8));
            printf("PCI;   SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.pci, 16, 20), trim(cpu.pci, 8, 16), trim(cpu.pci, 0, 8));
            printf("API;   SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.api, 16, 20), trim(cpu.api, 8, 16), trim(cpu.api, 0, 8));
            printf("PS;    SFPS: 0x%02x  PS: 0x%02x   PRA: 0x%02x   PWA: 0x%02x               \n", cpu.sfps, cpu.ps, cpu.pra, cpu.pwa);
            printf("ALU;   ALU: 0x%02x   Op: %-24s                                            \n", cpu.alu, dcmp_op(cpu.op, cpu.mode, cpu.cin));
            printf("FLG;   EQU: %1d      FLGXN: %1d   INTMN: %1d    NEG: %1d      ODD: %1d    \n", cpu.equ, cpu.flgxn, cpu.intmn, cpu.neg, cpu.odd);

            printf("\n");

        }else if(!strcmp(cmd, poke)) //Poke memory address
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR
            scanf("%x", &arg3); //Reads argument VAL

            mems[arg1][arg2] = arg3; //Write to a memory address

        }else if(!strcmp(cmd, peek)) //Peek memory address
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR

            printf("0x%02x \n", mems[arg1][arg2]); //Print from a memory address

        }else if(!strcmp(cmd, help)) //Display help list
        {
            printf("done - - - - - - - -> Exits this program\n");
            printf("step - - - - - - - -> Ticks clock once\n");
            printf("cycle TIMES- - - - -> Finishes whole CPU execution cycle TIMES times\n");
            printf("dump MEM - - - - - -> Prints all contents of a MEM memory\n");
            printf("snap CPU - - - - - -> Prints all current states CPU cpu states\n");
            printf("poke MEM ADR VAL - -> Writes hexadecimal VAL value to a memory MEM at address ADR\n");
            printf("peek MEM ADR - - - -> Prints a memory value from memory MEM at address ADR\n");
            printf("help - - - - - - - -> Prints help list of all commnds (This is help command)\n");
            printf("dec VAL- - - - - - -> Prints value VAL in a decimal base\n");
            printf("hex VAL- - - - - - -> Prints value VAL in a hexadecimal base\n");
            printf("list TABLE - - - - -> Lists TABLE table, where TABLE can be opc (Opcodes), argsrc (Data sources), argcnd (Jump conditions), aluop (ALU operations)\n");
            printf("sim ACT F- - - - - -> ACT 'start' starts simulation, 'stop' stops simulation and 'freq' has additional argument 'F' which sets simulation's frequency\n");
            printf("insert MEM ADR FILE-> Writes a string of values to a memory MEM at address ADR from '.bin' file FILE\n");

            printf("\n");

        }else if(!strcmp(cmd, dec)) //Transfer to decimal
        {
            scanf("%x", &arg1); //Reads argument VAL

            printf("%d \n", arg1);

        }else if(!strcmp(cmd, hex)) //Transfer to hexadecimal
        {
            scanf("%d", &arg1); //Reads argument VAL

            printf("0x%02x \n", arg1);

        }else if(!strcmp(cmd, list)) //
        {
            scanf("%15s", str1); //Reads argument TABLE

            if(!strcmp(str1, "opc")) //List opcodes
            {
                for(byte_t i = 0; i < 16; i++)
                {
                    printf("0x%1xX: %s \n", (byte_t)i, dcmp_opc(i));
                }

                printf("\n");

            }else if(!strcmp(str1, "argsrc")) //List argument data sources
            {
                for(byte_t i = 0; i < 16; i++)
                    printf("0xX%1x: %s \n", i, dcmp_arg(0, i));
                printf("\n");

            }else if(!strcmp(str1, "argcnd")) //List argument jump (branch) conditions
            {
                for(byte_t i = 0; i < 16; i++)
                    printf("0xX%1x: %s \n", i, dcmp_arg(0, i));

                printf("\n");

            }else if(!strcmp(str1, "aluop")) //List ALU operations
            {
                printf("Sel; Mode:0, Cin:0           Mode:0, Cin:1,          Mode:1, Cin:X\n");

                for(byte_t i = 0; i < 16; i++)

                {
                    printf("0x%1x: ", i);

                    printf("%s", dcmp_op(i, 0, 0));
                    printf("%s", dcmp_op(i, 0, 1));
                    printf("%s", dcmp_op(i, 1, 0));

                    printf("\n");
                }

                printf("\n");
            }

        }else if(!strcmp(cmd, sim)) //Simulation control
        {
            scanf("%15s", str1); //Reads argument ACT

            if(!strcmp(str1, "start")) //Starts simulation thread
            {
                sim_running = 1;
                sim_thread = std::thread(update_thread, freq);
                sim_thread.detach();

            }else if(!strcmp(str1, "stop")) //Ends simulation thread
            {
                sim_running = 0;

            }else if(!strcmp(str1, "freq")) //Sets simulation execution frequency
            {
                scanf("%d", &arg1); //Reads argument F

                freq = arg1;
            }

        }else if(!strcmp(cmd, insert)) //Inserts string of values into memory
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR
            scanf("%15s", &str1); //Reads argument FILE
            scanf("%d", &arg3); //Reads argument SIZE

            //Retriving values from a bin file
            FILE *p_file = fopen(str1, "rb");

            if(p_file == NULL)
                printf("Can't open this file");

            byte_t *tmp_mem = (byte_t*)malloc(arg3); //Creating temporary memory

            fread(tmp_mem, 1, arg3, p_file);

            //Write to a memory address
            for(int times = 0; times < arg3; times++)
                mems[arg1][times +arg2] = tmp_mem[times];

            free(tmp_mem);
            fclose(p_file);

        }else printf("\nThis command does not exist\n \n");
    }
}

