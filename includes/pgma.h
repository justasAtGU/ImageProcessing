//
// Created by Eric Lei on 6/16/17.
//

#ifndef IMAGEPROCESSING_PGMA_H
#define IMAGEPROCESSING_PGMA_H

/**
 * Encodes 8-bit grayscale image into PGM file.
 * @param filename
 * @param image
 * @param width
 * @param height
 */
void encodePGM(const char * filename, unsigned char * image, unsigned int width, unsigned int height);
int findMax(unsigned char* image, unsigned int width, unsigned int height);
#endif //IMAGEPROCESSING_PGMA_H
