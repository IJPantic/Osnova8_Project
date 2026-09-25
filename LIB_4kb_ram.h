//Osnova c++ 4KB RAM library header file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
*/

#include "LIB_osnova_utils.h"

#ifndef LIB_4kb_ram
#define LIB_4kb_ram

//Defining RAM
class RAM
{
    public:
        RAM(); //Constructor

        byte memory[4096] = {0}; //RAM's storage

        //INPUTS AND OUTPUTS

        //Buses
        pnt *adrBus; //Memory storage address bus
        byte *dataBus; //Memory IO data bus

        pnt validAdr; //Actually usable address space of RAM

        //Control lines
        bool *WE; //Write enable (Inverted)
        bool *RE; //Read enable (Inverted)
        bool *CE; //Chip enable (Inverted)

        //RAM STRUCTURE

        void update(); //RAM's value update
};

#endif

