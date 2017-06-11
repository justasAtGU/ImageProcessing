//
// Created by Eric Lei on 6/10/17.
//

#ifndef IMAGEPROCESSING_PREPROC_H
#define IMAGEPROCESSING_PREPROC_H

/**
 * Converts 8-bit image to 32-bit image for writing to png file in main.
 * @param image
 * @param width
 * @param height
 */
void prepDisplay(unsigned char *image, unsigned int width, unsigned int height);

/**
 * Converts 32 bit RGBA image to 8-bit grayscale image.
 *
 * @param image a pointer to first pixel of 32 bit image, changes to first pixel of 8 bit grayscale
 * @param width int width of image
 * @param height int height of image
 */
void grayscale(unsigned char *image, unsigned int width, unsigned int height);


#endif //IMAGEPROCESSING_PREPROC_H
