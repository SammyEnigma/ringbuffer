cmake_minimum_required(VERSION 3.9)
project(STLRoundBuffer)

option(STLROUNDBUFFER_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD 11)

if (${STLROUNDBUFFER_BUILD_TESTS})
    add_subdirectory(tests)
endif()

add_library(STLRoundBuffer src/ringbuffer.cpp include/ringbuffer.hpp)