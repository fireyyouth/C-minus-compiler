cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 14)

add_library(type_to_name type_to_name.cc)
add_library(tokenize tokenize.cc)
add_library(parser parser.cc)
add_library(gen_code gen_code.cc)
target_link_libraries(gen_code type_to_name)

add_library(dot_dump dot_dump.cc)
target_link_libraries(dot_dump type_to_name)

add_executable(test_tokenize test_tokenize.cc)
target_link_libraries(test_tokenize tokenize type_to_name)

add_executable(test_parser test_parser.cc)
target_link_libraries(test_parser parser dot_dump)

add_executable(main main.cc)
target_link_libraries(main tokenize parser gen_code dot_dump)
