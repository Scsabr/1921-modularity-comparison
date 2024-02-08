#ifndef ERROR_CODES
#define ERROR_CODES

#define BAD_ARGC 1
#define BAD_FNAME 2
#define BAD_MAGIC_NUMBER 3
#define BAD_COMMENT 9
#define BAD_DIM 4
#define BAD_MAX_GRAY 11
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define BAD_LAYOUT 8
#define MISC_ERROR 100
#define SUCCESS 0

void error_printer(int code, char* filename);

#endif