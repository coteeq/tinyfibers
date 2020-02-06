cmake_minimum_required(VERSION 3.5)

# Google Benchmark

# Disable internal tests
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Enable testing of the benchmark library.")

if(CMAKE_BUILD_TYPE MATCHES "Release" AND ${CLEW_BENCHMARKS})
    add_subdirectory(benchmark)
else()
    message(STATUS "Skip clew/benchmark library in '${CMAKE_BUILD_TYPE}' build type")
endif()
