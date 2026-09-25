//Osnova c++ 4KB RAM library file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
*/

#include "LIB_4kb_ram.h" //Header file of this library

//Defining RAM
RAM::RAM()
{
    memory; //RAM's storage

    //INPUTS AND OUTPUTS

    //Buses
    *adrBus; //Memory storage address bus
    *dataBus; //Memory IO data bus

    validAdr; //Actually usable address space of RAM

    //Control lines
    *WE; //Write enable (Inverted)
    *RE; //Read enable (Inverted)
    *CE; //Chip enable (Inverted)

    //RAM STRUCTURE

    void update(); //RAM's value update
}

//RAM's value update
void RAM::update()
{
    if(!*CE) //Is chip selected?
    {
        validAdr = trim(*adrBus, 0, 12); //Containing within 12-bit (4096 addresses) address space

        if(*RE && !*WE) //writing from data bus to RAM
            memory[validAdr] = *dataBus;

        else if(!*RE && *WE) //Reading from RAM to data bus
            *dataBus = memory[validAdr];
    }
}

