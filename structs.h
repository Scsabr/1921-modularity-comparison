#ifndef STRUCTS_DEFINES
#define STRUCTS_DEFINES


// the libraries which are used by everything else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 100
#define MAX_DIM 131072
#define MIN_DIM 1

// using one struct for everything at the moment..
// not huge differences between the images.
typedef struct
{
  char magic_number[2];
  int width;
  int height;
  int max_grey;
  uint8_t **pixels;
  uint8_t *pixel_block;
} Image;

// useful struct functions.
void free_image(Image *img);
int allocate_pixels(Image *img);

#endif