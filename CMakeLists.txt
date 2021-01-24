cmake_minimum_required(VERSION 3.9)
project(tinyfibers)

include(cmake/CompileOptions.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(third_party)

include(cmake/Platform.cmake)
include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

add_subdirectory(tinyfibers)

option(TINY_FIBERS_EXAMPLES "Enable tinyfibers examples" OFF)
option(TINY_FIBERS_TESTS "Enable tinyfibers tests" OFF)
option(TINY_FIBERS_BENCHMARKS "Enable tinyfibers benchmarks" OFF)

if(${TINY_FIBERS_EXAMPLES})
    add_subdirectory(examples)
endif()

if(${TINY_FIBERS_TESTS})
    add_subdirectory(tests)
endif()

if(${TINY_FIBERS_BENCHMARKS})
    add_subdirectory(benchmarks)
endif()
