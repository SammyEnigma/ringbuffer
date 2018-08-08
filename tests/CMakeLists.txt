cmake_minimum_required(VERSION 3.5)
project(STLRoundBufferTests)

add_subdirectory(googletest)

include_directories(../include)
include_directories(googletest/googletest/include)

add_executable(STLRoundBufferTests
    main.cpp
    TestElementAccess.cpp
    TestingExtend.hpp
    TestIterators.cpp
    TestMainFunctional.cpp
)

target_link_libraries(STLRoundBufferTests STLRoundBuffer gtest)