cmake_minimum_required (VERSION 3.1)

project(img2fitst)

set(PROJECT_NAME img_extract)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(CFITSIO)

include_directories(${CFITSIO_INCLUDE_DIR} ../..)
include_directories(${CFITSIO_INCLUDE_DIR} ../../dom)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wshadow -Wextra")

add_executable(img2fits img2fits.c utils.c)
set_property(TARGET img2fits PROPERTY C_STANDARD 99)
target_link_libraries(img2fits ${CFITSIO_LIBRARIES})

add_executable(img2fits_dom img2fits_dom.c utils.c)
set_property(TARGET img2fits_dom PROPERTY C_STANDARD 99)
target_link_libraries(img2fits_dom ${CFITSIO_LIBRARIES})
