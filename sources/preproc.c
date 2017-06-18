//
// Created by Eric Lei on 6/10/17.
// Look in preproc.h for function descriptions.

#include "../includes/preproc.h"
#include <stdio.h>

void grayscale(unsigned char *image, unsigned int width, unsigned int height)
{
    unsigned char* grayPix = image;
    unsigned int grayIndex = 0;
    for (int i = 0; i < width * height * 4; i+=4)
    {
        unsigned char avgVal = (306 * image[i] + 601 * image[i+1] + 117 * image[i+2])/1024;
        grayPix[grayIndex] = avgVal;
        grayIndex++;
    }
}

void printPixelsBW(unsigned char * image, unsigned int *width, unsigned int *height)
{
    for (int row = 0; row < *height; row++)
    {
        for (int col = 0; col < *width; col++)
        {
            printf("%d ", image[col]);
        }
        printf("\n");
    }
}

void rotate(unsigned char * image, unsigned int width, unsigned int height)
{
    for (int i = 0; i < width * height; i++)
    {

    }
}
