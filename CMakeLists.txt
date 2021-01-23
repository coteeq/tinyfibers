cmake_minimum_required(VERSION 3.9)
project(tinyfibers)

include(cmake/CompileOptions.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(third_party)

include(cmake/Platform.cmake)
include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

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
