cmake_minimum_required (VERSION 2.6)
include_directories ("${PROJECT_SOURCE_DIR}/../zad1")
add_subdirectory (${PROJECT_SOURCE_DIR}/../zad1)
set(CMAKE_C_FLAGS "-std=c99")
project (zad2)
add_executable(Main main.c)
add_library(libaddrbook main.c)
target_link_libraries (Main "${PROJECT_SOURCE_DIR}/../zad1")