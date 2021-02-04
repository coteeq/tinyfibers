# Tests

set(TESTS_TARGET ${LIB_TARGET}_tests)

file(GLOB TESTS_CXX_SOURCES *.cpp)

add_executable(${TESTS_TARGET} ${TESTS_CXX_SOURCES})
target_link_libraries(${TESTS_TARGET} ${LIB_TARGET})
