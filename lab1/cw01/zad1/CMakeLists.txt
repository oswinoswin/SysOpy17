cmake_minimum_required(VERSION 3.0)
project(zad1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c libaddrbook.c)
add_executable(zad1 ${SOURCE_FILES})
add_library(libaddrbook STATIC libaddrbook.c)