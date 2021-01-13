cmake_minimum_required(VERSION 3.5)

# --------------------------------------------------------------------

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_ASM_SOURCES ${LIB_PATH}/*.S)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

enable_language(ASM)

add_library(tinyfibers STATIC ${LIB_CXX_SOURCES} ${LIB_ASM_SOURCES} ${LIB_HEADERS})
target_include_directories(tinyfibers PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(tinyfibers PUBLIC pthread wheels)

# --------------------------------------------------------------------

# Linters

if(CLANG_FORMAT_TOOL)
    add_clang_format_target(
            tinyfibers_clang_format
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${LIB_HEADERS} ${LIB_CXX_SOURCES})
endif()

if(CLANG_TIDY_TOOL)
    add_clang_tidy_target(
            tinyfibers_clang_tidy
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${LIB_INCLUDE_PATH}
            ${LIB_HEADERS} ${LIB_CXX_SOURCES})
endif()
