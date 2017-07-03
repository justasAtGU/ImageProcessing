//
// Created by Eric Lei on 6/10/17.
// Look in preproc.h for function descriptions.

#include "../includes/preproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char minimum(unsigned char v1, unsigned char v2, unsigned char v3); /*finds val of max of 3 vals*/

void grayscale(unsigned char *image32, unsigned char *image8, unsigned int width, unsigned int height)
{
    unsigned int grayIndex = 0;
    for (int i = 0; i < width * height * 4; i+=4)
    {
        unsigned char avgVal = (306 * image32[i] + 601 * image32[i+1] + 117 * image32[i+2])/1024;
        image8[grayIndex] = avgVal;
        grayIndex++;
    }
}

void decimate(unsigned char * image8, unsigned int * width, unsigned int * height)
{
    if (*height < 2 || *width < 2)
    {
        printf("Height or width too small for decimation.\n");
        return;
    }
    unsigned char * decimatedImage = image8;
    unsigned index = 0;
    for (int row = 0; row < *height-1; row+=2)
    {
        index = row * (*width);
        for (int col = 0; col < *width-1; col += 2)
        {
            *decimatedImage = image8[index];
            decimatedImage++;
            index += 2;
        }
    }
    *width = (*width) / 2;
    *height = (*height) / 2;
}

void prepBWOutPNG(unsigned char *image32, unsigned char *image8, unsigned int width, unsigned int height)
{
    unsigned int grayIndex = 0;
    for (int i = 0; i < width * height * 4; i+=4)
    {
        image32[i] = image8[grayIndex];
        image32[i+1] = image8[grayIndex];
        image32[i+2] = image8[grayIndex];
        image32[i+3] = 255;
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

void avgBlur(unsigned char * image, unsigned char* tempBuf, unsigned int width, unsigned int height)
{
    int pixelInd; //points to first element of each row per iteration
    int val; //Stores the calculated pixel value
    for (int row = 1; row < height - 1; row++)
    {
        pixelInd = width * row;
        for (int col = 1; col < width - 1; col++)
        {
            val = image[pixelInd+col] + image[pixelInd+col+1] + image[pixelInd+col-1] + image[pixelInd+col-width]
                    + image[pixelInd+col-width-1] + image[pixelInd+col-width+1] + image[pixelInd+col+width]
                    + image[pixelInd+col+width-1] + image[pixelInd+col+width+1];
            val = val/9;
            if (val < 0)
                val = 0;
            if (val > 255)
                val = 255;
            tempBuf[pixelInd+col] = val;
        }
    }
    memcpy(image, tempBuf, width*height);
}

void sharpen(unsigned char * image, unsigned char* tempBuf, unsigned int width, unsigned int height)
{
    int pixelInd;
    int val;
    for (int row = 1; row < height - 1; row++)
    {
        pixelInd = width * row;
        for (int col = 1; col < width - 1; col++)
        {
//            val = 9*image[pixelInd+col] - image[pixelInd+col+1] - image[pixelInd+col-1] - image[pixelInd+col-width]
//                  - image[pixelInd+col-width-1] - image[pixelInd+col-width+1] - image[pixelInd+col+width]
//                  - image[pixelInd+col+width-1] - image[pixelInd+col+width+1];
            val = 5*image[pixelInd+col] - image[pixelInd+col-1] - image[pixelInd+col+1]
            -image[pixelInd+col-width] - image[pixelInd+col+width];
            if (val < 0)
                val = 0;
            if (val > 255)
                val = 255;
            tempBuf[pixelInd+col] = val;
        }
    }
    memcpy(image, tempBuf, width*height);
}

//has weird line going down diagonally, need to fix
void erode(unsigned char * image8, unsigned char * tempBuf, unsigned int width, unsigned int height)
{
    unsigned char * erodeImage = image8 + width + 1;
    unsigned char * temp = tempBuf + width + 1;
    unsigned char v1, v2, v3;
    for (int row = 1; row < height - 1; row++)
    {
        v1 = minimum(*(erodeImage - width - 1), *(erodeImage - 1), *(erodeImage + width - 1));
        v2 = minimum(*(erodeImage - width), *(erodeImage), *(erodeImage + width));
        v3 = minimum(*(erodeImage - width + 1), *(erodeImage + 1), *(erodeImage + width + 1));
        for (int col = 1; col < width - 1; col++)
        {
            *temp = minimum(v1, v2, v3);
            temp++;
            erodeImage++;
            v1 = v2;
            v2 = v3;
            v3 = minimum(*(erodeImage - width + 1), *(erodeImage + 1), *(erodeImage + width + 1));
        }
        erodeImage += 3;
        temp += 3;
    }
    memcpy(image8, tempBuf, width*height);
}

unsigned char minimum(unsigned char v1, unsigned char v2, unsigned char v3)
{
    unsigned char min = v1;
    if (v2 < min)
        min = v2;
    if (v3 < min)
        min = v3;
    return min;
}
