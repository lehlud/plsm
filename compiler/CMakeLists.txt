cmake_minimum_required(VERSION 3.22.1)

project(plasmatum)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

set(GEN_DIR ${CMAKE_SOURCE_DIR}/generated)

file(GLOB_RECURSE sources ${SRC_DIR}/*.cpp ${SRC_DIR}/**/*.cpp)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)

find_package( 
 Boost 1.83 REQUIRED 
 COMPONENTS json 
)

# Generate ANTLR4 Files
set(ANTLR_INC ${INC_DIR}/plsmLexer.h ${INC_DIR}/plsmParser.h ${INC_DIR}/plsmVisitor.h ${INC_DIR}/plsmBaseVisitor.h)
set(ANTLR_SRC ${SRC_DIR}/plsmLexer.cpp ${SRC_DIR}/plsmParser.cpp ${SRC_DIR}/plsmVisitor.cpp ${SRC_DIR}/plsmBaseVisitor.cpp)
add_custom_command(PRE_BUILD
  DEPENDS ${CMAKE_SOURCE_DIR}/plsm.g4
  OUTPUT ${ANTLR_SRC}
  OUTPUT ${ANTLR_INC}
  COMMENT "Generating plsm_parser"
  COMMAND java -jar
    ${CMAKE_SOURCE_DIR}/thirdparty/antlr4-4.13.2-complete.jar
    ${CMAKE_SOURCE_DIR}/plsm.g4
    -o ${GEN_DIR} -Dlanguage=Cpp -no-listener -visitor
  COMMAND ${CMAKE_COMMAND} -E copy ${GEN_DIR}/*.h ${INC_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${GEN_DIR}/*.cpp ${SRC_DIR})

set(sources ${sources} ${ANTLR_SRC})

add_executable(plsm ${sources})
add_dependencies(plsm antlr4_static)

target_include_directories(plsm PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(plsm PRIVATE ${ANTLR4_INCLUDE_DIRS})
target_link_directories(plsm PRIVATE ${ANTLR4_OUTPUT_DIR})
target_link_libraries(plsm PRIVATE antlr4-runtime Boost::json)

add_custom_target(clean-all
  COMMAND ${CMAKE_COMMAND} -E rm -rf
    ${GEN_DIR} ${CMAKE_SOURCE_DIR}/build
)

