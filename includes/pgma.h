//
// Created by Eric Lei on 6/16/17.
//

#ifndef IMAGEPROCESSING_PGMA_H
#define IMAGEPROCESSING_PGMA_H

/**
 * Encodes 8-bit grayscale image into PGM file.
 */
void encodePGM(const char * filename, unsigned char * image, unsigned int width, unsigned int height);

/*
 * Decodes 8-bit PGM image and gets image buffer.
 */
void decodePGM(const char * filename, unsigned char * image, unsigned int width, unsigned int height);

int findMax(unsigned char* image, unsigned int width, unsigned int height);
#endif //IMAGEPROCESSING_PGMA_H
