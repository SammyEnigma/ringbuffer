
#include <gtest/gtest.h>
#include <ringbuffer.hpp>
#include <random>
#include "TestingExtend.hpp"

static std::default_random_engine generator;
static std::uniform_int_distribution<uint8_t> distribution;

void fill_buffer(ringbuffer<uint8_t>& buffer)
{
//    auto count = distribution(generator) * 0.8;
    uint8_t count = 254;

    for (uint8_t i = 0; i < count; ++i)
    {
        buffer.push_back(distribution(generator));
    }
}

void read_buffer(ringbuffer<uint8_t>& buffer, ringbuffer<uint8_t>::size_type& readBytes)
{
//    auto count = distribution(generator);
    uint8_t count = 255;

    for (uint8_t i = 0; i < count && !buffer.empty(); ++i)
    {
        buffer.pop_front();
    }

    readBytes += count;
}

TEST(Usecases, CacheBuffer)
{

    ringbuffer<uint8_t> buffer;
    buffer.reserve(64 * 1024);

    std::fill_n(
            std::back_inserter(buffer),
            60 * 1024,
            0xAA
    );

    ringbuffer<uint8_t>::size_type readBytes = 0;

    while (!buffer.empty())
    {
        fill_buffer(buffer);
        read_buffer(buffer, readBytes);
    }

    PRINTF("Read %d bytes.\n", readBytes);
}