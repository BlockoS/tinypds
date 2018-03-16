cmake_minimum_required (VERSION 3.1)

project(tinypds)

set(TINY_PDS_MAJOR_VERSION 0)
set(TINY_PDS_MINOR_VERSION 0)
set(TINY_PDS_PATCH_VERSION 1)
set(TINY_PDS_VERSION "${TINY_PDS_MAJOR_VERSION}.${TINY_PDS_MINOR_VERSION}.${TINY_PDS_PATCH_VERSION}")

find_package(Doxygen)

set(PROJECT_NAME tinypds)

file(WRITE ${CMAKE_BINARY_DIR}/tinypds.c "#define TINY_PDS_IMPL\n#include \"${CMAKE_SOURCE_DIR}/tinypds.h\"")
add_library(tinypds STATIC ${CMAKE_BINARY_DIR}/tinypds.c)

file(WRITE ${CMAKE_BINARY_DIR}/tinypds_dom.c "#define TINY_PDS_DOM_IMPL\n#include \"${CMAKE_SOURCE_DIR}/tinypds_dom.h\"")
add_library(tinypds_dom STATIC ${CMAKE_BINARY_DIR}/tinypds_dom.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wshadow -Wextra -std=c99")

set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY IMMEDIATE)

add_custom_target(doc
         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
         SOURCES ${DOXYFILE}
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         COMMENT "Generating API documentation with Doxygen"
         VERBATIM )

enable_testing()

add_library(munit STATIC ${CMAKE_CURRENT_SOURCE_DIR}/test/munit/munit.c)
target_include_directories(munit PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test/munit>)
set_property(TARGET munit PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test/munit>)

include_directories(.)

add_executable(PDS_string_tests test/PDS_string_tests.c)
target_link_libraries(PDS_string_tests munit)

add_executable(PDS_parse_tests test/PDS_parse_tests.c)
target_link_libraries(PDS_parse_tests munit)

add_executable(PDS_parse test/PDS_parse.c test/utils.c)

add_executable(PDS_DOM_parse test/PDS_DOM_parse.c test/utils.c)
target_link_libraries(PDS_DOM_parse tinypds)

add_executable(PDS_DOM_find test/PDS_DOM_find.c)
target_link_libraries(PDS_DOM_find tinypds_dom tinypds munit)

add_test(PDS_string_tests PDS_string_tests)
add_test(PDS_parse_tests PDS_parse_tests)
add_test(PDS_parse_GASPRA_SUMM_RTN.LBL PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/GASPRA_SUMM_RTN.LBL)
add_test(PDS_parse_LOR_0034851929_0X630_ENG_1.LBL PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/LOR_0034851929_0X630_ENG_1.LBL)
add_test(PDS_parse_W20140403T020157750ID20F12.IMG PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/W20140403T020157750ID20F12.IMG)
add_test(PDS_parse_AMI_LR3_R00976_00031_00040.IMG PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/AMI_LR3_R00976_00031_00040.IMG)

add_test(PDS_DOM_parse_W20140403T020157750ID20F12.IMG PDS_DOM_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/W20140403T020157750ID20F12.IMG)
add_test(PDS_DOM_parse_0844636876_lvl1_0.lbl PDS_DOM_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/0844636876_lvl1_0.lbl)
add_test(PDS_DOM_parse_hayabusa_20050907_20050908_v01.lbl PDS_DOM_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/hayabusa_20050907_20050908_v01.lbl)
add_test(PDS_DOM_find PDS_DOM_find)
