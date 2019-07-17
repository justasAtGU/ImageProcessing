#ifndef IMAGEPROCESSING_HOUGH_H
#define IMAGEPROCESSING_HOUGH_H

#define PI 3.1415926

typedef struct {
   int x;
   int y;
} Point;

typedef struct {
   Point p1;
   Point p2;
} Line;

typedef struct
{
	float data;
	size_t limit;
	size_t current;
} Trig;

void houghTransform(unsigned char *image, int width, int height, int thresh, int lineLength,
 int lineGap, int linesMax, float rho, float theta, int edge_thresh, int print);

void drawLines(unsigned char *image, Line *lines, int width, int height);

#endif //IMAGEPROCESSING_HOUGH_H
