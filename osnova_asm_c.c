//Osnova assembly C compiler, version beta, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
9. October, 2026.
11. October, 2026.
12. October, 2026.
13. October, 2026.
*/

/*
TODO
nemnoj napraviti bin buffer nego za to odmah pisi u file sa append
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//File extension check, checks if 'fpath' has extension 'ext' ('Ext' max lenght is 5 characters including '\0')
int ext_check(char *fpath, const char ext[3]);

//Delimiter characters
const char DELIMITER[23];

int main()
{
    //FILE MANIPULATION

    //Reading user input
    char asm_fpath[256]; //Assembly file path
    char bin_fpath[256]; //Binary file path

    //Short program's notice
    printf("Osnova assembly C compiler version beta\n");
    printf("Created by Ivan Jonjic (IJPantic on github), GPL-V3\n \n");

    printf("Assembly ('.asm') input file (path): ");
    scanf("%255s", asm_fpath); //Getting assembly file path

    printf("Binary ('.bin') output file (path): ");
    scanf("%255s", bin_fpath); //Getting binary file path

    //Checking filepaths
    if(ext_check(asm_fpath, ".asm")) //Input filepath is not assembly check
    {
        printf("ERROR: Input file does not have '.asm' extension!\n");
        return 1;
    }

    if(ext_check(bin_fpath, ".bin")) //Output filepath is not binary check
    {
        printf("ERROR: Input file does not have '.bin' extension!\n");
        return 1;
    }

    //Opening files
    FILE *p_fasm = fopen(asm_fpath, "r"); //Assembly input file
    if(p_fasm == NULL) //Failed to read input file check
    {
        printf("ERROR: This assembly file does not exist!\n");
        return 1;
    }

    FILE *p_fbin = fopen(bin_fpath, "wb"); //Binary output file
    fwrite("", 0, 0, p_fbin); //Clear output file before writing
    p_fbin = fopen(bin_fpath, "ab");

    //Reading input file to a buffer
    int asm_fsize; //Assembly file size
    char *asm_fbuf; //Assembly file buffer

    fseek(p_fasm, 0, SEEK_END); //Setting file's pointer to the end of a file
    asm_fsize = ftell(p_fasm) +1; //Reading file size (+2 because index starts at 0)
    fseek(p_fasm, 0, SEEK_SET); //Setting file's pointer to the beggining of a file

    asm_fbuf = (char*)malloc(asm_fsize); //Creating assembly file buffer

    fread(asm_fbuf, 1, asm_fsize, p_fasm); //Writing from a file to the buffer
    fclose(p_fasm);

    //LEXER TODO 2 skena, u prvom pobroji delimitere i alociraj za p array, 2. sken sa poinettrima odvoji rici i alociraj u struct

    //first run
    int token_num = 0;

    for(int i = 0; i < asm_fsize; i++)
        if(asm_fbuf[i]) token_num++;

    printf("toke_num: %d\n", token_num);

    //second run

    int first_p = 0;
    int last_p = 0;

    char current_char;

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        for(int j = 0; j < sizeof(DELIMITER); j++)
        {
            if(current_char == DELIMITER[j])
            {
                for(int i = first_p; i < last_p; i++)
                {
                    printf("%c", asm_fbuf[i]);
                    if(i == last_p-1 && DELIMITER[j] != '\n')
                        printf(".\n");
                }

                printf("%c [DELIMITER]\n", DELIMITER[j]);

                first_p = last_p +1;
            }
        }
    }

    return 0;
}

//FUNCTIONS

//File extension check, returns 0 if 'fpath' has extension 'ext' (Max 'ext' lenght is 5 characters including '\0')
int ext_check(char *fpath, const char ext[5])
{
    int dot_i; //Index where the last dot in a filepath should be
    char curr_ext[5]; //File's current exstension

    dot_i = strlen(fpath) -3 -1; //-1 is there because 'strlen' returns size, but index starts at 0

    for(int i = 0; i < 4; i++) //Reads current file extesion
        curr_ext[i] = fpath[dot_i +i];
    curr_ext[4] = '\0'; //Adding string terminator

    return strcmp(curr_ext, ext); //Assembly extension check
}

//GLOBAL CONSTANTS

//Delimiter characters
const char DELIMITER[23] = {
    '%', //Compiler directive
    '#', //Binary word join operator
    '\"', //String bracket
    '\'', //Character bracket
    ';', //Comment
    ':', //Function definition
    ',', //Argument/parameter seperator
    '.', //Line label
    ' ', //word separator (space)
    '\t', //word separator (tab)
    '\n', //Instruction end
    '+', //Addition operator
    '-', //Subtraction operator
    '*', //multiplication operator
    '/', //Integer division operator
    '&', //Logic and operator
    '|', //Logic or operator
    '!', //Logic invertor operator
    '(', //Open bracket for operation priority
    ')', //Closed bracket for operation priority
    '[', //Open bracket for part selection
    ']', //Closed bracket for part selection
    '\0' //End of assembly code
};

//lexer
//parser
//optimiser
//generator

