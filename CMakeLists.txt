cmake_minimum_required(VERSION 3.4)
project(ImageProcessing)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c includes/preproc.h sources/preproc.c sources/pgma.c includes/pgma.h sources/canny.c includes/canny.h includes/hough.h sources/hough.c)
add_executable(ImageProcessing ${SOURCE_FILES})