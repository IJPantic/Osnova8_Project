//Osnova c++ utilities library file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
25. september, 2026.
*/

#include "LIB_osnova_utils.h" //Header file of this library

//Universal variables that will be often used
byte FF = 255; //"FF" as 0xFF, max value of a byte
pnt adrAREA = 0X0FFFF; //Address area mask of ADD address space
pnt sctAREA = 0xF0000; //Sector area mask of ADD address

//Functions

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

