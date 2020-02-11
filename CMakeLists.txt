cmake_minimum_required(VERSION 3.9)
project(tinyfiber)

include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include(cmake/Platform.cmake)
add_subdirectory(tinyfiber)

option(TINY_FIBER_TESTS "Enable tinyfiber library tests" OFF)
option(TINY_FIBER_BENCHMARKS "Enable tinyfiber library benchmarks" OFF)

if(${TINY_FIBER_TESTS})
    add_subdirectory(tests)
endif()

if(${TINY_FIBER_BENCHMARKS})
    add_subdirectory(benchmarks)
endif()
