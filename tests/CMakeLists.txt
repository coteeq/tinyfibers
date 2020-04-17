cmake_minimum_required(VERSION 3.5)

# --------------------------------------------------------------------

# Google Test

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# --------------------------------------------------------------------

# Library tests

file(GLOB TESTS_SRCS *.cpp)

ADD_EXECUTABLE(tinyfibers_tests ${TESTS_SRCS})
TARGET_LINK_LIBRARIES(tinyfibers_tests tinyfibers ${GTEST_BOTH_LIBRARIES})
add_test(NAME tinyfibers_tests COMMAND tinyfibers_tests)
