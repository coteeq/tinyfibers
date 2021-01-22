cmake_minimum_required(VERSION 3.5)

SET(EXAMPLE_TARGET tinyfibers_example_hello)

ADD_EXECUTABLE(${EXAMPLE_TARGET} main.cpp)
TARGET_LINK_LIBRARIES(${EXAMPLE_TARGET} tinyfibers)
# add_test(NAME tinyfibers_tests COMMAND tinyfibers_tests)
