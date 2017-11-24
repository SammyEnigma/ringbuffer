//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

static std::vector<int> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(Capacity, Size)
{
    ringbuffer<int> zeroObject;
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(zeroObject.size(), 0);
    ASSERT_EQ(object.size(), dataSource.size());
}

TEST(Capacity, Empty)
{
    ringbuffer<int> zeroObject;
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_TRUE(zeroObject.empty());
    ASSERT_FALSE(object.empty());
}

TEST(Capacity, ResizeBigger)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    object.resize(object.size() + 5, 0xDEADBEEF);

    ASSERT_EQ(object.size(), dataSource.size() + 5);

    std::size_t i;
    for (i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object.data()[i], dataSource[i]);
    }

    for (; i < dataSource.size() + 5; ++i)
    {
        ASSERT_EQ(object.data()[i], 0xDEADBEEF);
    }
}

// todo: Add more tests for resize method

TEST(Capacity, Capacity)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size());
}

TEST(Capacity, Reserve)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size());

    object.reserve(object.capacity() + 5);

    ASSERT_EQ(object.capacity(), dataSource.size() + 5);

    for (std::size_t i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object.data()[i], dataSource[i]);
    }
}