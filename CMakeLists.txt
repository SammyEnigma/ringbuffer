cmake_minimum_required(VERSION 3.7)
project(ringbuffer)

option(RINGBUFFER_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD 11)

if (${RINGBUFFER_BUILD_TESTS})
    add_subdirectory(tests)
    add_subdirectory(benchmark)
endif()

add_library(ringbuffer include/ringbuffer.hpp)

target_include_directories(ringbuffer PUBLIC
    include
)

set_target_properties(ringbuffer PROPERTIES LINKER_LANGUAGE CXX)