//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

static std::vector<uint32_t> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(Modifiers, AssignRange)
{
    ringbuffer<uint32_t> object;

    object.assign(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.capacity(), dataSource.size());
    ASSERT_EQ(object.size(), dataSource.size());

    for (uint32_t i = 0; i < dataSource.size(); ++i)
    {
        ASSERT_EQ(object[i], dataSource[i]);
    }
}

TEST(Modifiers, AssignFill)
{
    constexpr uint32_t size = 19;

    ringbuffer<uint32_t> object;

    object.assign(size, 0xDEADBEEF);

    ASSERT_EQ(object.capacity(), size);
    ASSERT_EQ(object.size(), size);

    for (uint32_t i = 0; i < size; ++i)
    {
        ASSERT_EQ(object[i], 0xDEADBEEF);
    }
}

TEST(Modifiers, AssignInitializerList)
{
    std::vector<uint32_t> v = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };

    ringbuffer<uint32_t> object;

    object.assign({
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    });

    ASSERT_EQ(object.capacity(), v.capacity());
    ASSERT_EQ(object.size(), v.size());

    for (auto i = 0; i < v.size(); ++i)
    {
        ASSERT_EQ(v[i], object[i]);
    }

}

