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
2. October, 2026.
3. October, 2026.
*/

#include <stdio.h>
#include <cstring>
#include <pthread.h>
#include <time.h>
#include "LIB_osnova_utils.h"

#include "LIB_osnova_cpu.h"
#include "LIB_osnova_mem.h"

/*
TODO napravi daa je ovo glavni program ali sadrzi #include main (lib) koji di ti unda pises kakav ce bit harwaare
*/

//Argument decompile table (Turns argument number into argument's name)
const char *decompile_arg(byte_t opc, byte_t arg)
{
    if(opc) //Opcode is not "jmp" instruction
    {
        switch(arg) //Argument is for data sources
        {
            case 0: return "alu  "; break;
            case 1: return "rc   "; break;
            case 2: return "sf   "; break;
            case 3: return "pfl  "; break;
            case 4: return "pfh  "; break;
            case 5: return "resr "; break;
            case 6: return "add  "; break;
            case 7: return "dvr  "; break;
            case 8: return "x    "; break;
            case 9: return "x    "; break;
            case 10: return "x    "; break;
            case 11: return "x    "; break;
            case 12: return "x    "; break;
            case 13: return "x    "; break;
            case 14: return "x    "; break;
            case 15: return "x    "; break;
        }

    }else //Opcode is "jmp" instruction
    {
        switch(arg) //Argument is for jump (branch) conditions
        {
            case 0: return "nc    "; break;
            case 1: return "equ   "; break;
            case 2: return "flgxn "; break;
            case 3: return "intmn "; break;
            case 4: return "carry "; break;
            case 5: return "neg   "; break;
            case 6: return "odd   "; break;
            case 7: return "dont  "; break;
            case 8: return "nc    "; break;
            case 9: return "equ   "; break;
            case 10: return "flgxn "; break;
            case 11: return "intmn "; break;
            case 12: return "carry "; break;
            case 13: return "neg   "; break;
            case 14: return "odd   "; break;
            case 15: return "dont  "; break;
        }
    }

    return "Err";
}

//Opcode decompile table (Turns opcode number into opcode's name)
const char *decompile_opc(byte_t opc)
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
const char *decompile_alu_op(int op, bool mode, int cin)
{
    if(mode) //Logic operations
    {
        switch(op)
        {
            case 0: return "!A                      "; break;
            case 1: return "!(A || B)               "; break;
            case 2: return "(!A) && B               "; break;
            case 3: return "0                       "; break;
            case 4: return "!(A && B)               "; break;
            case 5: return "!B                      "; break;
            case 6: return "A ^ B                   "; break;
            case 7: return "A && (!B)               "; break;
            case 8: return "(!A) || B               "; break;
            case 9: return "!(A ^ B)                "; break;
            case 10: return "B                       "; break;
            case 11: return "A && B                  "; break;
            case 12: return "1                       "; break;
            case 13: return "A || (!B)               "; break;
            case 14: return "A || B                  "; break;
            case 15: return "A                       "; break;
        }

    }else //Arithmetic operations
    {
        if(cin) //Without carry in
        {
            switch(op)
            {
                case 0: return "A                       "; break;
                case 1: return "(A || B)                "; break;
                case 2: return "(A || (!B))             "; break;
                case 3: return "255                     "; break;
                case 4: return "A+(A && (!B))           "; break;
                case 5: return "(A || B)+(A && (!B))    "; break;
                case 6: return "A-B-1                   "; break;
                case 7: return "(A && (!B))-1           "; break;
                case 8: return "A+(A && B)              "; break;
                case 9: return "A+B                     "; break;
                case 10: return "(A || (!B))+(A && B)    "; break;
                case 11: return "(A && B)-1              "; break;
                case 12: return "A+A                     "; break;
                case 13: return "(A || B)+A              "; break;
                case 14: return "(A || (!B))+A           "; break;
                case 15: return "A-1                     "; break;
            }

        }else //With carry in
        {
            switch(op)
            {
                case 0: return "A +1                    "; break;
                case 1: return "(A || B) +1             "; break;
                case 2: return "(A || (!B)) +1          "; break;
                case 3: return "0                       "; break;
                case 4: return "A+(A && (!B)) +1        "; break;
                case 5: return "(A || B)+(A && (!B)) +1 "; break;
                case 6: return "A-B                     "; break;
                case 7: return "(A && (!B))             "; break;
                case 8: return "A+(A && B) +1           "; break;
                case 9: return "A+B +1                  "; break;
                case 10: return "(A || (!B))+(A && B) +1 "; break;
                case 11: return "(A && B)                "; break;
                case 12: return "A+A +1                  "; break;
                case 13: return "(A || B)+A +1           "; break;
                case 14: return "(A || (!B))+A +1        "; break;
                case 15: return "A                       "; break;
            }
        }
    }

    return "Err";
}

bool sim_running;
pthread_t thread;
int stage = 0; //Cycle's stage
int freq = 1;

//INITIALIZE HARDWARE
CPU main_cpu;
MEM main_ram(65536, "rw");

void update()
{
    main_cpu.update();
    main_ram.update();
}

//Every clock tick
void *update_thread(void *freq)
{
    int freq_num = *(int*)freq;

    while(sim_running)
    {
        for(int i = 0; i < 1000/freq_num; i++)
            update();

        stage++;
        if(stage > 7) stage = 0; //Bounds it's value between 0 and 7

        /* save start time */
        const time_t start = time(NULL);

        time_t current;
        do{
            /* get current time */
            time(&current);

            /* break loop when the requested number of seconds have elapsed */
        }while(difftime(current, start) < 1);
    }

    return NULL;
}

//CONNECT HARDWARE
int main()
{
    int mems_num = 1;
    byte_t *mems[mems_num]; //Napraavi da je zasebna funckija koju user moze pokrenuti TODO
    int mems_size[mems_num];

    int cpus_num = 1;
    CPU *cpus[cpus_num]; //Napravi da je zasebna funckija koju user moze pokrenuti TODO

    //COMPUTER CONNECTION
    byte_t data_bus; //Data bus
    pnt_t adr_bus; //Address bus

    bool we = 1; //CPU's write enable signal
    bool re = 1; //CPU's read enable signal

    bool int_add; //Interrupt ADD line
    bool int_cpu = 1; //Interrupt CPU line
    bool flgx = 1; //CPU's flag x pin
    bool ce = 0; //RAM's chip enable signal

    //MAIN CPU CONNECTION
    cpus[0] = &main_cpu;

    main_cpu.io_db = &data_bus; //Data bus
    main_cpu.io_adrb = &adr_bus; //Address bus

    main_cpu.io_if = &int_cpu; //Interrupt CPU line
    main_cpu.io_itx = &int_add; //Interrupt ADD line
    main_cpu.io_fx = &flgx; //CPU's flag x pin

    main_cpu.io_addw = &we; //Write enable signal
    main_cpu.io_addr = &re; //Read enable signal

    //MAIN RAM CONNECTION
    mems[0] = main_ram.memory;
    mems_size[0] = 65536;

    main_ram.io_db = &data_bus; //Data bus
    main_ram.io_adrb = &adr_bus; //Address bus

    main_ram.io_we = &we; //Write enable signal
    main_ram.io_re = &re; //Read enable signal
    main_ram.io_ce = &ce; //Chip enable signal

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
    char dec[] = "dec"; //takes hex value, prints dec value
    char hex[] = "hex"; //takes dec value, prints hex value
    char list[] = "list"; //Lists opc,arg, alu
    char sim[] = "sim"; //Simulation start, stop, frequency

    char cmd[16]; //Command (Emulator command)

    int arg1, arg2, arg3; //Command integer arguments
    char str1[16]; //Command string arguments

    //Short program notice
    printf("Osnova c++ emulator, type 'help' to list all commands and their arguments\n");
    printf("Created by Ivan Jonjic (IJPantic on github), GPL-V3\n \n");

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

            byte_t *memory = mems[arg1]; //Selected memory to dump

            if(mems_size[arg1] > 65536) continue; //Stops command for executing if memory size is too big

            for(int y = 0; y < mems_size[arg1]; y += 32) //Print rows
            {
                printf("0xXX%c%c%c%c: ", padd(y).digits[4], padd(y).digits[5], padd(y).digits[6], padd(y).digits[7]);

                //Print rows
                for(int x = 0; x < 32; x++)
                {
                    byte_t value = memory[y+x]; //Value at a certain address in memory

                    printf("%s ", padd(value).digits);
                }

                printf("\n");
            }

        }else if(!strcmp(cmd, snap)) //Snapshot CPU
        {
            scanf("%d", &arg1); //Reads argument CPU
            CPU cpu = *cpus[arg1];

            printf("REG;    RA:%s       RB:%s       RC:%s      RESR:%s    DVR:%s \n", padd(cpu.ra).digits, padd(cpu.rb).digits, padd(cpu.rc).digits, padd(cpu.resr).digits, padd(cpu.dvr).digits);
            printf("INST;   Stage:%d       IR:%s       Arg:%s   Opc:%s \n", stage, padd(cpu.ir).digits, decompile_arg(cpu.opc, cpu.arg), decompile_opc(cpu.opc));
            printf("PC;     PFL:%s      PFH:%s      SF:%s \n", padd((byte_t)trim(cpu.pc, 0, 8)).digits, padd((byte_t)trim(cpu.pc, 8, 16)).digits, padd((byte_t)trim(cpu.pc, 16, 20)).digits);
            printf("AP;     PFL:%s      PFH:%s      SF:%s \n", padd((byte_t)trim(cpu.ap, 0, 8)).digits, padd((byte_t)trim(cpu.ap, 8, 16)).digits, padd((byte_t)trim(cpu.ap, 16, 20)).digits);
            printf("PCI;    PFL:%s      PFH:%s      SF:%s \n", padd((byte_t)trim(cpu.pci, 0, 8)).digits, padd((byte_t)trim(cpu.pci, 8, 16)).digits, padd((byte_t)trim(cpu.pci, 16, 20)).digits);
            printf("API;    PFL:%s      PFH:%s      SF:%s \n", padd((byte_t)trim(cpu.api, 0, 8)).digits, padd((byte_t)trim(cpu.api, 8, 16)).digits, padd((byte_t)trim(cpu.api, 16, 20)).digits);
            printf("PS;     SFPS:%s     PS:%s       PRA:%s      PWA:%s \n", padd(cpu.sfps).digits, padd(cpu.ps).digits, padd(cpu.pra).digits, padd(cpu.pwa).digits);
            printf("ALU;    ALU:%s      Op:%s \n", padd(cpu.alu).digits, decompile_alu_op(cpu.op, cpu.mode, cpu.cin));
            printf("FLG;    EQU:%d         FLGXN:%d       INTMN:%d       NEG:%d        ODD:%d \n", cpu.equ, cpu.flgxn, cpu.intmn, cpu.neg, cpu.odd);
            printf("\n");

        }else if(!strcmp(cmd, poke)) //Poke memory address
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR
            scanf("%x", &arg3); //Reads argument VAL

            mems[arg1][arg2] = arg3;

        }else if(!strcmp(cmd, peek)) //Peek memory address
        {
            scanf("%d", &arg1); //Reads argument MEM
            scanf("%d", &arg2); //Reads argument ADR

            printf("%s \n", padd(mems[arg1][arg2]).digits);

        }else if(!strcmp(cmd, help)) //Display help list
        {
            printf("done -> Exits this program\n");
            printf("step -> Ticks clock once\n");
            printf("cycle TIMES -> Finishes whole CPU execution cycle TIMES times\n");
            printf("dump MEM -> Prints all contents of a MEM memory\n");
            printf("snap CPU -> Prints all current states CPU cpu states\n");
            printf("poke MEM ADR VAL-> Writes hexadecimal VAL value to a memory MEM at address ADR\n");
            printf("peek MEM ADR -> Prints a memory value from memory MEM at address ADR\n");
            printf("help -> Prints help list of all commnds (This is help command)\n");
            printf("dec VAL -> Prints value VAL in a decimal base\n");
            printf("hex VAL -> Prints value VAL in a hexadecimal base\n");
            printf("list TABLE -> Lists TABLE table, where TABLE can be opc (Opcodes), argsrc (Data sources), argcnd (Jump conditions), aluop (ALU operations)\n");
            printf("sim ACT F -> ACT 'start' starts simulation, 'stop' stops simulation and 'freq' has additional argument 'F' which sets simulation's frequency\n");

            printf("\n");

        }else if(!strcmp(cmd, dec)) //Peek memory address
        {
            scanf("%x", &arg1); //Reads argument VAL

            printf("%d \n", arg1);

        }else if(!strcmp(cmd, hex)) //Peek memory address
        {
            scanf("%d", &arg1); //Reads argument VAL

            printf("%x \n", arg1);

        }else if(!strcmp(cmd, list)) //
        {
            scanf("%15s", str1); //Reads argument TABLE

            if(!strcmp(str1, "opc"))
            {
                for(byte_t i = 0; i < 16; i++)
                    printf("0x%cX %s \n", padd(i).digits[3], decompile_opc(i));

                printf("\n");

            }else if(!strcmp(str1, "argsrc"))
            {
                for(byte_t i = 0; i < 16; i++)
                    printf("0xX%c %s \n", padd(i).digits[3], decompile_arg(1, i));

                printf("\n");

            }else if(!strcmp(str1, "argcnd"))
            {
                for(byte_t i = 0; i < 8; i++)
                    printf("0xX%c %s \n", padd(i).digits[3], decompile_arg(0, i));

                printf("\n");

            }else if(!strcmp(str1, "aluop"))
            {
                printf("Sel; Mode:0, Cin:0           Mode:0, Cin:1,          Mode:1, Cin:X\n");

                for(byte_t i = 0; i < 16; i++)
                {
                    printf("0x%c: ", padd(i).digits[3]);

                    printf("%s", decompile_alu_op(i, 0, 0));
                    printf("%s", decompile_alu_op(i, 0, 1));
                    printf("%s", decompile_alu_op(i, 1, 0));

                    printf("\n");
                }

                printf("\n");
            }

        }else if(!strcmp(cmd, sim)) //
        {
            scanf("%15s", str1); //Reads argument ACT

            if(!strcmp(str1, "start"))
            {
                sim_running = 1;
                pthread_create(&thread, NULL, update_thread, &freq);

            }else if(!strcmp(str1, "stop"))
            {
                sim_running = 0;
                pthread_join(thread, NULL);

            }else if(!strcmp(str1, "freq")) //TODO sredi da freq je zapravo frekvencija a ne samo neki broj
            {
                scanf("%d", &arg1); //Reads argument F

                freq = arg1;
            }

        }else printf("This command does not exist\n \n");
    }
}

