//Osnova assembly C compiler, version beta, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
9. October, 2026.
11. October, 2026.
12. October, 2026.
13. October, 2026.
14. October, 2026.
17. October, 2026.
*/

/*
TODO
nemnoj napraviti bin buffer nego za to odmah pisi u file sa append
token build nek bude funkcija
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//File extension check, checks if 'fpath' has extension 'ext' ('Ext' max lenght is 5 characters including '\0')
int ext_check(char *fpath, const char ext[3]);

//Delimiter characters check
int delimiter_check(const char check_i);

int main()
{
    //USER INPUT

    //Reading user input
    char asm_fpath[256]; //Assembly file path
    char bin_fpath[256]; //Binary file path

    //Short program's notice
    printf("Osnova assembly C compiler version beta\n");
    printf("Created by Ivan Jonjic (IJPantic on github), GPL-V3\n \n");

    //Getting assembly file path
    printf("Assembly ('.asm') input file (path): ");
    scanf("%255s", asm_fpath);

    if(ext_check(asm_fpath, ".asm")) //Wrong input file extension check
    {
        printf("\nERROR: Input file does not have '.asm' extension!\n");
        return 1;
    }

    //Getting binary file path
    printf("Binary ('.bin') output file (path): ");
    scanf("%255s", bin_fpath);

    if(ext_check(bin_fpath, ".bin")) //Wrong output extension check
    {
        printf("\nERROR: Output file does not have '.bin' extension!\n");
        return 1;
    }

    //FILE MANIPULATION

    //Opening files
    FILE *p_fasm = fopen(asm_fpath, "r"); //Assembly input file
    if(p_fasm == NULL) //Failed to read input file check
    {
        printf("\nERROR: This assembly file does not exist!\n");
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

    //LEXER

    int token_num = 0;

    int first_p;
    int last_p;

    char current_char;
    char token_build[32];

    int i;

    //first run

    first_p = 0;
    last_p = 0;

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        i = first_p;

        if(last_p-first_p > 32) //Max token size check
        {
            printf("\nERROR: Words excede max token limit (31)!\n");
            return 1;
        }

        if(delimiter_check(current_char)) //Delimiter is a sign that word is done
        {
            if(current_char == '\n') //Include delimiter also
            {
                if(first_p != last_p) token_num +=2;

            }else
            {
                if(current_char == ' ' || current_char == '\t')
                {
                    if(first_p != last_p) token_num +=1;

                }else
                {
                    if(first_p == last_p)
                        token_num +=1;
                    else
                        token_num +=2;
                }
            }

            first_p = last_p +1;
        }
    }

    char (*token_p_array)[32] = malloc(token_num*32); //Token pointer array (points to all tokens in the aassembly code, max token size is 31 characters)

    printf("\ntoken_num: %d\n\n", token_num);

    //second run

    first_p = 0;
    last_p = 0;

    int token_i = 0;

    void add_built_token()
    {
        for(i; i < last_p; i++) token_build[i -first_p] = asm_fbuf[i]; //slozi ric
        token_build[i -first_p] = '\0';

        strcpy(token_p_array[token_i], token_build); //prenesi ric
        token_i++;
    }

    void add_built_delimiter()
    {
        token_build[0] = current_char; //slozi ric (delimiter)
        token_build[1] = '\0';

        strcpy(token_p_array[token_i], token_build); //prenesi ric
        token_i++;
    }

    void add_newline_char()
    {
        strcpy(token_build, "NEWLINECHAR"); //slozi ric (new line character)

        strcpy(token_p_array[token_i], token_build); //prenesi ric
        token_i++;
    }

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        i = first_p;

        if(delimiter_check(current_char)) //Delimiter is a sign that word is done
        {
            if(current_char == '\n') //Include delimiter also
            {
                if(first_p != last_p)
                {
                    add_built_token();
                    add_newline_char();
                }

            }else
            {
                if(current_char == ' ' || current_char == '\t')
                {
                    if(first_p != last_p)
                        add_built_token();

                }else
                {
                    add_built_delimiter();

                    if(first_p != last_p)
                        add_built_token();
                }
            }

            first_p = last_p +1;
        }
    }

    // --> TEST <--

    for(int j = 0; j < token_num; j++)
    {
        printf("%d: %s\n", j, token_p_array[j]);
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

//Delimiter character check
int delimiter_check(const char check_i)
{
    switch(check_i)
    {
        case '%':  return 1; break; //Compiler directive
        case '#':  return 1; break; //Binary word join operator
        case '\"': return 1; break; //String bracket
        case '\'': return 1; break; //Character bracket
        case ';':  return 1; break; //Comment
        case ':':  return 1; break; //Function definition
        case ',':  return 1; break; //Argument/parameter seperator
        case '.':  return 1; break; //Line label
        case ' ':  return 1; break; //word separator (space)
        case '\t': return 1; break; //word separator (tab)
        case '\n': return 1; break; //Instruction end
        case '+':  return 1; break; //Addition operator
        case '-':  return 1; break; //Subtraction operator
        case '*':  return 1; break; //multiplication operator
        case '/':  return 1; break; //Integer division operator
        case '&':  return 1; break; //Logic and operator
        case '|':  return 1; break; //Logic or operator
        case '!':  return 1; break; //Logic invertor operator
        case '(':  return 1; break; //Open bracket for operation priority
        case ')':  return 1; break; //Closed bracket for operation priority
        case '[':  return 1; break; //Open bracket for part selection
        case ']':  return 1; break; //Closed bracket for part selection

        default: return 0;
    }
}

//parser
//generator

