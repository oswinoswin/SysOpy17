cmake_minimum_required(VERSION 3.0)
project(cw02)

include_directories("/usr/include")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "-std=c99")

set(SOURCE_FILES
        zad1/program.c
        zad2/zad2_no_nftw.c)


add_executable(program zad1/program.c)
add_executable(zad2 zad2/zad2_no_nftw.c)


add_custom_target( run_program
        COMMAND ./program lib generate file.txt 100 100
        )
