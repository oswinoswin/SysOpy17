cmake_minimum_required(VERSION 3.0)
project(cw05)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES zad1/interpreter.c zad2/mandelbrot.c)
add_executable(interpreter zad1/interpreter.c)
add_executable(mandelbrot zad2/mandelbrot.c)