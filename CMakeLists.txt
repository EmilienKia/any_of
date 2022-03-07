cmake_minimum_required(VERSION 3.19)
project(any_of)

set(CMAKE_CXX_STANDARD 17)

add_executable(any_of_tests any_of.hpp tests/catch.hpp tests/test_any_of.cpp tests/test_any_of_opt.cpp tests/testmain.cpp)
add_test(NAME any_of_tests COMMAND any_of_tests)
