#ifndef IMAGEPROCESSING_DISTANCE_H
#define IMAGEPROCESSING_DISTANCE_H

#include "vector.h"
#include "struct.h"

#define PI              3.1415926
#define FOCAL_LENGTH    3.04 
#define SENSOR_HEIGHT   2.760

void angleBetweenLines(Line *lines, int base_index, int line_count, int height);

#endif //IMAGEPROCESSING_DISTANCE_H
