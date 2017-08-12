//
// Created by Eric Lei on 7/4/17.
//
#include "../includes/canny.h"
#include "../includes/preproc.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned HIGH_THRESH = 200;
unsigned LOW_THRESH = 25;

int isNotMax(int leftNeighbor, int pixel, int rightNeighbor); /*returns 1 if pixel is not max, 0 otherwise*/

int * edgeDirection(int* horizGx, int * vertGy, int width, int height)
{
    int * angles = (int *) malloc(width*height*4);
    int Gx;
    int Gy;
    for (int i = 0; i < width * height; i++)
    {
        Gx = horizGx[i];
        Gy = vertGy[i];
        if (Gx == 0)
        {
            if (Gy == 0)
                angles[i] = 0;
            else if (Gy > 0)
                angles[i] = 90;
            else
                angles[i] = -90;
        }
        else
        {
            angles[i] = (int) (atan2(Gy, Gx) * (180 / PI));
        }
    }
    return angles;
}

int * edgeMagnitude(int* horizGx, int *vertGy, int width, int height)
{
    int * magnitude = (int *) malloc(width*height*4);
    for (int i = 0; i < width * height; i++)
    {
        magnitude[i] = (int) (abs(horizGx[i]) + abs(vertGy[i]));
        //printf("Magnitude: %d\n", magnitude[i]);
    }
    return magnitude;
}

void nonMaxSuppress(unsigned char * image8, int * magnitude, int * direction, int width, int height)
{
    int rowIndex;
    int val;
    for (int row = 1; row < height-1; row++)
    {
        rowIndex = width * row;
        for (int col = 1; col < width-1; col++)
        {
            int currPixel = rowIndex + col;
            //horizontal axis
            if ((direction[currPixel] <= 22 && direction[currPixel] >= -22) || (direction[currPixel] >= 158 ||
                                                                                direction[currPixel] <= -158))
            {
                if (isNotMax(magnitude[currPixel-1], magnitude[currPixel], magnitude[currPixel+1]))
                    image8[currPixel] = 0;
                else
                {
                    val = magnitude[currPixel];
                    if (val < 0)
                        val = 0;
                    if (val > 255)
                        val = 255;
                    image8[currPixel] = val;
                }
            }

            //45, -135 degree axis
            if ((direction[currPixel] <= 67 && direction[currPixel] >= 23) || (direction[currPixel] >= -157 &&
                                                                                direction[currPixel] <= -113))
            {
                if (isNotMax(magnitude[currPixel+width-1], magnitude[currPixel], magnitude[currPixel-width+1]))
                    image8[currPixel] = 0;
                else
                {
                    val = magnitude[currPixel];
                    if (val < 0)
                        val = 0;
                    if (val > 255)
                        val = 255;
                    image8[currPixel] = val;
                }
            }

            //vertical axis
            if ((direction[currPixel] <= 112 && direction[currPixel] >= 68) || (direction[currPixel] >= -112 &&
                                                                               direction[currPixel] <= -68))
            {
                if (isNotMax(magnitude[currPixel+width], magnitude[currPixel], magnitude[currPixel-width]))
                    image8[currPixel] = 0;
                else
                {
                    val = magnitude[currPixel];
                    if (val < 0)
                        val = 0;
                    if (val > 255)
                        val = 255;
                    image8[currPixel] = val;
                }
            }

            //135, -45 degree axis
            if ((direction[currPixel] <= 157 && direction[currPixel] >= 113) || (direction[currPixel] >= -67 &&
                                                                                direction[currPixel] <= -23))
            {
                if (isNotMax(magnitude[currPixel-width-1], magnitude[currPixel], magnitude[currPixel+width+1]))
                    image8[currPixel] = 0;
                else
                {
                    val = magnitude[currPixel];
                    if (val < 0)
                        val = 0;
                    if (val > 255)
                        val = 255;
                    image8[currPixel] = val;
                }
            }
        }
    }
}

int isNotMax(int leftNeighbor, int pixel, int rightNeighbor)
{
    if (leftNeighbor > pixel || rightNeighbor > pixel)
        return 1;
    return 0;
}

void doubleThreshold(unsigned char * image8, int *width, int *height)
{
    int magIndex = *width + 1;
    int imageIndex = 0;
    int val;
    for (int row = 1; row < *height-1; row++)
    {
        for (int col = 1; col < *width-1; col++)
        {
            val = image8[magIndex];
            if (val <= LOW_THRESH)
                val = 0;
            else
                val = 255;
//            if (val >= HIGH_THRESH)
//                val = 255;
            image8[imageIndex] = val;
            magIndex++;
            imageIndex++;
        }
        magIndex += 2;
    }
    *width = *width - 2;
    *height = *height - 2;
}

void hysteresis(unsigned char * image8, int *width, int *height)
{
    int * magnitude = (int*) malloc((*width)*(*height)*4);
    int val;
    //Sets high threshold to 255
    for (int i = 0; i < (*width)*(*height); i++)
    {
        if (image8[i] >= 0)
            magnitude[i] = 255;
        else
            magnitude[i] = 0;
    }

    int magIndex = *width + 1;
    int imageIndex = 0;

    for (int row = 1; row < *height-1; row++)
    {
        for (int col = 1; col < *width-1; col++)
        {
            val = image8[magIndex];
            if (val < HIGH_THRESH && val != 0)
            {
                if (magnitude[magIndex-1] == 255 || magnitude[magIndex-(*width)-1] == 255 || magnitude[magIndex-(*width)] == 255
                        || magnitude[magIndex-(*width)+1] == 255 || magnitude[magIndex+1] == 255 || magnitude[magIndex+(*width)+1] == 255
                        || magnitude[magIndex+(*width)] == 255 || magnitude[magIndex+(*width)-1] == 255)
                {
                    val = 255;
                }
                else
                    val = 0;
            }
            image8[imageIndex] = val;
            magIndex++;
            imageIndex++;
        }
        magIndex += 2;
    }
    *width = *width - 2;
    *height = *height - 2;
}

unsigned char averagePixel(unsigned char * image8, unsigned width, unsigned height)
{
    unsigned sum = 0;
    for (int i = 0; i < width*height; i++)
    {
        sum += image8[i];
        //printf("%d\n", sum);
    }
    sum /= (width*height);
    return (unsigned char) sum;
}

void detectEdgeCanny(unsigned char * image8, unsigned char * tempBuf, int *width, int *height)
{
    unsigned char avg = averagePixel(image8, *width, *height);
    avg = 2*avg/4;
    printf("%d\n", avg);
    LOW_THRESH = 84 * avg / 64;
    HIGH_THRESH = 42 * avg / 64;
    gaussBlur5x5(image8, tempBuf, width, height); //Apply Gaussian smoothing
    int * Gx = sobelHoriz(image8, width, height); //x-direction gradient from Sobel filter
    int * Gy = sobelVert(image8, width, height); //y-direction gradient from Sobel filter
    *width = *width -2;
    *height = *height -2;
    int * direction = edgeDirection(Gx, Gy, *width, *height); //buffer stores direction of gradient
    int * magnitude = edgeMagnitude(Gx, Gy, *width, *height); //buffer stores magnitude of gradient
    nonMaxSuppress(image8, magnitude, direction, *width, *height); //Apply non-maximum suppression
    doubleThreshold(image8, width, height); //Apply double thresholding

    free(Gx);
    free(Gy);
    free(direction);
    free(magnitude);
}