#include <gtest/gtest.h>
#include <ringbuffer.hpp>

TEST(Main, InifitePushBack)
{
    ringbuffer<uint32_t, 128> buffer;

    for (int i = 0; i < 1024; ++i)
    {
        buffer.push_back(i % 256);
    }

    ASSERT_EQ(buffer.size(), 128);
}