cmake_minimum_required(VERSION 3.0)
project(ZAD4)
set(CMAKE_C_FLAGS "-std=c99 -Wall")

set(CMAKE_CXX_STANDARD 11)

#MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} PATH)
set(ZAD1_DIR ${PARENT_DIR}/zad1/)
set(ZAD2_DIR ${PARENT_DIR}/zad2/)

set(SOURCE_FILES
        ${ZAD1_DIR}/libaddrbook.c
        ${ZAD1_DIR}/libaddrbook.h
        ${ZAD2_DIR}/main.c)

add_executable(MainStatic main.c)
add_executable(MainShared main.c)
#add_executable(MainDynamic ${ZAD2_DIR}/main.c)


add_library(static STATIC ${ZAD1_DIR}/libaddrbook.c)
add_library(shared SHARED ${ZAD1_DIR}/libaddrbook.c)
add_library(dynamic MODULE ${ZAD1_DIR}/libaddrbook.c)

target_link_libraries(MainStatic static)
target_link_libraries(MainShared shared)


add_custom_target(tests
        COMMAND echo STATIC: >>results.txt
        COMMAND MainStatic >> results.txt
        COMMAND echo SHARED: >>results.txt
        COMMAND MainShared >> results.txt

        DEPENDS static
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        )
