cmake_minimum_required(VERSION 3.15)

project(tinyfibers)

include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include(cmake/ClangFormat.cmake)
include(cmake/ClangTidy.cmake)

set(LIB_TARGET ${PROJECT_NAME})

option(TINY_FIBERS_DEVELOPER "Tinyfibers developer mode" OFF)
option(TINY_FIBERS_EXAMPLES "Enable tinyfibers examples" OFF)
option(TINY_FIBERS_TESTS "Enable tinyfibers tests" OFF)
option(TINY_FIBERS_BENCHMARKS "Enable tinyfibers benchmarks" OFF)

add_subdirectory(tf)

add_subdirectory(play)

if(TINY_FIBERS_EXAMPLES OR TINY_FIBERS_DEVELOPER)
    add_subdirectory(examples)
endif()

if(TINY_FIBERS_TESTS OR TINY_FIBERS_DEVELOPER)
    add_subdirectory(tests)
endif()

if(TINY_FIBERS_DEVELOPER)
    add_subdirectory(dev)
endif()
