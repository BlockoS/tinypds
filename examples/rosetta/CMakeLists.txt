cmake_minimum_required (VERSION 3.1)

project(rosetta_extract)

set(PROJECT_NAME rosetta_extract)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(CFITSIO)

include_directories(${CFITSIO_INCLUDE_DIR} ../..)
include_directories(${CFITSIO_INCLUDE_DIR} ../../dom)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wshadow -Wextra")

add_executable(rosetta_extract rosetta_extract.c utils.c)
set_property(TARGET rosetta_extract PROPERTY C_STANDARD 99)
target_link_libraries(rosetta_extract ${CFITSIO_LIBRARIES})

add_executable(rosetta_extract_dom rosetta_extract_dom.c utils.c)
set_property(TARGET rosetta_extract PROPERTY C_STANDARD 99)
target_link_libraries(rosetta_extract_dom ${CFITSIO_LIBRARIES})
