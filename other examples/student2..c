/*
 Another student - this one has had a reasonable go at modularising their code
 But we still have lots of repeated functionality
 And shared responsibility.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct{
    int width;
    int height;
    long numBytes;
    unsigned int **imageData;
    unsigned int *imageDataBlock;
    unsigned int **storeData;
    unsigned int *storeDataBlock;
    unsigned char magicNumber[2];
    unsigned short * magicNumberValue;
    //for storing size8
    int size;

    }Initalization;

int ebfDataRead(FILE *input,Initalization *pointer, char* arg){
    long size = 0;
    long numBytes = pointer->width * pointer->height;
    for(int i = 0; i<pointer->height; i++){
        for(int j = 0 ; j<pointer->width; j++){
            //When data is correctly read, increment size by 1
            if(fscanf(input,"%u",&pointer->imageData[i][j]) == 1){
                //increment counter by 1
                size++;
            }else{
                //stop incrementing when invalid data is read
                break;
            }
        }
    }
    
    char store [50];
    //initialize inputFile pointer back to first line
    fseek(input,0,SEEK_SET);
    //increment inputFile pointer to line 3
    for (int i = 1;fgets(store,50,input);i++){
        if(i == 2){
            long linePos = ftell(input);
            fseek(input,linePos,SEEK_SET);
            break;
        }
    }
    // read in each grey value from the file
    for(int currenth = 0; currenth < pointer->height; currenth++)
    {
        for(int currentw = 0; currentw < pointer->width; currentw++)
        {
            //reads in each data
            int check = fscanf(input, "%u", &pointer->imageData[currenth][currentw]);
             // validate that we have captured 1 pixel value
            if (check != 1 || pointer->imageData[currenth][currentw] < 0 
            || pointer->imageData[currenth][currentw] >= 32 || size>numBytes)
            {// check inputted data
            // ensure that allocated data is freed before exit.
                free(pointer->imageData);
                fclose(input);
                printf("ERROR: Bad Data (%s)\n",arg);
                return 1;
            }// check inputted data
        }// reading in
    }
    //checks if there is data after last data is read
    int c = getc(input);
    if (c != EOF){
        fclose(input);
        printf("ERROR: Bad Data (%s)\n",arg);
        return 1;
    }
    return 0;
}

int ebuDataRead(FILE *input,Initalization *pointer, char* arg){
    long size = 0;
    long numBytes = pointer->height * pointer->width;
    //reads first space of data
    fread(&pointer->imageData[0][0],1,1,input);
    for(int i = 0; i<pointer->height; i++){
        for(int j = 0 ; j<pointer->width; j++){
            //increment size by 1 when data element is successfully read
            if(fread(&pointer->imageData[i][j],1,1,input) == 1){
                size++;
            }else{
                //stop incrementing when invalid data is read, escape for loop
                break;
            }
        }
    }

    char store [50];
    //initialize inputFile pointer back to first line
    fseek(input,0,SEEK_SET);
    //increment inputFile pointer to line 3
    for (int i = 1;fgets(store,50,input);i++){
        if(i == 2){
            long linePos = ftell(input);
            fseek(input,linePos,SEEK_SET);
            break;
        }
    }
    // read in each grey value from the file
    for(int currenth = 0; currenth < pointer->height; currenth++)
    {
        for(int currentw = 0; currentw < pointer->width; currentw++)
        {
            //read in grey values
            int check = fread(&pointer->imageData[currenth][currentw],1,1,input);
             // validate that we have captured 1 pixel value
            if (check != 1 || pointer->imageData[currenth][currentw] < 0 
            || pointer->imageData[currenth][currentw] >= 32 || size>numBytes)
            {// check inputted data
            // ensure that allocated data is freed before exit.
                free(pointer->imageData);
                fclose(input);
                printf("ERROR: Bad Data (%s)\n",arg);
                return 1;
            }// check inputted data
            
        }// reading in
    }
    //check if there are characters beyond amount of numbers
    int c = getc(input);
    if (c != EOF){
        fclose(input);
        printf("ERROR: Bad Data (%s)\n",arg);
        return 1;
    }
    return 0;
}

int GET_BIT(int number, int n){
    //create binary number 0000 0001
    uint8_t Bits = 0x01; 
    //move the 1 towards the desired place, and mask out numbers
    //apart from the place with 1
    int binary_get = number & (Bits << n);
    return binary_get;
}

int ebu2ebcDataR(FILE *input,Initalization *pointer,char* arg, unsigned int *Array){
    long size = 0;
    long numBytes = pointer->height * pointer->width;
    //reads in frist space of the file
    fread(&pointer->imageData[0][0],1,1,input);
    for(int i = 0; i<pointer->height; i++){
        for(int j = 0 ; j<pointer->width; j++){
            //condition when data element is successfully read
            if(fread(&pointer->imageData[i][j],1,1,input) == 1){
                //increment counter by 1
                size++;
            }else{
                //stop incrementing when invalid data is read
                break;
            }
        }
    }

    char store [50];
    //initialize inputFile pointer back to first line
    fseek(input,0,SEEK_SET);
    //increment inputFile pointer to line 3
    for (int i = 1;fgets(store,50,input);i++){
        if(i == 2){
            long linePos = ftell(input);
            fseek(input,linePos,SEEK_SET);
            break;
        }
    }

    int increment = 0;
    // read in each grey value from the file
    for(int currenth = 0; currenth < pointer->height; currenth++)
    {
        for(int currentw = 0; currentw < pointer->width; currentw++)
        {
            int check = fread(&pointer->imageData[currenth][currentw],1,1,input);
             // validate that we have captured 1 pixel value
            if (check != 1 || pointer->imageData[currenth][currentw] < 0 
            || pointer->imageData[currenth][currentw] >= 32 || size>numBytes)
            {// check inputted data
            // ensure that allocated data is freed before exit.
                free(pointer->imageData);
                fclose(input);
                printf("ERROR: Bad Data (%s)\n",arg);
                return 1;
            }// check inputted data

            //Place each captured value into inputArray
            Array[increment] = pointer->imageData[currenth][currentw];
            //increases increment by 1 to set next inputArray position
            increment ++;
        }// reading in
    }
    //check if there are any values beyond the amount of numbers specified
    int c = getc(input);
    if (c != EOF){
        fclose(input);
        printf("ERROR: Bad Data (%s)\n",arg);
        return 1;
    }
    //close input file
    fclose(input);
    return 0;
}

int ebu2ebcDataW(Initalization* pointer,FILE* file, unsigned int *Array, long loop,char *str,int height,int width){
    
    int counter;
    int pointtoMove;
    unsigned int bitmask3;
    int gotBit;

    //print the magic number, height and width to file
    fprintf(file, "%s\n%d %d\n",str,height,width);
    //Reading to conversion
    uint8_t placementBits = 0;
    counter = 0;
    pointtoMove = 8;
    /*for(int i = 0; i<100; i++){
        printf("data %d = %d\n",i,Array[i]);
    }*/
    for(int i = 0; i<loop; i++){
        bitmask3 = Array[i] << 3;
        for(int k = 1; k <= 5; k++){
            pointtoMove --;
            counter++;
            //gets the bit of specified location from number 
            //in inputArray[i] 
            gotBit = GET_BIT(bitmask3,8-k);
            //place a 1 in specified location of placementBits
            //if the gotBit is not 0
            if(gotBit != 0){
                placementBits = placementBits|(1<<pointtoMove);
            }
            //push the 8 bit number to output file once all
            //bits of placementBits are filled (i.e. counter = 8) 
            if(counter == 8){
            fwrite(&placementBits,1,1,file);
            //reset everything
            placementBits = 0;
            counter = 0;
            pointtoMove = 8;
            }
        }     
    }
    //catered for the last number being read in, yet is not
    //being pushed yet due to not all placementBits filled
    if(counter != 0){
        fwrite(&placementBits,1,1,file);
    }
    //free data and close output file
    free(pointer->imageData);
    fclose(file);
    return 0;
}

int ebu2ebcDataW128(Initalization* pointer,FILE* file, unsigned int *Array, long loop,char *str,int height,int width){
    int counter;
    int pointtoMove;
    unsigned int bitmask3;
    int gotBit;
    int test = 0;

    //print the magic number, height and width to file
    fprintf(file, "%s\n%d %d\n",str,height,width);
    //Reading to conversion
    uint8_t placementBits = 0;
    counter = 0;
    pointtoMove = 8;
    for(int i = 0; i<loop; i++){
        bitmask3 = Array[i] << 1;
        for(int k = 1; k <= 7; k++){
            pointtoMove --;
            counter++;
            //gets the bit of specified location from number 
            //in inputArray[i] 
            gotBit = GET_BIT(bitmask3,8-k);
            //place a 1 in specified location of placementBits
            //if the gotBit is not 0
            if(gotBit != 0){
                placementBits = placementBits|(1<<pointtoMove);
            }
            //push the 8 bit number to output file once all
            //bits of placementBits are filled (i.e. counter = 8) 
            if(counter == 8){
            test++;
            fwrite(&placementBits,1,1,file);
            //reset everything
            placementBits = 0;
            counter = 0;
            pointtoMove = 8;
            }
        }     
    }
    //catered for the last number being read in, yet is not
    //being pushed yet due to not all placementBits filled
    if(counter != 0){
        fwrite(&placementBits,1,1,file);
    }
    //free data and close output file
    free(pointer->imageData);
    fclose(file);
    return 0;
}

int ebc2ebuDataRandW(FILE *input,Initalization *pointer, char* arg, char* arg2,int dataCheck){

    //initalize values
    int size8 = 0;
    long size5 = 0;
    long numBytes = pointer->height * pointer->width;
    unsigned int *inputArray = (unsigned int*)malloc(numBytes*sizeof(unsigned int));
    int gotBit;
    uint8_t placementBits = 0;
    int counter = 0;
    int placementint;
    int increment = 0;
    int temp = 0;
    int pointtoMove = 8;
    int isBreak = false;

    //read in first space
    fread(&inputArray[0],1,1,input);
    for(int i = 0; i<numBytes; i++){
        //read in first 8 bits of data from input file 
        int checker = fread(&inputArray[i],1,1,input);
        if(checker != 1){
            break;
        }
        //increment size8 every 8 bit read
        size8++;
        //loops through each bit of the 8 bits
        for(int k = 1; k <= 8; k++){
            counter++;
            //increment size5 every 5 bit read
            if(counter == 5){
                size5++;
                counter = 0;
            }
        }
    }
    if(dataCheck == 1){
        int badData = 1;
        // check if the number of elements exceeds
        //the number of elements that should exist
        if(size5 == numBytes+1 || size5 == numBytes){
            badData = 0;
        }
        //check if number of elements in file is equal
        //to number of elements that is supposed to have
        if(badData == 1){
            fclose(input);
            printf("ERROR: Bad Data (%s)\n",arg);
            return 1;
        }
    }
    

    char store [50];
    //initialize inputFile pointer back to first line
    fseek(input,0,SEEK_SET);
    //increment inputFile pointer to line 3
    for (int i = 1;fgets(store,50,input);i++){
        if(i == 2){
            long linePos = ftell(input);
            fseek(input,linePos,SEEK_SET);
            break;
        }
    }
    //Reinitialize values
    placementBits = 0;
    counter = 0;
    //make array for storing uncompressed binary data
    unsigned int *inputArray2 = (unsigned int*)malloc
    (numBytes*sizeof(unsigned int));

    //Read in data, and convert data from compressed binary to binary data 
    for(int i = 0; i<size8; i++){
        //read in first 8 bits of data from input file  
        //loop through each bit of the 8 bits 
        for(int k = 1; k <= 8; k++){
            pointtoMove --;
            counter++;
            //get the corresponding bit from the data 
            gotBit = GET_BIT(inputArray[i],8-k);   //k=5  10110101 0101010110
            //add 1 to the corresponding position of placementBits
            //if gotBit is not 0
            if(gotBit != 0){
                placementBits = placementBits|(1<<pointtoMove);
            }
            //if any of these are fulfilled, do the following
            if(counter == 5 && (k == 5-temp || 8-k == temp-5)){
                //right shift placementBits by 3 
                //(e.g. 00011000 --> 00000011 )
                placementBits >>= 3;
                placementint = placementBits;
                //place the placementBits value into inputArray2
                inputArray2[increment] = placementint;
                increment++;
                temp = 8-k;
                //reset everything
                placementBits = 0;
                counter = 0;
                pointtoMove = 8;
            }
            if (increment >= size5){
                isBreak = true;
            }
        }
        if(isBreak == true){
            break;
        }
    }
    
    //place the remaining placementBit if
    //counter hasnt reached 5 yet, but loop is broken
    if(counter != 0){
        placementBits >>= 3;
        placementint = placementBits;
        inputArray2[increment] = placementint;
    }
    //put in data from inputArray2 into our 2D array previously allocated **
    int k = 0;
    for(int i = 0; i<pointer->height; i++){
        for(int j = 0; j<pointer->width; j++){
            pointer->imageData[i][j] = inputArray2[k++];
        }
    }
    free(inputArray2);
    fclose(input);

    return size5;
}

int ebcDataRandW(FILE *input,Initalization *pointer, char* arg, char* arg2){
    //assign number of values the file should have
    long numBytes = pointer->height * pointer->width;
    unsigned int inputArray[numBytes];
    // read in each grey value from the file

    //initialize all values
    int counter = 0;
    int pointtoMove;
    int increment = 0;
    int increment2 = 0;
    int temp = 0;
    pointtoMove = 8;
    int isBreak = 0;
    int size8 = 0;
    int size5 = 0;

    //counts the number of values in the file
    //as 8 bits and as 5 bits
    fread(&inputArray[0],1,1,input);
    for(int i = 0; i<numBytes; i++){
        //read in first 8 bits of data from input file 
        int checker = fread(&inputArray[i],1,1,input);
        if(checker != 1){
            break;
        }
        //increment size8 after reading every 8 bits
        size8++;
        for(int k = 1; k <= 8; k++){
            counter++;
            if(counter == 5){
                //increment size5 after reading every 5 bits
                size5++;
                counter = 0;
            }
        }
    }

    //checks if number of data in file is equal to 
    // the number of values that should be in file
    if(size5 != numBytes){
        free(pointer->imageData);
        fclose(input);
        printf("ERROR: Bad Data (%s)\n",arg);
        return 1;
    }

    char store [50];
    //initialize inputFile pointer back to first line
    fseek(input,0,SEEK_SET);
    //increment inputFile pointer to line 3
    for (int i = 1;fgets(store,50,input);i++){
        if(i == 2){
            long linePos = ftell(input);
            fseek(input,linePos,SEEK_SET);
            break;
        }
    }

    //reset counter
    counter = 0;

    //Start doing bitshift
    while(!feof(input)){
        for(int currenth = 0; currenth<pointer->height; currenth++){
            for(int currentw = 0; currentw<pointer->width; currentw++){
                //read each 8 bit data in file
                fread(&pointer->imageData[currenth][currentw],1,1,input);
                //loop through each bit of the 8 bit number   
                for(int k = 1; k <= 8; k++){
                    pointtoMove --;
                    counter++;
                    if(counter == 5 && (k == 5-temp || 8-k == temp-5)){
                        //increments by 1 when every 5 bits is looped through
                        increment++;
                        //breaks out of for loop when enough bits are read
                        if(increment == numBytes){
                            isBreak = 1;
                            break;
                        }
                        temp = 8-k;
                        counter = 0;
                        pointtoMove = 8;
                    }
                }
                //increments by 1 when 8 bits are read
                if(isBreak != 1){
                    increment2++;
                }
            }
        }
    }
    if(counter != 0){
        increment2++;
    }
    fclose(input);

    FILE* outputFile = fopen(arg2, "wb");

    //check if output has permissions to write
    // check file pointer
    if (outputFile == NULL){ 
        printf("ERROR: Bad File Name (%s)\n",arg2);
        return 2;
        } 

    //prints out header, and height and width
    fprintf(outputFile, "ec\n%d %d\n", pointer->height, pointer->width);
    //write all data stored into output file
    for (int i = 0; i < pointer->height; i++){
        for(int j = 0; j < pointer->width; j++){
            fwrite(&pointer->imageData[i][j],1,1,outputFile);
            //decreases increment2 by 1 every time 8 bits are looped
            increment2--;
            //condition to stops writing and close file 
            if(increment2 == 0){
                //free image data and  close the file
                free(pointer->imageData);
                fclose(outputFile);
                return 0;
            }
        }
    }
    return 0;
}

int ebcDataRead(FILE *input,Initalization *pointer, char* arg, unsigned int inputArray[]){

    //Initialize values
    int counter = 0;
    long numBytes = pointer->height*pointer->width;
    int size8 = 0;
    int size5 = 0;
    
    //counts the number of values in the file
    //as 8 bits and as 5 bits
    fread(&inputArray[0],1,1,input);
    for(int i = 0; i<numBytes; i++){
        //read in first 8 bits of data from input file 
        int checker = fread(&inputArray[i],1,1,input);
        if(checker != 1){
            break;
        }
        //increment size8 after reading every 8 bits
        size8++;
        for(int k = 1; k <= 8; k++){
            counter++;
            if(counter == 5){
                //increment size5 after reading every 5 bits
                size5++;
                counter = 0;
            }
        }
    }
    //set size to size8, for use in pixelComp comparison
    pointer->size = size8;
    //checks if number of data in file is equal to 
    // the number of values that should be in file
    int badData = 1;
    
    // check if the number of elements exceeds
    //the number of elements that should exist
    if(size5 == numBytes+1 || size5 == numBytes){
        badData = 0;
    }
    //if so, free imageData and close file
    if(badData == 1){
        free(pointer->imageData);
        fclose(input);
        printf("ERROR: Bad Data (%s)\n",arg);
        return 1;
    }

    char store [50];
    //initialize inputFile pointer back to first line
    fseek(input,0,SEEK_SET);
    //increment inputFile pointer to line 3
    for (int i = 1;fgets(store,50,input);i++){
        if(i == 2){
            long linePos = ftell(input);
            fseek(input,linePos,SEEK_SET);
            break;
        }
    }

    //for breaking out double for loop
    int isBreak = 0;

        for(int currenth = 0; currenth<pointer->height; currenth++){
            for(int currentw = 0; currentw<pointer->width; currentw++){
                //read in each 8 bits from ebc file and store them
                fread(&pointer->imageData[currenth][currentw],1,1,input);
                //break out of inner loop when sufficient data is read  
                if(numBytes == size8){
                    isBreak = 1;
                    break;
                }
            }
            //break out of outer loop after inner loop is broken
            if(isBreak == 1){
                break;
            }
        }

    return 0;
}