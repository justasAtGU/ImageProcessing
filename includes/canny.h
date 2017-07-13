//
// Created by Eric Lei on 7/4/17.
//
/*
 * Implements canny edge detection. Requires Sobel filtering which is implemented in preproc.c
 */

#ifndef IMAGEPROCESSING_CANNY_H
#define IMAGEPROCESSING_CANNY_H


#define PI 3.1415926

/*
 * Returns direction of gradient.
 * Must be freed.
 */
int * edgeDirection(int* horizGx, int * vertGy, int width, int height);

/*
 * Returns magnitude of gradient.
 * Must be freed.
 */
int * edgeMagnitude(int* horizGx, int *vertGy, int width, int height);

/*
 * Suppresses edges that are not maxes.
 * Frees magnitude and direction buffers.
 * Sobel horiz and vert buffers still need to be freed after this is called.
 */
void nonMaxSuppress(unsigned char * image8, int * magnitude, int * direction, int width, int height);

/*
 * Filters out non-edges (below threshold).
 * Reduces height and width by 2, removes border.
 */
void doubleThreshold(unsigned char * image8, int *width, int *height);

void hysteresis(unsigned char * image8, int *width, int *height);

/*
 * Returns average of edge values after NMS.
 */
unsigned char averagePixel(unsigned char * image8, unsigned width, unsigned height);

/*
 * Uses Canny's algorithm for edge detection.
 */
void detectEdgeCanny(unsigned char * image8, unsigned char * tempBuf, int *width, int *height);

#endif //IMAGEPROCESSING_CANNY_H
