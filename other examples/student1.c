/**
 This is a student submission from last year

 It contains what I call a 'megafunction'
 The student hasn't really understood how to approach modularity
 And has therefore just copied and pasted a large chunk into a function.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct data
    {
    int returnCode;
    unsigned char **imageData;
    int width;
    int height;
    long numBytes;
    unsigned short *magicNumberValue;
    } resultsData;

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAGIC_NUMBER 0x7565
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1
#define MAX_MEMORY_SIZE 2147483648 // 2G

resultsData *dataValidation(char* fileName)
    { // Data validation function
	
    resultsData *results;
    results = malloc(sizeof(resultsData));
        
    // create a char array to hold magic number
    // and cast to short
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue = (unsigned short *)magicNumber;

    // create and initialise variables used within code
    int width = 0, height = 0;
    long numBytes;

    // open the input file in read mode
    FILE *inputFile = fopen(fileName, "rb");
    // check file opened successfully
    if (!inputFile)
        { // check file pointer
        printf("ERROR: Bad File Name (%s)\n", fileName);
        results->returnCode = BAD_FILE;
	return results;
        } // check file pointer

    // get first 2 characters which should be magic number
    magicNumber[0] = getc(inputFile);
    magicNumber[1] = getc(inputFile);

    // checking against the casted value due to endienness.
    if (*magicNumberValue != MAGIC_NUMBER)
        { // check magic number
        printf("ERROR: Bad Magic Number (%s)\n", fileName);
        results->returnCode = BAD_MAGIC_NUMBER;
	return results;
        } //check magic number

    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    int check = fscanf(inputFile, "%d %d", &height, &width);
    if (check != 2 || height < MIN_DIMENSION || width < MIN_DIMENSION || height > MAX_DIMENSION || width > MAX_DIMENSION)
        { // check dimensions
        // close the file as soon as an error is found
        fclose(inputFile);
        // print appropriate error message and return
        printf("ERROR: Bad Dimensions (%s)\n", fileName);
        results->returnCode = BAD_DIM;
	return results;
        } // check dimensions

    // caclulate total size and allocate memory for array
    numBytes = height * width;
    // if malloc is unsuccessful, it will return a null pointer
    unsigned char **imageData = (unsigned char **)malloc(height * sizeof(unsigned char *));
    if (imageData == NULL)
        { // check malloc
	free(imageData);
        fclose(inputFile);
        printf("ERROR: Image Malloc Failed\n");
        results->returnCode = BAD_MALLOC;
        return results;
        } // check malloc
    for (int i = 0; i < height; i++)
	{
        imageData[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
	if (imageData[i] == NULL)
	    { // check malloc
	    for (int j = 0; j < i; j++)
		{
		free(imageData[j]);
		}
	    free(imageData);
	    fclose(inputFile);
	    printf("ERROR: Image Malloc Failed\n");
            results->returnCode = BAD_MALLOC;
            return results;
	    } // check malloc
	}

    size_t requiredMemory = (size_t)height * (size_t)width * sizeof(unsigned int);
    if (requiredMemory > MAX_MEMORY_SIZE) 
        {
        fclose(inputFile);
        printf("ERROR: Image Malloc Failed\n");
	results->returnCode = BAD_MALLOC;
        return results;
        }

    // read in each grey value from the file
    for (int current = 0; current < numBytes; current++)
        { // reading in
        check = fread(&imageData[current / width][current % width], sizeof(unsigned char), 1, inputFile);
        // validate that we have captured 1 pixel value
        if (check != 1 || imageData[current / width][current % width] < 0 || imageData[current / width][current % width] > 31)    
	    { // check inputted data
            // ensure that allocated data is freed before exit.
            for (int i = 0; i < height; i++)
	        {
		free (imageData[i]);
		}
            free(imageData);
            fclose(inputFile);
            printf("ERROR: Bad Data (%s)\n", fileName);
            results->returnCode = BAD_DATA;
	    return results;
            } // check inputted data
        } // reading in

    // check for too many greyvalues
    unsigned int temp;
    check = fread(&temp, sizeof(unsigned int), 1, inputFile);
    if (check != 0)
        {
        // ensure that allocated data is freed before exit.
        for (int i = 0; i < height; i++)
	    {
            free(imageData[i]);
	    }
        free(imageData);
        fclose(inputFile);
        printf("ERROR: Bad Data (%s)\n", fileName);
	results->returnCode = BAD_DATA;
	return results;
	}

    // now we have finished using the inputFile we should close it
    fclose(inputFile);
    
    results->returnCode = SUCCESS;
    results->width = width;
    results->height = height;
    results->numBytes = numBytes;
    results->magicNumberValue = magicNumberValue;
    results->imageData = malloc(height * sizeof(unsigned int *));
    for (int i = 0; i < height; i++) 
        {
        results->imageData[i] = malloc(width * sizeof(unsigned int));
        for (int j = 0; j < width; j++) 
	    {
            results->imageData[i][j] = imageData[i][j];
            }
        }

    // free allocated memory
    for (int i = 0; i < height; i++)
        {
	free(imageData[i]);
	}
    free(imageData);

    return results;

    } // dataValidation()

