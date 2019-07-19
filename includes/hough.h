#ifndef IMAGEPROCESSING_HOUGH_H
#define IMAGEPROCESSING_HOUGH_H

#define PI 3.1415926

#include "./struct.h"

void houghTransform(unsigned char *image, int width, int height, int thresh, int lineLength,
 int lineGap, int linesMax, float rho, float theta, int edge_thresh, int print);

void drawLines(unsigned char* image, Line *lines, int width, int height);

#endif //IMAGEPROCESSING_HOUGH_H
