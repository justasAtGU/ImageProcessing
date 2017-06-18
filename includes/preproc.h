//
// Created by Eric Lei on 6/10/17.
//

#ifndef IMAGEPROCESSING_PREPROC_H
#define IMAGEPROCESSING_PREPROC_H


/**
 * Converts 32 bit RGBA image to 8-bit grayscale image.
 */
void grayscale(unsigned char *image, unsigned int width, unsigned int height);

/**
 * Prints pixel values to console.
 */
void printPixelsBW(unsigned char * image, unsigned int *width, unsigned int *height);

/*
 * Rotates image 90 degrees clockwise.
 */
void rotate(unsigned char * image, unsigned int width, unsigned int height);

void decimate();

void avgBlur();

void gaussBlur();

void sharpen();

void erode();

void dilate();


#endif //IMAGEPROCESSING_PREPROC_H
