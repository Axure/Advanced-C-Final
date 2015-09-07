cmake_minimum_required(VERSION 3.2)
project(Final_Works)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -DDEBUG")

set(LINKED_LIST src/main/c/lib/linked_list.c src/main/c/lib/linked_list.h)

set(SOURCE_FILES
        src/main/c/lib/linked_list.h
        src/test/c/lib/test_linked_list.c)

include_directories(src/main/c/lib)


add_executable(Final_Works ${SOURCE_FILES})

add_executable(Linked_List_Test src/test/c/lib/test_linked_list.c ${LINKED_LIST})