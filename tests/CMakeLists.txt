cmake_minimum_required(VERSION 3.5)
project(ringbuffer_tests)

add_subdirectory(googletest)

include_directories(../include)
include_directories(googletest/googletest/include)

add_executable(ringbuffer_tests
    main.cpp
    TestElementAccess.cpp
    TestingExtend.hpp
    TestIterators.cpp
    TestMainFunctional.cpp
)

target_link_libraries(ringbuffer_tests ringbuffer gtest)