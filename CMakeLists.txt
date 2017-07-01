cmake_minimum_required(VERSION 3.6)
project(ImageProcessing)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c includes/preproc.h sources/preproc.c sources/pgma.c includes/pgma.h)
add_executable(ImageProcessing ${SOURCE_FILES})