cmake_minimum_required(VERSION 3.5)

# --------------------------------------------------------------------

# Library tests

file(GLOB TESTS_SRCS *.cpp)

add_executable(tinyfibers_tests ${TESTS_SRCS})
target_link_libraries(tinyfibers_tests tinyfibers)
