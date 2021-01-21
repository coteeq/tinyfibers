cmake_minimum_required(VERSION 3.9)
project(tinyfibers)

include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(third_party)

include(cmake/Platform.cmake)
add_subdirectory(tinyfibers)

option(TINY_FIBER_EXAMPLES "Enable tinyfiber examples" OFF)
option(TINY_FIBER_TESTS "Enable tinyfiber tests" OFF)
option(TINY_FIBER_BENCHMARKS "Enable tinyfiber benchmarks" OFF)

if(${TINY_FIBER_EXAMPLES})
    add_subdirectory(examples)
endif()

if(${TINY_FIBER_TESTS})
    add_subdirectory(tests)
endif()


if(${TINY_FIBER_BENCHMARKS})
    add_subdirectory(benchmarks)
endif()
