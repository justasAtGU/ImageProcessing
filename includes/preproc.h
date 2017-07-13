//
// Created by Eric Lei on 6/10/17.
//

#ifndef IMAGEPROCESSING_PREPROC_H
#define IMAGEPROCESSING_PREPROC_H

/*
 * Various image processing algorithms.
 */

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

/*
 * Performs gaussian filter with 5x5 gaussian kernel.
 * (1/256) * [1 4 6 4 1; 4 16 24 16 4; 6 24 36 24 6; 4 16 24 16 4; 1 4 6 4 1]
 * Uses separability of Gaussian kernel.
 * Removes the 2 pixel width border surrounding the resultant image.
 */
void gaussBlur5x5(unsigned char * image, unsigned char* tempBuf, unsigned int *width, unsigned int *height);

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

/*
 * Finds gradients by convolving image with horizontal Sobel 3x3 kernel.
 * Returns pointer to array of values that contain the x-direction gradient.
 * Needs to be freed.
 */
int * sobelHoriz(unsigned char * image8, unsigned * width, unsigned * height);


int * sobelVert(unsigned char * image8, unsigned * width, unsigned * height);



void avgPool();




#endif //IMAGEPROCESSING_PREPROC_H
