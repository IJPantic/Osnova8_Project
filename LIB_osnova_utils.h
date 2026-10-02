//Osnova c++ utilities library header file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
25. september, 2026.
25. september, 2026.
1. October, 2026.
2. October, 2026.
*/

#include <cstdint>
#include <cstdio>
#include <math.h>

#ifndef LIB_osnova_utils
#define LIB_osnova_utils

//DATA TYPES

using byte_t = uint8_t; //CPU's word, 8-bit
using pnt_t = int; //Pointer, 20-bit

struct bstr_t{char digits[5];};
struct pstr_t{char digits[9];};

//UNIVERSAL CONSTANTS

extern const byte_t FF; //"FF" as 0xFF, max value of a byte
extern const pnt_t ADR_AREA; //Address area mask of ADD address space
extern const pnt_t SCT_AREA; //Sector area mask of ADD address

//FUNCTIONS

//Trims off all bits in a byte except selected part
byte_t trim(byte_t value, int start, int end);

//Trims off all bits in a pointer except selected part
pnt_t trim(pnt_t value, int start, int end);

//Adds padding to a byte value
bstr_t padd(byte_t value);

//Adds padding to a pointer value
pstr_t padd(pnt_t value);

#endif

