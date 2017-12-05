//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

TEST(Constructor, Default)
{
    ringbuffer<uint32_t> object;

    ASSERT_TRUE(object.empty());
    ASSERT_EQ(object.capacity(), 0);
}

TEST(Constructor, Fill)
{
    constexpr uint32_t count = 1024;

    ringbuffer<uint32_t> object(count, 0xDEADBEEF);

    ASSERT_EQ(object.capacity(), count);

    for (uint32_t i = 0; i < count; ++i)
    {
        ASSERT_EQ(object[i], 0xDEADBEEF);
    }
}

TEST(Constructor, Range)
{
    std::vector<uint32_t> data = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1,
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ringbuffer<uint32_t> object(data.begin(), data.end());

    ASSERT_EQ(data.size(), object.capacity());

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ASSERT_EQ(object[i], data.data()[i]);
    }
}

TEST(Constructor, Copy)
{
    std::vector<uint32_t> data = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1,
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ringbuffer<uint32_t> object(data.begin(), data.end());

    ASSERT_EQ(data.size(), object.capacity());

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ASSERT_EQ(object[i], data.data()[i]);
    }

    ringbuffer<uint32_t> object_2 = object;

    ASSERT_EQ(data.size(), object_2.capacity());

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ASSERT_EQ(object_2[i], object[i]);
    }
}

TEST(Constructor, InitializerList)
{
    ringbuffer<uint32_t> buffer = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    std::vector<uint32_t> buffer_2 = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ASSERT_EQ(buffer.size(), buffer_2.size());

    for (std::size_t index = 0; index < buffer_2.size(); ++index)
    {
        ASSERT_EQ(buffer[index], buffer_2[index]);
    }
}

TEST(Constructor, Move)
{
    ringbuffer<uint32_t> result;

    std::vector<uint32_t> buffer_2 = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    {
        ringbuffer<uint32_t> buffer = {
            1, 2, 3, 4, 5,
            5, 4, 3, 2, 1
        };

        result = std::move(buffer);
    }

    ASSERT_EQ(result.size(), buffer_2.size());

    for (std::size_t index = 0; index < buffer_2.size(); ++index)
    {
        ASSERT_EQ(result[index], buffer_2[index]);
    }
}

TEST(Constructor, CopyOperator)
{
    ringbuffer<uint32_t> result;

    std::vector<uint32_t> buffer = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ringbuffer<uint32_t> source(buffer.begin(), buffer.end());

    result = source;

    ASSERT_EQ(result.size(), buffer.size());

    for (std::size_t index = 0; index < buffer.size(); ++index)
    {
        ASSERT_EQ(result[index], buffer[index]);
    }
}