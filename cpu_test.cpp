//Osnova c++ CPU testing
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
*/

#include "LIB_osnova_utils.h"
#include "LIB_osnova_cpu.h"
#include "LIB_4kb_ram.h"

//GLOBALS

//Computer's connection lines and buses
byte DataBus; //Computer data bus
pnt AdrBus; //Computer ddress bus
bool WE; //CPU write enable signal
bool RE; //CPU read enable signal
bool CIntD; //CPU interrupts ADD
bool DIntC; //ADD interrupts CPU

//Computer's hardware
CPU MainCPU;
RAM MainRAM;

int main()
{
    //CONNECTING CPU's IO

    //Buses
    MainCPU.Bus = &DataBus;
    MainCPU.ADDB = &AdrBus;

    //Interrupt lines
    MainCPU.IF = &DIntC;
    MainCPU.ITX = &CIntD;

    //Control signals
    MainCPU.Addw = &WE;
    MainCPU.Addr = &RE;

    //CONNECTING RAM's IO

    //Buses
    MainRAM.dataBus = &DataBus;
    MainRAM.adrBus = &AdrBus;

    //Control signals
    MainRAM.WE = &WE;
    MainRAM.RE = &RE;

    return 0;
}

