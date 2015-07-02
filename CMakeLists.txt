cmake_minimum_required(VERSION 3.2)
project(Final_Works)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/lib/linked_list.h
    src/lib/test_linked_list.c)

include_directories(src/lib)

add_executable(Final_Works ${SOURCE_FILES})