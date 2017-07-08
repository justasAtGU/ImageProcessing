//
// Created by Eric Lei on 6/10/17.
// Look in preproc.h for function descriptions.

#include "../includes/preproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char minimum(unsigned char v1, unsigned char v2, unsigned char v3); /*finds val of min of 3 vals*/
unsigned char maximum(unsigned char v1, unsigned char v2, unsigned char v3); /*finds val of max of 3 vals*/

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
        erodeImage += 2;
        temp += 2;
    }
    memcpy(image8, tempBuf, width*height);
}

void dilate(unsigned char * image8, unsigned char * tempBuf, unsigned int width, unsigned int height)
{
    unsigned char * erodeImage = image8 + width + 1;
    unsigned char * temp = tempBuf + width + 1;
    unsigned char v1, v2, v3;
    for (int row = 1; row < height - 1; row++)
    {
        v1 = maximum(*(erodeImage - width - 1), *(erodeImage - 1), *(erodeImage + width - 1));
        v2 = maximum(*(erodeImage - width), *(erodeImage), *(erodeImage + width));
        v3 = maximum(*(erodeImage - width + 1), *(erodeImage + 1), *(erodeImage + width + 1));
        for (int col = 1; col < width - 1; col++)
        {
            *temp = maximum(v1, v2, v3);
            temp++;
            erodeImage++;
            v1 = v2;
            v2 = v3;
            v3 = maximum(*(erodeImage - width + 1), *(erodeImage + 1), *(erodeImage + width + 1));
        }
        erodeImage += 2;
        temp += 2;
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

unsigned char maximum(unsigned char v1, unsigned char v2, unsigned char v3)
{
    unsigned char max = v1;
    if (v2 > max)
        max = v2;
    if (v3 > max)
        max = v3;
    return max;
}

void gaussBlur5x5(unsigned char * image8, unsigned char *tempBuf, unsigned int *width, unsigned int *height)
{
    unsigned int * tempbuf = (unsigned int *) tempBuf;
    int arrayRow;
    int val;

    //vertical convolution
    for (int row = 2; row < (*height) - 2; row++)
    {
        arrayRow = row * (*width);
        for (int col = 0; col < (*width); col++)
        {
            val = image8[arrayRow - 2*(*width) + col] + 4*image8[arrayRow-(*width) + col] + 6*image8[arrayRow + col] + 4*image8[arrayRow+(*width)+col]
                    + image8[arrayRow + 2*(*width)+col];
            //printf("%d\n", val);
            tempbuf[arrayRow+col] = val;
        }
    }

    //horizontal convolution, normalizes pixel value
    for (int row = 0; row < (*height); row++)
    {
        arrayRow = row * (*width);
        for (int col = 2; col < (*width) - 2; col++)
        {
            val = tempbuf[arrayRow-2+col] + 4*tempbuf[arrayRow-1+col] + 6*tempbuf[arrayRow+col] +
                    4*tempbuf[arrayRow+1+col] + tempbuf[arrayRow + 2 + col];
            val = val/256;
            //printf("%d\n", val);
            if (val < 0)
                val = 0;
            if (val > 255)
                val = 255;
            image8[arrayRow + col] = val;
        }
    }

    //Removes the border.
    arrayRow = 0;
    for (int row = 0; row < (*height) - 4; row++)
    {
        for (int col = 0; col < (*width) - 4; col++)
        {
            image8[arrayRow] = image8[(row+2)*(*width) + 2 + col];
            arrayRow++;
        }
    }
    *width = *width - 4;
    *height = *height - 4;
}

/*
 * Needs to freed if called.
 * Re-implement by using matrix separability.
 */
int * sobelHoriz(unsigned char * image8, unsigned * width, unsigned * height)
{
    int * Gx = (int *) malloc((*width- 2) * (*height-2) * 4); //x direction gradient to be returned
    int rowIndex;
    int Gxindex = 0;
    int val;
    for (int row = 1; row < (*height) - 1; row++)
    {
        rowIndex = row * (*width);
        for (int col = 1; col < (*width)-1; col++)
        {
            val = image8[rowIndex + col - (*width) + 1] + 2*image8[rowIndex+col+1] + image8[rowIndex + col + (*width) + 1]
                    - image8[rowIndex + col - (*width) - 1] - 2*image8[rowIndex+col-1] - image8[rowIndex+col+(*width)-1];
            Gx[Gxindex] = val;
            //printf("%d\n", val);
            Gxindex++;
        }
    }
    return Gx;
}

/*
 * Needs to freed if called.
 * Re-implement through matrix separability.
 */
int * sobelVert(unsigned char * image8, unsigned * width, unsigned * height)
{
    int * Gy = (int *) malloc((*width- 2) * (*height-2) * 4); //x direction gradient to be returned
    int rowIndex;
    int Gyindex = 0;
    int val;
    for (int row = 1; row < (*height) - 1; row++)
    {
        rowIndex = row * (*width);
        for (int col = 1; col < (*width)-1; col++)
        {
            val = image8[rowIndex+col-(*width)-1] + 2*image8[rowIndex+col-(*width)]+image8[rowIndex+col-(*width)+1]
                    -image8[rowIndex+col+(*width)-1] - 2*image8[rowIndex+col+(*width)] - image8[rowIndex+col+(*width)+1];
            Gy[Gyindex] = val;

            //printf("%d\n", val);
            Gyindex++;
        }
    }
    return Gy;
}