//
// Created by Eric Lei on 6/10/17.
//

#ifndef IMAGEPROCESSING_PREPROC_H
#define IMAGEPROCESSING_PREPROC_H


/**
 * Converts 32 bit RGBA image to 8-bit grayscale image.
 */
void grayscale(unsigned char *image32, unsigned char *image8, unsigned int width, unsigned int height);

/*
 * image32 will contain grayscale data. Can call lodepng encode function. Sets alpha value to 255.
 */
void prepBWOutPNG(unsigned char *image32, unsigned char *image8, unsigned int width, unsigned int height);

/**
 * Prints pixel values to console.
 */
void printPixelsBW(unsigned char * image, unsigned int *width, unsigned int *height);

/*
 * Rotates image 90 degrees clockwise.
 */
void rotate(unsigned char * image, unsigned int width, unsigned int height);

void decimate();

void avgBlur(unsigned char * image, unsigned char* tempBuf, unsigned int width, unsigned int height);

void gaussBlur();

void sharpen(unsigned char * image, unsigned char* tempBuf, unsigned int width, unsigned int height);

void erode();

void dilate();


#endif //IMAGEPROCESSING_PREPROC_H
