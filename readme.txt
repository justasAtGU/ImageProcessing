To compile and run via terminal (uses CMake for Makefile generation):
1. Enter project directory.
2. rm -r cmake-build-debug
3. cd cmake-build-debug
4. cmake ..
5. make clean
6. make
7. ./ImageProcessing

/images contains on the original images to perform image processing on. The resultant images will
be outputted into /imagesNew. Refer to the macro definitions of IMAGE and OUTIMAGE in main.c

Uses the lodepng library for decoding and encoding PNG images. Also has the ability to encode into PGM formatted images.
