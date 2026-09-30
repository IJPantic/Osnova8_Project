//Osnova c++ memory library file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
30. september, 2026.
*/

#include "LIB_osnova_mem.h" //Header file of this library

//Memory constructor
MEM::MEM(int size, const char type[3])
{
    //SETTING PROPERTIES

    mem_size = size; //Memory size (In bytes)

    //Memory type ('rw' read/write, '-w' write only, 'r-' read only)
    mem_type[0] = type[0]; //Read?
    mem_type[1] = type[1]; //Write?
    mem_type[2] = '\0';

    memory = (byte*)malloc(mem_size); //Creating memory

    //INPUTS AND OUTPUTS

    //Buses
    *adr_bus; //Memory storage address bus
    *data_bus; //Memory IO data bus

    //Control lines
    *we; //Write enable (Inverted)
    *re; //Read enable (Inverted)
    *ce; //Chip enable (Inverted)

    //MEMORY ACTIONS

    void update(); //Memory value update

    void end(); //Free allocated memory from this memory emulation
}

//Memory value update
void MEM::update()
{
    if(!*ce) //Is chip selected?
    {
        pnt valid_adr = trim(*adr_bus, 0, log2(mem_size)); //Containing 20-bit address within actual memory address space

        if(mem_type[0] == 'r' && !*re && *we) //Reading from memory to data bus
            *data_bus = memory[valid_adr];

        else if(mem_type[1] == 'w' && *re && !*we) //writing from data bus to memory
            memory[valid_adr] = *data_bus;
    }
}

//Free allocated memory from this memory emulation
void MEM::end()
{
    free(memory);
}

