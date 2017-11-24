//
// Created by megaxela on 11/24/17.
//
#include <gtest/gtest.h>
#include <ringbuffer.hpp>

static std::vector<int> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(Iterators, BeginEnd)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

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
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

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
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    auto begin = object.cbegin();
    auto end = object.cend();

    for (auto sourceBegin = dataSource.begin();
         sourceBegin != dataSource.end();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

TEST(Iterators, ReverseBeginEnd)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    auto begin = object.rbegin();
    auto end = object.rend();

    for (auto sourceBegin = dataSource.rbegin();
         sourceBegin != dataSource.rend();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

TEST(Iterators, ReverseConstBeginEnd)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    auto begin = object.rbegin();
    auto end = object.rend();

    for (auto sourceBegin = dataSource.rbegin();
         sourceBegin != dataSource.rend();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

TEST(Iterators, ReverseExplicitConstBeginEnd)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    auto begin = object.crbegin();
    auto end = object.crend();

    for (auto sourceBegin = dataSource.crbegin();
         sourceBegin != dataSource.crend();
         ++sourceBegin, ++begin)
    {
        ASSERT_EQ(*sourceBegin, *begin);
    }
}

