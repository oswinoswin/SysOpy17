cmake_minimum_required(VERSION 3.0)
project(cw01)
set(CMAKE_C_FLAGS "-std=c99")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        zad1/libaddrbook.c
        zad1/libaddrbook.h
        zad2/main.c)

add_executable(Main zad2/main.c)

add_library(myLib STATIC zad1/libaddrbook.c)

target_link_libraries(Main myLib)



