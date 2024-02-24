cmake_minimum_required(VERSION 3.25)

project(plasmatum)

set(CMAKE_CXX_STANDARD 23)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(parser
  ${CMAKE_SOURCE_DIR}/parser.yy
  ${SOURCE_DIR}/parser.gen.cpp
  DEFINES_FILE ${INCLUDE_DIR}/parser.gen.h)

FLEX_TARGET(lexer
  ${CMAKE_SOURCE_DIR}/lexer.ll
  ${SOURCE_DIR}/lexer.gen.cpp)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

file(GLOB_RECURSE sources ${SOURCE_DIR}/*.cpp)
set(sources ${sources} ${FLEX_lexer_OUTPUTS})
set(sources ${sources} ${BISON_parser_OUTPUT_SOURCE})

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(plsm ${sources})
