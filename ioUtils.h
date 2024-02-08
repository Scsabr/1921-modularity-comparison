#ifndef ioUtilsER
#define ioUtilsER

#include "structs.h"

int pgmReader(Image *img, char *filename);
int ebfReader(Image *img, char *filename);
int ebfWriter(Image *img, char *filename);
int pgmWriter(Image *img, char *filename);
int ebuWriter(Image *img, char *filename);
int ebuReader(Image *img, char *filename);
int ebcWriter(Image *img, char *filename);
int ebcReader(Image *img, char *filename);

int file_check(char *filename, char *mode);
int header_reader(Image *img, FILE *file, char *format);


#endif