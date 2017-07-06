//
// Created by Eric Lei on 7/4/17.
//

#ifndef IMAGEPROCESSING_CANNY_H
#define IMAGEPROCESSING_CANNY_H

/*
 * Returns direction of gradient.
 * Must be freed.
 */
int * edgeDirection(int* horizGx, int * vertGy, int width, int height);
#endif //IMAGEPROCESSING_CANNY_H
