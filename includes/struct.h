#ifndef IMAGEPROCESSING_STRUCT_H
#define IMAGEPROCESSING_STRUCT_H

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

#endif //IMAGEPROCESSING_STRUCT_H
