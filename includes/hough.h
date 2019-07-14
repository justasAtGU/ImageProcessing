//
// Created by Eric Lei on 7/27/17.
//

#ifndef IMAGEPROCESSING_HOUGH_H
#define IMAGEPROCESSING_HOUGH_H

#define PI 3.1415926
 
typedef struct {
   int x;
   int y;
} Point;

typedef struct
{
	float data;
	size_t limit;
	size_t current;
} Vector;

void houghTransform(unsigned char * image, int width, int height, int thresh, int lineLength,
 int lineGap, int linesMax, float rho, float theta, Vector lines[4]);

#endif //IMAGEPROCESSING_HOUGH_H
