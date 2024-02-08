#include "structs.h"
#include "ioUtils.h"
#include "errors.h"


/*
 * Reads in the inputted file as an ebf
 * and writes out an identical copy into
 * the output file.
*/
int main(int argc, char **argv)
{

    // arg check
    if (argc == 1)
    {
        printf("Usage: ebfEcho file1 file2\n");
        return SUCCESS;
    }
    else if (argc != 3)
    {
        printf("ERROR: Bad Arguments\n");
        return BAD_ARGC;
    }

    // init struct for input ebf
    Image *input = malloc(sizeof(Image));

    int code = ebfReader(input, argv[1]);
    if (code)
    {
        error_printer(code, argv[1]);
        free_image(input);
        return code;
    }

    code = ebfWriter(input, argv[2]);
    if (code)
    {  
        error_printer(code, argv[2]);
        free_image(input);
        return code;
    }

    printf("ECHOED\n");
    return SUCCESS;
}