// file to deal with the more onerous error handling..

#include <stdio.h>

#include "errors.h"

/*
 * from a given error code, prints the required error message to the user
 * including the filename, which is not accessible in many of the
 * sub-procedures which return error codes.
 */
void error_printer(int code, char *filename)
{
    // they all start in the same way
    printf("ERROR: ");

    switch (code)
    {
    case BAD_FNAME:
        printf("Bad File Name (%s)", filename);
        break;
    case BAD_MAGIC_NUMBER:
        printf("Bad Magic Number (%s)", filename);
        break;
    case BAD_COMMENT:
        printf("Bad Comment (%s)", filename);
        break;
    case BAD_DIM:
        printf("Bad Dimensions (%s)", filename);
        break;
    case BAD_MAX_GRAY:
        printf("Bad Max Grey Value (%s)", filename);
        break;
    case BAD_MALLOC:
        printf("Image Malloc Failed");
        break;
    case BAD_DATA:
        printf("Bad Data (%s)", filename);
        break;
    case BAD_OUTPUT:
        printf("Bad Output (%s)", filename);
        break;
    case BAD_LAYOUT:
        printf("Bad Layout (%s)", filename);
        break;
    case MISC_ERROR:
        printf("Miscelaneous Error (%s)", filename);
        break;
    }
    // and follow up with a newline.
    printf("\n");
}