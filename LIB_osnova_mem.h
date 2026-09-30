//Osnova c++ memory library header file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
30. september, 2026.
*/

#include "LIB_osnova_utils.h"

#ifndef LIB_osnova_mem
#define LIB_osnova_mem

//Defining RAM
class MEM
{
    public:
        MEM(int size, const char type[3]); //Memory constructor

        //SETTING PROPERTIES

        int mem_size; //Memory size (In bytes)

        char mem_type[3]; //Memory type ('rw' read/write, '-w' write only, 'r-' read only)

        byte *memory; //Memory storage

        //INPUTS AND OUTPUTS

        //Buses
        pnt *adr_bus; //Memory storage address bus
        byte *data_bus; //Memory IO data bus

        //Control lines
        bool *we; //Write enable (Inverted)
        bool *re; //Read enable (Inverted)
        bool *ce; //Chip enable (Inverted)

        //MEMORY AACTIONS

        void update(); //Memory value update

        void end(); //Free allocated memory from this memory emulation
};

#endif

