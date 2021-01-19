cmake_minimum_required(VERSION 3.5)

# --------------------------------------------------------------------

# Library tests

file(GLOB TESTS_SRCS *.cpp)

ADD_EXECUTABLE(tinyfibers_tests ${TESTS_SRCS})
TARGET_LINK_LIBRARIES(tinyfibers_tests tinyfibers)
add_test(NAME tinyfibers_tests COMMAND tinyfibers_tests)
