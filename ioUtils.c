#include "errors.h"
#include "structs.h"
// #include "compressionTools.h"

/*
 * opens the given file and checks that we have
 * correct permissions for it in the given mode.
 */
int file_check(char *filename, char *mode)
{

  // open the file and check it exists
  FILE *file = fopen(filename, mode);
  if (file == NULL)
  {
    return BAD_FNAME;
  }

  // if we're reading, check permissions
  if (mode[0] == 'r')
  {
    // test reading one character
    char c = getc(file);
    if (c == ' ')
    {
      return BAD_FNAME;
    }
    // put it back if we can read
    ungetc(c, file);
  }

  return 0;
}

/*
 * scans lines for comments and absorbs any found
 * allows error checking for e files which should
 * not contain any comments!
 */
int comment_eater(FILE *file)
{
  char line[MAX_LINE_LENGTH];
  char c = ' ';
  c = getc(file);

  // check if it's a comment line
  if (c == '#')
  {
    // if it is - put back and then read & absorb the line
    ungetc(c, file);
    fgets(line, MAX_LINE_LENGTH, file);
    return 1;
  }

  // otherwise - put it back and return pseudofalse
  ungetc(c, file);
  return 0;
}

/*
 * read and validate the magic number from image files
 * expects you to enter the expected magic_number
 * for pgm you can use p since we don't know
 * which flavour we get!
 */
int get_magic_number(Image *img, FILE *file, char *expected)
{
  while (comment_eater(file))
  {
    // if there's a comment, need to return an error
    if (expected[0] == 'e')
    {
      return BAD_COMMENT;
    }
  }
  // set up a line to read the string & absorb newlines and similar
  // has to be decently long..
  char magic_number[100];

  fgets(magic_number, 100, file);
  if (expected[0] == 'p')
  {
    // if I don't get either P2 or P5
    if (magic_number[0] != 'P' || !(magic_number[1] != '2' || magic_number[1] != '5'))
    {
      return BAD_MAGIC_NUMBER;
    }
  }
  else
  {
    if (magic_number[0] != expected[0] || magic_number[1] != expected[1])
    {
      return BAD_MAGIC_NUMBER;
    }
  }

  // only copy if successful!
  strcpy(img->magic_number, magic_number);
  return 0;
}

/*
 * read the dimesions and swap them over if
 * required by the file format (eb/eu/ec)
 */
int get_dimensions(Image *img, FILE *file)
{
  while (comment_eater(file))
  {
    if (img->magic_number[0] == 'e' || img->magic_number[0] == 'E')
    {
      return BAD_COMMENT;
    }
  }

  // in order to gradefully handle any bad data, fgets and sscanf
  char line[MAX_LINE_LENGTH];
  fgets(line, MAX_LINE_LENGTH, file);

  int valid = sscanf(line, "%d %d ", &img->width, &img->height);

  // validate that the dimensions a) exist and b) are appropriate
  if (valid != 2 ||
      img->width > MAX_DIM ||
      img->width < MIN_DIM ||
      img->height > MAX_DIM ||
      img->height < MIN_DIM)
  {
    return BAD_DIM;
  }

  // need to swap them if it's eb/eu/ec
  if (img->magic_number[0] == 'e' || img->magic_number[0] == 'E')
  {
    int swap = img->height;
    img->height = img->width;
    img->width = swap;
  }

  return 0;
}

/*
 * exclusively used for PGM files - read in the
 * maxgrey value.
 */
int get_max_grey(Image *img, FILE *file)
{
  while (comment_eater(file))
  {
    // run comment eater until it becomes false to strip comments
  }

  char line[MAX_LINE_LENGTH];
  fgets(line, MAX_LINE_LENGTH, file);
  int valid = sscanf(line, "%d", &img->max_grey);
  if (!valid || img->max_grey < 0 || img->max_grey > 255)
  {
    return BAD_MAX_GRAY;
  }
  return 0;
}

/*
 * Read in and validate ascii pixel data
 */
int get_pixels_a(Image *img, FILE *file)
{

  while (comment_eater(file))
  {
    // run comment eater until it becomes false to strip comments
  }
  // we will be using a counter to keep track of how many items have been read in.
  int counter = 0;

  // Read in the pixel values from the file and store them in the 2D array
  for (int row = 0; row < img->height; row++)
  {
    for (int col = 0; col < img->width; col++)
    {
      counter += fscanf(file, " %hhd ", &img->pixels[row][col]);
      if (img->pixels[row][col] > img->max_grey || img->pixels[row][col] < 0)
      {
        return BAD_DATA;
      }
    }
  }
  // accounts for any stray newlines at the end of the file
  fscanf(file, "\n ");

  // if either not at the end, or there were too few - fail
  if (!feof(file) || img->width * img->height != counter)
  {
    return BAD_DATA;
  }

  return 0;
}

/*
 * Read in and validate any binary pixel data
 */
int get_pixels_b(Image *img, FILE *file)
{

  while (comment_eater(file))
  {
    // run comment eater until it becomes false to strip comments
  }

  // REJIG - read in as one block
  int check = fread(img->pixel_block, 1, img->height * img->width, file);

  for (int row = 0; row < img->height; row++)
  {
    for (int col = 0; col < img->width; col++)
    {
      if (img->pixels[row][col] > img->max_grey || img->pixels[row][col] < 0)
      {
        return BAD_DATA;
      }
    }
  }

  // accounts for any stray newlines at the end of the file
  fscanf(file, "\n ");

  // if either not at the end, or there were too few - fail
  if (!feof(file) || (img->width * img->height) != check)
  {
    return BAD_DATA;
  }

  return 0;
}

/*
 * Writes out the pixels array to a given file pointer
 * since we have already checked writing, no need to
 * validate this.
 */
void ascii_writer(Image *img, FILE *file)
{

  for (int row = 0; row < img->height; row++)
  {
    for (int col = 0; col < img->width; col++)
    {
      if (img->magic_number[0] == 'P')
      {
        // if it came from a pgm file, shift down
        // REMOVED FOR THE MOMENT
        fprintf(file, "%d", (img->pixels[row][col]));
      }
      else
      {
        fprintf(file, "%d", img->pixels[row][col]);
      }

      // to make it neat, only put a space when it's not the end in a row
      if (col != img->width - 1)
      {
        fprintf(file, " ");
      }
    }

    // likewise for newlines
    if (row != img->height - 1)
    {
      fprintf(file, "\n");
    }
  }
}

/*
 * Composite function used to read any given image format
 * header data and appropriately validate.
 */
int header_reader(Image *img, FILE *file, char *format)
{

  int check = get_magic_number(img, file, format);
  if (check)
  {
    return check;
  }

  // Read the next line, which should contain the width and height of the img
  check = get_dimensions(img, file);
  if (check)
  {
    return check;
  }

  // now we have a valid width & height -> dynamic allocate memory
  check = allocate_pixels(img);
  if (check)
  {
    return check;
  }

  // only read magic_number for PGMs -> always 31 for others.
  if (format[0] == 'p')
  {
    check = get_max_grey(img, file);
    if (check)
    {
      return check;
    }
  }
  else
  {
    img->max_grey = 31;
  }

  return SUCCESS;
}

/*
 * reads in a given PGM file (ascii or binary)
 * validating all data and storing in a given
 * image struct.
 */
int pgmReader(Image *img, char *filename)
{

  int check = file_check(filename, "rb");
  if (check)
  {
    return check;
  }

  FILE *file = fopen(filename, "rb");

  check = header_reader(img, file, "pgm");
  if (check)
  {
    return check;
  }

  // decide whether we're handling binary or ascii
  if (img->magic_number[1] == '2')
  {
    check = get_pixels_a(img, file);
    if (check)
    {
      return check;
    }
  }
  else
  {
    check = get_pixels_b(img, file);
    if (check)
    {
      return check;
    }
  }

  fclose(file);

  return 0;
}

/*
 * reads in an ebf file, storing the data into
 * a given image struct.
 */
int ebfReader(Image *img, char *filename)
{

  int check = file_check(filename, "r");
  if (check)
  {
    return check;
  }

  FILE *file = fopen(filename, "r");

  check = header_reader(img, file, "eb");
  if (check)
  {
    return check;
  }

  check = get_pixels_a(img, file);
  if (check)
  {
    return check;
  }

  fclose(file);

  return 0;
}

/*
 * Reads in an ebu file into a given struct
 */
int ebuReader(Image *img, char *filename)
{

  int check = file_check(filename, "rb");
  if (check)
  {
    return check;
  }

  FILE *file = fopen(filename, "rb");

  check = header_reader(img, file, "eu");
  if (check)
  {
    return check;
  }

  check = get_pixels_b(img, file);
  if (check)
  {
    return check;
  }

  fclose(file);

  return 0;
}

/*
 * takes a given struct of ANY image type and
 * writes it out to an ebf file.
 */
int ebfWriter(Image *img, char *filename)
{

  int check = file_check(filename, "w");
  if (check)
  {
    return check;
  }
  FILE *file = fopen(filename, "w");

  // print the header data...
  check = fprintf(file, "eb\n");
  // check write successful.
  if (!check){
    return BAD_OUTPUT;
  }
  fprintf(file, "%d %d\n", img->height, img->width);

  ascii_writer(img, file);

  fclose(file);

  return 0;
}

/*
 * takes a given struct of ANY image type and
 * writes it out to a pgm file.
 */
int pgmWriter(Image *img, char *filename)
{

  int check = file_check(filename, "w");
  if (check)
  {
    return check;
  }

  FILE *file = fopen(filename, "w");

  // print the header data...
  check = fprintf(file, "P2\n");
  // check write successful.
  if (!check){
    return BAD_OUTPUT;
  }
  fprintf(file, "%d %d\n", img->width, img->height);
  fprintf(file, "255\n");

  // now we do the loop to write our data...

  ascii_writer(img, file);

  fclose(file);

  return 0;
}

/*
 * takes a given struct of ANY image type and
 * writes it out to an ebu file.
 */
int ebuWriter(Image *img, char *filename)
{

  int check = file_check(filename, "wb");
  if (check)
  {
    return check;
  }

  FILE *file = fopen(filename, "wb");

  // print the header data...
  check = fprintf(file, "eu\n");
  // check write successful.
  if (!check){
    return BAD_OUTPUT;
  }
  fprintf(file, "%d %d\n", img->height, img->width);

  fwrite(img->pixel_block, 1, img->height * img->width, file);

  fclose(file);

  return 0;
}