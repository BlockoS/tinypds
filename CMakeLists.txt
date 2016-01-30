cmake_minimum_required (VERSION 3.1)

project(tinyPDS)

set(TINY_PDS_MAJOR_VERSION 0)
set(TINY_PDS_MINOR_VERSION 0)
set(TINY_PDS_PATCH_VERSION 1)
set(TINY_PDS_VERSION "${TINY_PDS_MAJOR_VERSION}.${TINY_PDS_MINOR_VERSION}.${TINY_PDS_PATCH_VERSION}")

find_package(Doxygen)

set(PROJECT_NAME tinyPDS)

set(TINY_PDS_SOURCES pds.c)

add_library(tinyPDS STATIC ${TINY_PDS_SOURCES})
set_property(TARGET tinyPDS PROPERTY C_STANDARD 99)

set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY IMMEDIATE)

add_custom_target(doc
         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
         SOURCES ${DOXYFILE}
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         COMMENT "Generating API documentation with Doxygen"
         VERBATIM )
