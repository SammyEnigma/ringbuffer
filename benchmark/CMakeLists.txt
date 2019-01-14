project(ringbuffer_benchmark)

set(CMAKE_CXX_STANDARD 17)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/benchmark/CMakeLists.txt")
    set(BENCHMARK_ENABLE_TESTING Off)
    add_subdirectory(benchmark)
endif()

add_executable(ringbuffer_benchmark
        main.cpp
        TestType.hpp
        bench_extend/TemplateFunctionBenchmark.hpp
)

target_link_libraries(ringbuffer_benchmark
        ringbuffer
        gtest
        benchmark
)

target_compile_options(ringbuffer_benchmark PUBLIC -O3)