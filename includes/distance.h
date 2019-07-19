#ifndef IMAGEPROCESSING_DISTANCE_H
#define IMAGEPROCESSING_DISTANCE_H

#include "vector.h"
#include "struct.h"

#define PI 3.1415926

void angleBetweenLines(Line *lines, int base_index, int line_count);

float distanceToCone(int real_size, Line *lines, int sensor_size, int focal_length, int heigth);


#endif //IMAGEPROCESSING_DISTANCE_H
