cmake_minimum_required(VERSION 3.0)
project(cw05)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -std=c99)


set(SOURCE_FILES zad1/interpreter.c zad2/master.c zad2/slave.c)
add_executable(interpreter zad1/interpreter.c)
add_executable(master zad2/master.c)
add_executable(slave zad2/slave.c)

