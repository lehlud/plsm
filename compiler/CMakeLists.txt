cmake_minimum_required(VERSION 3.25)

project(plasmatum)

set(CMAKE_CXX_STANDARD 23)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

set(GEN_DIR ${CMAKE_SOURCE_DIR}/generated)

set(ANTLR_INC ${INC_DIR}/plsmLexer.h ${INC_DIR}/plsmParser.h ${INC_DIR}/plsmVisitor.h ${INC_DIR}/plsmBaseVisitor.h)
set(ANTLR_SRC ${SRC_DIR}/plsmLexer.cpp ${SRC_DIR}/plsmParser.cpp ${SRC_DIR}/plsmVisitor.cpp ${SRC_DIR}/plsmBaseVisitor.cpp)

add_custom_command(PRE_BUILD
  DEPENDS ${CMAKE_SOURCE_DIR}/plsm.g4
  OUTPUT ${ANTLR_SRC}
  OUTPUT ${ANTLR_INC}
  COMMENT "Generating plsm_parser"
  COMMAND java -jar
    ${CMAKE_SOURCE_DIR}/thirdparty/antlr-4.13.1-complete.jar
    ${CMAKE_SOURCE_DIR}/plsm.g4
    -o ${GEN_DIR} -Dlanguage=Cpp -no-listener -visitor
  COMMAND ${CMAKE_COMMAND} -E copy ${GEN_DIR}/*.h ${INC_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${GEN_DIR}/*.cpp ${SRC_DIR})

file(GLOB_RECURSE sources ${SRC_DIR}/*.cpp)
set(sources ${sources} ${ANTLR_SRC})

add_executable(plsm ${sources})

target_include_directories(plsm PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(plsm PRIVATE /usr/include/antlr4-runtime)
target_link_libraries(plsm antlr4-runtime)

add_custom_target(clean-all
  COMMAND ${CMAKE_COMMAND} -E rm -rf
    ${GEN_DIR} ${CMAKE_SOURCE_DIR}/build
)

