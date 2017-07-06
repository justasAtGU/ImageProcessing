To generate Makefile (uses CMake):
1. Enter project directory.
2. rm -r cmake-build-debug
3. cd cmake-build-debug
4. cmake ..

To compile and run:
1. Enter cmake-build-debug.
2. make clean
3. make
4. ./ImageProcessing

/images contains on the original images to perform image processing on. The resultant images will
be outputted into /imagesNew. Refer to the macro definitions of IMAGE and OUTIMAGE in main.c

Uses the lodepng library for decoding and encoding PNG images. Also has the ability to encode into PGM formatted images.
