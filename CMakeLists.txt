cmake_minimum_required(VERSION 3.6)
project(ImageProcessing)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c includes/gaussianBlur.h sources/gaussianBlur.c includes/preproc.h sources/preproc.c)
add_executable(ImageProcessing ${SOURCE_FILES})