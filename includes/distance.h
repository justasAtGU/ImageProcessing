#ifndef IMAGEPROCESSING_DISTANCE_H
#define IMAGEPROCESSING_DISTANCE_H

#include "vector.h"
#include "struct.h"

#define PI              3.1415926
#define FOCAL_LENGTH    6.08
#define SENSOR_HEIGHT   2.76
#define CONE_SIZE       325
#define IMAGE_HEIGHT	240

void findCone(Line *lines, float angle, int line_count, int height, unsigned char* image);

int findIntersectionPoint(Line line1, Line line2, float slope1, float slope2);

void findDistance(int cone_height);

#endif //IMAGEPROCESSING_DISTANCE_H
