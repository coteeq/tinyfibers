cmake_minimum_required(VERSION 3.5)

set(EXAMPLE_TARGET tinyfibers_example_hello)

add_executable(${EXAMPLE_TARGET} main.cpp)
target_link_libraries(${EXAMPLE_TARGET} tinyfibers)
