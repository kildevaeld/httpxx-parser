cmake_minimum_required(VERSION 3.10)
project(httpxx_parser)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

option(HTTPXX_PARSER_BUILD_TESTS "build and run tests" off)

include_directories(vendor/http-parser)
include_directories(include)

add_subdirectory(src)

if(HTTPXX_PARSER_BUILD_TESTS)
    add_subdirectory(test)
endif(HTTPXX_PARSER_BUILD_TESTS)



# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()