#include <stdio.h>
#include <stdlib.h>
#include "includes/lodepng.h"
#include "sources/lodepng.c"
#include "includes/preproc.h"
#include "includes/pgma.h"

#define IMAGE "../images/street.png" //preprocessed image (PNG format)
#define OUTIMAGE "../imagesNew/street1.pgm" //postprocessed image (PGM format)

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
    unsigned error;
    unsigned char* image; //will point to first (upper left) pixel after png decode
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, IMAGE);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    /* Perform image processing here********************************************/
    grayscale(image, width, height);
    /****************************************************************************/

    encodePGM(OUTIMAGE, image, width, height); /*save image to new image in /imagesNew directory*/
    free(image);

    return 0;
}