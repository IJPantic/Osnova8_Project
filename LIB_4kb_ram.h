//Osnova c++ 4KB RAM header file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
*/

#include "LIB_osnova_utils.h"

#ifndef LIB_4kb_ram
#define LIB_4kb_ram

//Defining RAM
class RAM
{
    byte memory[4096]; //RAM's storage

    public:
        RAM(); //Constructor

        //INPUTS AND OUTPUTS

        //Buses
        pnt *adrBus; //Memory storage address bus
        byte *dataBus; //Memory IO data bus

        //Control lines
        bool *WE; //Write enable (Inverted)
        bool *RE; //Read enable (Inverted)
        bool *CE; //Chip enable (Inverted)

        //RAM STRUCTURE

        void update(); //RAM's value update
};

#endif

