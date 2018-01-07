//
// Created by megaxela on 1/8/18.
//
#include <gtest/gtest.h>
#include <ringbuffer.hpp>

constexpr std::size_t Size = 14;

static std::array<uint32_t, Size> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(Iterators, Distance)
{
    ringbuffer<uint32_t, Size> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(std::distance(object.begin(), object.end()), Size);

    object.clear();

    ASSERT_EQ(std::distance(object.begin(), object.end()), 0);
}

TEST(Iterators, BeginEnd)
{
    ringbuffer<uint32_t, Size> object(dataSource.begin(), dataSource.end());

    auto begin = object.begin();
    auto end = object.end();

    for (auto sourceBegin = dataSource.begin();
         sourceBegin != dataSource.end();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

TEST(Iterators, ConstBeginEnd)
{
    const ringbuffer<uint32_t, Size> object(dataSource.begin(), dataSource.end());

    auto begin = object.begin();
    auto end = object.end();

    for (auto sourceBegin = dataSource.begin();
         sourceBegin != dataSource.end();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

TEST(Iterators, ExplicitConstBeginEnd)
{
    const ringbuffer<uint32_t, Size> object(dataSource.begin(), dataSource.end());

    auto begin = object.cbegin();
    auto end = object.cend();

    for (auto sourceBegin = dataSource.begin();
         sourceBegin != dataSource.end();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

