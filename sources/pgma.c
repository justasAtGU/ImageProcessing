//
// Created by Eric Lei on 6/16/17.
//
#include <stdio.h>
#include <stdlib.h>
#include "../includes/pgma.h"

void encodePGM(const char * filename, unsigned char * image, unsigned int width, unsigned int height)
{
    FILE *pgmimage;
    pgmimage = fopen(filename, "wb");
    fputs("P2\n", pgmimage);
    fprintf(pgmimage, "%d ", width);
    fprintf(pgmimage, "%d\n", height);
    fprintf(pgmimage, "%d\n", findMax(image, width, height));

    for (int i = 0; i < width*height; i++)
    {
        if (i % 17 == 0)
            fprintf(pgmimage, "\n");
        fprintf(pgmimage, "%d ", image[i]);
    }
    fclose(pgmimage);
}

void decodePGM(const char * filename, unsigned char * image, unsigned int width, unsigned int height)
{

}

int findMax(unsigned char* image, unsigned int width, unsigned int height)
{
    int max = 0;
    for (int i = 0; i < width*height; i++)
    {
        int check = image[i];
        if (check > max)
            max = check;
    }
    return max;
}