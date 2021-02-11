set(EXAMPLE_TARGET ${LIB_TARGET}_example_yield)

add_executable(${EXAMPLE_TARGET} main.cpp)
target_link_libraries(${EXAMPLE_TARGET} ${LIB_TARGET})
