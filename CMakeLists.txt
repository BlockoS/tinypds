cmake_minimum_required (VERSION 3.1)

project(tinyPDS)

set(TINY_PDS_MAJOR_VERSION 0)
set(TINY_PDS_MINOR_VERSION 0)
set(TINY_PDS_PATCH_VERSION 1)
set(TINY_PDS_VERSION "${TINY_PDS_MAJOR_VERSION}.${TINY_PDS_MINOR_VERSION}.${TINY_PDS_PATCH_VERSION}")

find_package(Doxygen)

set(PROJECT_NAME tinyPDS)

add_library(tinyPDS STATIC pds.c)
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

set(TINY_PDS_TEST_SOURCE_FILES test/utils.c test/test.c)

enable_testing()

include_directories(.)
add_executable(PDS_find_first test/PDS_find_first.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_datetime test/PDS_parse_datetime.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_identifier test/PDS_parse_identifier.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_int_base test/PDS_parse_int_base.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_int test/PDS_parse_int.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_real test/PDS_parse_real.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_sequence test/PDS_parse_sequence.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_set test/PDS_parse_set.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_statement test/PDS_parse_statement.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_string test/PDS_parse_string.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_symbol test/PDS_parse_symbol.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse_unit test/PDS_parse_unit.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_skip_whitespaces test/PDS_skip_whitespaces.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_string_case_compare test/PDS_string_case_compare.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_string_compare test/PDS_string_compare.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_trim test/PDS_trim.c ${TINY_PDS_TEST_SOURCE_FILES})
add_executable(PDS_parse test/PDS_parse.c ${TINY_PDS_TEST_SOURCE_FILES})

add_test(PDS_find_first PDS_find_first )
add_test(PDS_parse_datetime PDS_parse_datetime)
add_test(PDS_parse_identifier PDS_parse_identifier)
add_test(PDS_parse_int_base PDS_parse_int_base)
add_test(PDS_parse_int PDS_parse_int)
add_test(PDS_parse_real PDS_parse_real)
add_test(PDS_parse_sequence PDS_parse_sequence)
add_test(PDS_parse_set PDS_parse_set)
add_test(PDS_parse_statement PDS_parse_statement)
add_test(PDS_parse_string PDS_parse_string)
add_test(PDS_parse_symbol PDS_parse_symbol)
add_test(PDS_parse_unit PDS_parse_unit)
add_test(PDS_skip_whitespaces PDS_skip_whitespaces)
add_test(PDS_string_case_compare PDS_string_case_compare)
add_test(PDS_string_compare PDS_string_compare)
add_test(PDS_trim PDS_trim)
add_test(PDS_parse_GASPRA_SUMM_RTN.LBL PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/GASPRA_SUMM_RTN.LBL)
add_test(PDS_parse_LOR_0034851929_0X630_ENG_1.LBL PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/LOR_0034851929_0X630_ENG_1.LBL)
add_test(PDS_parse_W20140403T020157750ID20F12.IMG PDS_parse ${CMAKE_CURRENT_SOURCE_DIR}/test/data/W20140403T020157750ID20F12.IMG)
