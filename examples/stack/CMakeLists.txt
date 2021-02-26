set(EXAMPLE_TARGET ${LIB_TARGET}_example_stack)

add_executable(${EXAMPLE_TARGET} main.cpp)
target_link_libraries(${EXAMPLE_TARGET} ${LIB_TARGET})
