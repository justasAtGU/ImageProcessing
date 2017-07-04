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

/*
 * Halves height and width. If height or width is odd, will lose the last row or column.
 */
void decimate(unsigned char * image8, unsigned int * width, unsigned int * height);

/*
 * Uses a 3x3 kernel to blur image and averages the 9 pixel values.
 */
void avgBlur(unsigned char * image, unsigned char* tempBuf, unsigned int width, unsigned int height);

void gaussBlur();

/*
 * Sharpens the image.
 */
void sharpen(unsigned char * image, unsigned char* tempBuf, unsigned int width, unsigned int height);

/*
 * Morphology erosion.
 */
void erode(unsigned char * image8, unsigned char * tempBuf, unsigned int width, unsigned int height);

/*
 * Morphology dilation.
 */
void dilate(unsigned char * image8, unsigned char * tempBuf, unsigned int width, unsigned int height);

void dithering();


#endif //IMAGEPROCESSING_PREPROC_H
