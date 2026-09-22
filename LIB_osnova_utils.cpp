//Osnova c++ utilities library
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
*/

#include "osnova_utils.h" //Header file of this library

//Data types
using byte = uint8_t; //CPU's word, 8-bit
using pnt = int; //Pointer, 20-bit

//Universal variables that will be often used
byte FF = 255; //"FF" as 0xFF, max value of a byte
pnt adrAREA = 65535; //Address area mask of ADD address space (0x0FFFF)
pnt sctAREA = 1048575-65535; //Sector area mask of ADD address space (0XF0000)

//Functions
int trim(int value, int start, int end);

//trims off all bits except selected part
int trim(int value, int start, int end)
{
    value = value >>start;

    int mask = 0;
    for(int i=0; i<end-start; i++)
    {
        mask += pow(2, i);
    }

    value = value & mask;

    return value;
}

