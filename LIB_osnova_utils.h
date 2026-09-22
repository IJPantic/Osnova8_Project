//Osnova c++ utilities heaader file
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
*/

#ifndef LIB_osnova_utils.h
#define LIB_osnova_utils.h

//Data types
using byte = uint8_t; //CPU's word, 8-bit
using pnt = int; //Pointer, 20-bit

//Universal variables that will be often used
extern byte FF; //"FF" as 0xFF, max value of a byte
extern pnt adrAREA; //Address area mask of ADD address space (0x0FFFF)
extern pnt sctAREA; //Sector area mask of ADD address space (0XF0000)

//Functions
extern int trim(int value, int start, int end);

#endif

