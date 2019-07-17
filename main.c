#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "includes/lodepng.h"
#include "sources/lodepng.c"
#include "includes/preproc.h"
#include "includes/pgma.h"
#include "includes/canny.h"
#include "includes/hough.h"

char IMAGE[50]; //preprocessed image. Takes in first program argument.
char OUTIMAGE[50]; //postprocessed image (same as preprocessed but in imagesNew folder)

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
 * Processes name of image.
 * Later should add ability to just perform specified functions.
 */
void processArgs(int argc, char *argv[])
{
    sprintf(IMAGE, "../images/%s", argv[1]);
//    if (argv[2][0] == "p" && argv[2][1] == "g" && argv[2][2] == "m")
//    {
//        char * temp;
//        strncpy(temp, argv[1], strlen(argv[1]) - 3);
//        printf("%s", temp);
//        sprintf(OUTIMAGE, "../imagesNew/%s.pgm", temp);
//    }
    sprintf(OUTIMAGE, "../imagesNew/%s", argv[1]);
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

int main(int argc, char *argv[])
{
    processArgs(argc, argv); //parses image name
    unsigned error;
    unsigned char* image32; //will point to first (upper left) pixel after png decode
    unsigned width, height;

    error = lodepng_decode32_file(&image32, &width, &height, IMAGE); //image32 is malloc'd
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    unsigned char* image8 = (unsigned char*) malloc(width*height); //used for grayscale image
    unsigned char* tempBuf = (unsigned char*) malloc(width*height*4); //used to store values in

    printf("width %d, height%d\n", width, height);

    clock_t start = clock(), diff;

    /* Perform image processing here********************************************/
    grayscale(image32, image8, width, height);
    //decimate(image8, &width, &height);
    detectEdgeCanny(image8, tempBuf, &width, &height);
    houghTransform(image8, width, height, 1, 10, 1, 10, 1, PI/180);
    //dilate(image8, tempBuf, width, height);
    //erode(image8, tempBuf, width, height);
    /****************************************************************************/
    diff = clock() - start;
    int msec = diff * 1000/ CLOCKS_PER_SEC;
    printf("Time taken: %d seconds %d milliseconds\n", msec/1000, msec%1000);

    outputImage(OUTIMAGE, image8, image32, width, height, 0);
    free(image8);
    free(image32);
    free(tempBuf);
    return 0;
}

