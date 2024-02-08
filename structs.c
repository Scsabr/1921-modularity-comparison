#include "structs.h"
#include "errors.h"

/*
 * frees any malloced data from a given struct
 */
void free_image(Image *img)
{

    // check we're not freeing null..
    if (img->pixels != NULL)
    {
        // for (int row = 0; row < img->height; row++)
        // {
        //     // free each row
        //     free(img->pixels[row]);
        // }
        free(img->pixel_block);
        free(img->pixels);
    }

    // free the whole struct
    free(img);
}

/*
 * Allocates **pixels with the read in height and width.
 * Validates against the unlikely possibility of a failed malloc
 */
int allocate_pixels(Image *img)
{
    img->pixel_block = (uint8_t *)malloc(img->height * img->width * sizeof(uint8_t));
    img->pixels = (uint8_t **)malloc(img->height * sizeof(uint8_t *));
    for (int row = 0; row < img->height; row++)
    {
        img->pixels[row] = img->pixel_block + img->width * row;
        //  img->pixels[row] = (uint8_t *)malloc(img->width * sizeof(uint8_t));
    }

    if (img->pixels == NULL)
    {    
        return BAD_MALLOC;
    }

    for (int row = 0; row < img->height; row++)
    {
        if (img->pixels[row] == NULL)
        {
	return BAD_MALLOC;
	}
    }    

    return SUCCESS;
}