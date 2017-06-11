#include <stdio.h>
#include <stdlib.h>
#include "includes/lodepng.h"
#include "sources/lodepng.c"
#include "includes/preproc.h"

#define IMAGE "../images/lion.png"
#define NEWIMAGE "../imagesNew/lion.png"

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height);
void decodeOneStep(const char* filename);

/**
 * Takes png image in same directory, gets raw pixel data, and performs gaussBlur()
 * on it.
 */
void decodeOneStep(const char* filename)
{
    unsigned error;
    unsigned char* image; //will point to first (upper left) pixel after png decode
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, filename);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    /* Perform image processing here********************************************/
    grayscale(image, width, height);

    /****************************************************************************/

    encodeOneStep(NEWIMAGE, image, width, height); /*save image to new image in /imagesNew directory*/
    free(image);
}

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
    FILE *newimage;
    newimage = fopen(filename, "wb");
    /*Encode the image*/
    unsigned error = lodepng_encode32_file(filename, image, width, height);

    /*if there's an error, display it*/
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    fclose(newimage);
}

int main()
{
    //const char* filename = "../images/flag.png";
    decodeOneStep(IMAGE);

    return 0;
}