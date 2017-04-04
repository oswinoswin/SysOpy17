cmake_minimum_required(VERSION 3.6)
project(cw04)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES zad1/printer.c zad2/zad2.c zad3/program3.c zad3/child.c)
add_executable(zad1 zad1/printer.c)
add_executable(zad2 zad2/zad2.c)
add_executable(zad3 zad3/program3.c)
