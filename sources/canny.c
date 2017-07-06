//
// Created by Eric Lei on 7/4/17.
//
#include "../includes/canny.h"
#include "../includes/preproc.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.1415926

int * edgeDirection(int* horizGx, int * vertGy, int width, int height)
{
    int * angles = (int *) malloc(width*height*4);
    for (int i = 0; i < width * height; i++)
    {
        angles[i] = (int) (atan2(vertGy[i], horizGx[i]) * (180/PI));
        printf("%d: %d\n", i, angles[i]);
    }
    return angles;
}



//Need to reduce width and height by 2 after calling sobelHoriz and sobelVert