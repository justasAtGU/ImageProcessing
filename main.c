#include <stdio.h>
#include <stdlib.h>
#include "includes/lodepng.h"
#include "sources/lodepng.c"
#include "includes/preproc.h"
#include "includes/pgma.h"
#include "includes/canny.h"

#define IMAGE "../images/spellingbee.png" //preprocessed image
#define OUTIMAGE "../imagesNew/spellingbee.png" //postprocessed image

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

/*
 * Outputs image in either PNG or PGM format in imagesNew folder. Color int is 0 if want to output image8 BW to
 * 32 bit PNG image. Color int is 1 if image8 is not used (image32 will be outputted to 32 PNG image). Does not matter if
 * output image is PGM format.
 */
void outputImage(const char* filename, const unsigned char* image8, const unsigned char* image32, unsigned width, unsigned height, int color)
{
    int length = strlen(filename);
    if (filename[length-3] == 'p' || filename[length-3] == 'P')
    {
        if ((filename[length-2] == 'g' || filename[length-2] == 'G') &&
                (filename[length-1] == 'm' || filename[length-1] == 'M'))
        {
            encodePGM(filename, image8, width, height);
        }
        else if ((filename[length-2] == 'n' || filename[length-2] == 'N') &&
                 (filename[length-1] == 'g' || filename[length-1] == 'G'))
        {
            if (color == 0)
            {
                prepBWOutPNG(image32, image8, width,height);
                encodeOneStep(filename, image32, width,height);
            }
            else
                encodeOneStep(filename, image32, width,height);
        }
    }
    else
    {
        printf("Output image name not valid.\n");
    }
}

int main()
{
    unsigned error;
    unsigned char* image32; //will point to first (upper left) pixel after png decode
    unsigned width, height;

    error = lodepng_decode32_file(&image32, &width, &height, IMAGE); //image32 is malloc'd
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    unsigned char* image8 = (unsigned char*) malloc(width*height); //used for grayscale image
    unsigned char* tempBuf = (unsigned char*) malloc(width*height*4); //used to store values in

    /* Perform image processing here********************************************/
    grayscale(image32, image8, width, height);

    detectEdgeCanny(image8, tempBuf, &width, &height);
    //dilate(image8, tempBuf, width, height);
    //erode(image8, tempBuf, width, height);
    /****************************************************************************/

    outputImage(OUTIMAGE, image8, image32, width, height, 0);
    free(image8);
    free(image32);
    free(tempBuf);
    return 0;
}

