#include "structs.h"
#include "errors.h"
#include "ioUtils.h"


/*
 Need to include assert to use it
*/
#include <assert.h>

int main()
{

    printf("Test - loading bad filename\n");
    // Assert lets you check a boolean evaluates to true
    // in this case, I want to make sure that if I try and open
    // a file which doesn't exist, it should return BAD_FNAME
    // which is defined in errors.h
    assert(file_check("fake","r")==BAD_FNAME);

    printf("Test - loading good filename\n");
    assert(file_check("ebfEcho.c","r")==SUCCESS);

}
