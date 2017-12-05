cmake_minimum_required(VERSION 3.5)
project(STLRoundBufferTests)

add_subdirectory(googletest)

include_directories(../include)
include_directories(googletest/googletest/include)

add_executable(STLRoundBufferTests
        main.cpp
        TestConstructors.cpp
        TestIterators.cpp
        TestCapacity.cpp TestElementAccess.cpp TestModifiers.cpp TestUsecase.cpp TestSTLAlgo.cpp TestingExtend.hpp)

target_link_libraries(STLRoundBufferTests STLRoundBuffer gtest)