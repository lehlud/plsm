cmake_minimum_required(VERSION 3.25)

project(libplsm C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.c)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(plsm SHARED ${sources})
target_link_libraries(plsm gc)
