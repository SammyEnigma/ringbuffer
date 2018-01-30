cmake_minimum_required(VERSION 3.7)
project(STLRoundBuffer)

option(STLROUNDBUFFER_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD 11)

if (${STLROUNDBUFFER_BUILD_TESTS})
    add_subdirectory(tests)
endif()

add_library(STLRoundBuffer include/ringbuffer.hpp)

target_include_directories(STLRoundBuffer PUBLIC
        include
)

set_target_properties(STLRoundBuffer PROPERTIES LINKER_LANGUAGE CXX)