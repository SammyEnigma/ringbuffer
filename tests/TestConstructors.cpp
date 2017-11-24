//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

TEST(Constructors, Default)
{
    ringbuffer<int> object;

    ASSERT_TRUE(object.empty());
    ASSERT_EQ(object.capacity(), 0);
}

TEST(Constructors, Fill)
{
    constexpr int count = 1024;

    ringbuffer<int> object(count, 0xDEADBEEF);

    ASSERT_EQ(object.capacity(), count);

    for (int i = 0; i < count; ++i)
    {
        ASSERT_EQ(object.data()[i], 0xDEADBEEF);
    }
}

TEST(Constructors, Range)
{
    std::vector<int> data = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1,
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ringbuffer<int> object(data.begin(), data.end());

    ASSERT_EQ(data.size(), object.capacity());

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ASSERT_EQ(object.data()[i], data.data()[i]);
    }
}

TEST(Constructor, Copy)
{
    std::vector<int> data = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1,
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ringbuffer<int> object(data.begin(), data.end());

    ASSERT_EQ(data.size(), object.capacity());

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ASSERT_EQ(object.data()[i], data.data()[i]);
    }

    ringbuffer<int> object_2 = object;

    ASSERT_EQ(data.size(), object_2.capacity());

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ASSERT_EQ(object_2.data()[i], object.data()[i]);
    }
}

TEST(Constructor, InitializerList)
{
    ringbuffer<int> buffer = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    std::vector<int> buffer_2 = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ASSERT_EQ(buffer.size(), buffer_2.size());

    for (std::size_t index = 0; index < buffer_2.size(); ++index)
    {
        ASSERT_EQ(buffer.data()[index], buffer_2.data()[index]);
    }
}

TEST(Constructor, Move)
{
    ringbuffer<int> result;

    std::vector<int> buffer_2 = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    {
        ringbuffer<int> buffer = {
            1, 2, 3, 4, 5,
            5, 4, 3, 2, 1
        };

        result = std::move(buffer);
    }

    ASSERT_EQ(result.size(), buffer_2.size());

    for (std::size_t index = 0; index < buffer_2.size(); ++index)
    {
        ASSERT_EQ(result.data()[index], buffer_2.data()[index]);
    }
}

TEST(Constructor, CopyOperator)
{
    ringbuffer<int> result;

    std::vector<int> buffer = {
        1, 2, 3, 4, 5,
        5, 4, 3, 2, 1
    };

    ringbuffer<int> source(buffer.begin(), buffer.end());

    result = source;

    ASSERT_EQ(result.size(), buffer.size());

    for (std::size_t index = 0; index < buffer.size(); ++index)
    {
        ASSERT_EQ(result.data()[index], buffer.data()[index]);
    }
}