//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

static std::vector<uint32_t> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(Capacity, Size)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(zeroObject.size(), 0);
    ASSERT_EQ(object.size(), dataSource.size());
}

TEST(Capacity, Empty)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    ASSERT_TRUE(zeroObject.empty());
    ASSERT_FALSE(object.empty());
}

TEST(Capacity, ResizeBigger)
{
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    object.resize(object.size() + 5, 0xDEADBEEF);

    ASSERT_EQ(object.size(), dataSource.size() + 5);

    std::size_t i;
    for (i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object[i], dataSource[i]);
    }

    for (; i < dataSource.size() + 5; ++i)
    {
        ASSERT_EQ(object[i], 0xDEADBEEF);
    }
}

// todo: Add more tests for resize method

TEST(Capacity, Capacity)
{
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size());
}

TEST(Capacity, Reserve)
{
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size());

    object.reserve(object.capacity() + 5);

    ASSERT_EQ(object.capacity(), dataSource.size() + 5);

    for (std::size_t i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object[i], dataSource[i]);
    }
}

TEST(Capacity, SizeNotFull)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    ASSERT_EQ(zeroObject.size(), 0);
    ASSERT_EQ(object.size(), dataSource.size());
}

TEST(Capacity, EmptyNotFull)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    ASSERT_TRUE(zeroObject.empty());
    ASSERT_FALSE(object.empty());
}

TEST(Capacity, ResizeBiggerNotFull)
{
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    object.resize(object.size() + 5, 0xDEADBEEF);

    ASSERT_EQ(object.size(), dataSource.size() + 5);

    std::size_t i;
    for (i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object[i], dataSource[i]);
    }

    for (; i < dataSource.size() + 5; ++i)
    {
        ASSERT_EQ(object[i], 0xDEADBEEF);
    }
}

TEST(Capacity, CapacityNotFull)
{
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size() + 10);
}

TEST(Capacity, ReserveNotFull)
{
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size() + 10);

    object.reserve(object.capacity() + 5);

    ASSERT_EQ(object.capacity(), dataSource.size() + 15);

    for (std::size_t i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object[i], dataSource[i]);
    }
}

// ------------------------------------------------------------

TEST(Capacity, SizeRounded)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_EQ(zeroObject.size(), 0);
    ASSERT_EQ(object.size(), dataSource.size());
}

TEST(Capacity, EmptyRounded)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_TRUE(zeroObject.empty());
    ASSERT_FALSE(object.empty());
}

TEST(Capacity, ResizeBiggerRounded)
{
    std::vector<uint32_t> expected = {
        4,  5,  6,  7,  8,  9, 10, 11,
        12, 13, 14, 0x10, 0x20, 0x30,
        0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF
    };

    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    object.resize(object.size() + 5, 0xDEADBEEF);

    ASSERT_EQ(object.size(), expected.size());

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        ASSERT_EQ(object[i], expected[i]);
    }
}

TEST(Capacity, CapacityRounded)
{
    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_EQ(object.capacity(), dataSource.size());
}

TEST(Capacity, ReserveRounded)
{
    std::vector<uint32_t> expected = {
        4,  5,  6,  7,  8,  9, 10, 11,
        12, 13, 14, 0x10, 0x20, 0x30
    };

    ringbuffer<uint32_t> object(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_EQ(object.capacity(), dataSource.size());

    object.reserve(object.capacity() + 5);

    ASSERT_EQ(object.capacity(), expected.size() + 5);

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        ASSERT_EQ(object[i], expected[i]);
    }
}

TEST(Capacity, SizeNotFullRounded)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_EQ(zeroObject.size(), 0);
    ASSERT_EQ(object.size(), dataSource.size());
}

TEST(Capacity, EmptyNotFullRounded)
{
    ringbuffer<uint32_t> zeroObject;
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_TRUE(zeroObject.empty());
    ASSERT_FALSE(object.empty());
}

TEST(Capacity, ResizeBiggerNotFullRounded)
{
    std::vector<uint32_t> expected = {
        4,  5,  6,  7,  8,  9, 10, 11,
        12, 13, 14, 0x10, 0x20, 0x30,
        0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF
    };

    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    object.resize(object.size() + 5, 0xDEADBEEF);

    ASSERT_EQ(object.size(), expected.size());

    std::size_t i;
    for (i = 0; i < expected.size(); ++i)
    {
        ASSERT_EQ(object[i], expected[i]);
    }
}

TEST(Capacity, CapacityNotFullRounded)
{
    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_EQ(object.capacity(), dataSource.size() + 10);
}

TEST(Capacity, ReserveNotFullRounded)
{
    std::vector<uint32_t> expected = {
        4,  5,  6,  7,  8,  9, 10, 11,
        12, 13, 14, 0x10, 0x20, 0x30
    };

    ringbuffer<uint32_t> object;
    object.reserve(dataSource.size() + 10);
    object.assign(dataSource.begin(), dataSource.end());

    object.pop_front();
    object.pop_front();
    object.pop_front();

    object.push_back(0x10);
    object.push_back(0x20);
    object.push_back(0x30);

    ASSERT_EQ(object.capacity(), dataSource.size() + 10);

    object.reserve(object.capacity() + 5);

    ASSERT_EQ(object.capacity(), dataSource.size() + 15);

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        ASSERT_EQ(object[i], expected[i]);
    }
}