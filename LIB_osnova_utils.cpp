//Osnova c++ utilities library file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
25. september, 2026.
1. October, 2026.
2. October, 2026.
*/

#include "LIB_osnova_utils.h" //Header file of this library

//UNIVERSAL CONSTANTS

const byte_t FF = 255; //"FF" as 0xFF, max value of a byte

const byte_t LOW_AREA = 0X0F; //Low area mask of a byte
const byte_t HIGH_AREA = 0xF0; //High area mask of a byte

const pnt_t ADR_AREA = 0X0FFFF; //Address area mask of ADD address space
const pnt_t SCT_AREA = 0xF0000; //Sector area mask of ADD address

//FUNCTIONS

//Trims off all bits in a byte except selected part
byte_t trim(byte_t value, int start, int end)
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

//Trims off all bits in a pointer except selected part
pnt_t trim(pnt_t value, int start, int end)
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

//Adds padding to a byte value
bstr_t padd(byte_t value)
{
    //Log base 2 divided by 4 is equivalent to log base 16 (We add +1 to the value because number of combinations is max number +1)
    int dig_num = (log2(value +1) /4); //Number of hex digits needed to represent the number
    if(!dig_num) dig_num = 1; //Zero still needs one hex digit to be represented

    bstr_t padded_val = {"0x12"}; //Return string array

    //String representation of a value
    char str_val[3];
    sprintf(str_val, "%x", value);

    int msd = 1+2; //Most significant digit index in a max value byte
    int j = 0;

    for(int i = msd; i > -1; i--)
    {
        if(msd -dig_num < i) //Add value's digit
            padded_val.digits[i] = str_val[j];

        else if(1 < i) //Add padding except for 0x part
            padded_val.digits[i] = '0';

        j++; //Step of j is in contra-directions of i
    }
    
    return padded_val;
}

//Adds padding to a pointer value
pstr_t padd(pnt_t value)
{
    //Log base 2 divided by 4 is equivalent to log base 16 (We add +1 to the value because number of combinations is max number +1)
    int dig_num = (log2(value +1) /4); //Number of hex digits needed to represent the number
    if(!dig_num) dig_num = 1; //Zero still needs one hex digit to be represented

    pstr_t padded_val = {"0x123456"}; //Return string array

    //String representation of a value
    char str_val[6];
    sprintf(str_val, "%x", value);

    int msd = 5+2; //Most significant digit index in a max value pointer
    int j = 0;

    for(int i = msd; i > -1; i--)
    {
        if(msd -dig_num < i) //Add value's digit
            padded_val.digits[i] = str_val[j];

        else if(1 < i)//Add padding except for 0x part
            padded_val.digits[i] = '0';

        j++; //Step of j is in contra-directions of i
    }
    
    return padded_val;
}

