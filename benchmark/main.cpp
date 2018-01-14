#include <benchmark/benchmark.h>
#include <ringbuffer.hpp>
#include "TestType.hpp"
#include "bench_extend/TemplateFunctionBenchmark.hpp"

template<std::size_t N>
static void creation(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ringbuffer<Type, N>());
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void move(benchmark::State &state)
{
    ringbuffer<Type, N> buffer;
    ringbuffer<Type, N> copy;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        copy = std::move(buffer);

        benchmark::DoNotOptimize(copy);

        state.PauseTiming();
        buffer = std::move(copy);
        state.ResumeTiming();
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void push_back_full(benchmark::State &state)
{
    ringbuffer<Type, N> buffer;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            buffer.push_back(TEST_VALUE);
        }

        state.PauseTiming();

        buffer.clear();

        state.ResumeTiming();
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void pop_back_full(benchmark::State &state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        while (!buffer.empty())
        {
            buffer.pop_back();
        }

        state.PauseTiming();

        for (std::size_t i = 0; i < N; ++i)
        {
            buffer.push_back(TEST_VALUE);
        }

        state.ResumeTiming();
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void pop_front_full(benchmark::State &state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        while (!buffer.empty())
        {
            buffer.pop_front();
        }

        state.PauseTiming();

        for (std::size_t i = 0; i < N; ++i)
        {
            buffer.push_back(TEST_VALUE);
        }

        state.ResumeTiming();
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void pop_front_multiple_full(benchmark::State &state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        buffer.pop_front(N);

        state.PauseTiming();

        for (std::size_t i = 0; i < N; ++i)
        {
            buffer.push_back(TEST_VALUE);
        }

        state.ResumeTiming();
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void clear(benchmark::State& state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        buffer.clear();

        state.PauseTiming();

        for (std::size_t i = 0; i < N; ++i)
        {
            buffer.push_back(TEST_VALUE);
        }

        state.ResumeTiming();
    }

    state.SetComplexityN(static_cast<int>(N));

}

template<std::size_t N>
static void iterating_index(benchmark::State& state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            benchmark::DoNotOptimize(buffer[i]);
        }
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void iterating_at(benchmark::State& state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            benchmark::DoNotOptimize(buffer.at(i));
        }
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void iterating_iterators(benchmark::State& state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        for (auto&& el : buffer)
        {
            benchmark::DoNotOptimize(el);
        }
    }

    state.SetComplexityN(static_cast<int>(N));
}


template<std::size_t N>
static void size(benchmark::State& state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(buffer.size());
    }

    state.SetComplexityN(static_cast<int>(N));
}

template<std::size_t N>
static void max_size(benchmark::State& state)
{
    ringbuffer<Type, N> buffer;

    for (std::size_t i = 0; i < N; ++i)
    {
        buffer.push_back(TEST_VALUE);
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(buffer.max_size());
    }

    state.SetComplexityN(static_cast<int>(N));
}

BENCHMARK_TEMPLATE_RANGE(creation)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(move)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(push_back_full)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(pop_back_full)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(pop_front_multiple_full)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(pop_front_full)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(clear)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(iterating_index)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(iterating_at)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(iterating_iterators)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(size)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_TEMPLATE_RANGE(max_size)
    ->TemplateRange<1, 1 << 15>()
    ->Complexity();

BENCHMARK_MAIN();