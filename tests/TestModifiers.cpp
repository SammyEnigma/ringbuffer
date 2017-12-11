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

TEST(Modifiers, PushBack)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    ASSERT_NO_THROW(object.push_back(0));
    ASSERT_NO_THROW(object.push_back(1));
    ASSERT_NO_THROW(object.push_back(2));
    ASSERT_NO_THROW(object.push_back(3));
    ASSERT_NO_THROW(object.push_back(4));

    ASSERT_THROW(
        object.push_back(1),
        std::overflow_error
    );

    for (uint32_t i = 0; i < 5; ++i)
    {
        ASSERT_EQ(object[i], i);
    }
}

TEST(Modifiers, PushBackRounded)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    ASSERT_NO_THROW(object.push_back(0));
    ASSERT_NO_THROW(object.push_back(1));
    object.pop_front();
    object.pop_front();

    ASSERT_NO_THROW(object.push_back(0));
    ASSERT_NO_THROW(object.push_back(1));
    ASSERT_NO_THROW(object.push_back(2));
    ASSERT_NO_THROW(object.push_back(3));
    ASSERT_NO_THROW(object.push_back(4));

    ASSERT_THROW(
        object.push_back(1),
        std::overflow_error
    );

    for (uint32_t i = 0; i < 5; ++i)
    {
        ASSERT_EQ(object[i], i);
    }
}

TEST(Modifiers, EmplaceBack)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    ASSERT_NO_THROW(object.emplace_back(0));
    ASSERT_NO_THROW(object.emplace_back(1));
    ASSERT_NO_THROW(object.emplace_back(2));
    ASSERT_NO_THROW(object.emplace_back(3));
    ASSERT_NO_THROW(object.emplace_back(4));

    ASSERT_THROW(
        object.emplace_back(1),
        std::overflow_error
    );

    for (uint32_t i = 0; i < 5; ++i)
    {
        ASSERT_EQ(object[i], i);
    }
}

struct TestStruct
{
    TestStruct(uint32_t a, char b) :
        a(a),
        b(b)
    {

    }

    uint32_t a;
    char b;
};

TEST(Modifiers, EmplaceBackStruct)
{
    ringbuffer<TestStruct> object;
    object.reserve(5);

    ASSERT_NO_THROW(object.emplace_back(0, '\0'));
    ASSERT_NO_THROW(object.emplace_back(1, '\1'));
    ASSERT_NO_THROW(object.emplace_back(2, '\2'));
    ASSERT_NO_THROW(object.emplace_back(3, '\3'));
    ASSERT_NO_THROW(object.emplace_back(4, '\4'));

    ASSERT_THROW(
        object.emplace_back(1, '\1'),
        std::overflow_error
    );

    for (uint32_t i = 0; i < 5; ++i)
    {
        ASSERT_EQ(object[i].a, i);
        ASSERT_EQ(object[i].b, i);
    }
}

TEST(Modifiers, EmplaceBackRounded)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    ASSERT_NO_THROW(object.emplace_back(0));
    ASSERT_NO_THROW(object.emplace_back(1));
    object.pop_front();
    object.pop_front();

    ASSERT_NO_THROW(object.emplace_back(0));
    ASSERT_NO_THROW(object.emplace_back(1));
    ASSERT_NO_THROW(object.emplace_back(2));
    ASSERT_NO_THROW(object.emplace_back(3));
    ASSERT_NO_THROW(object.emplace_back(4));

    ASSERT_THROW(
        object.emplace_back(1),
        std::overflow_error
    );

    for (uint32_t i = 0; i < 5; ++i)
    {
        ASSERT_EQ(object[i], i);
    }
}

TEST(Modifiers, EraseBegin)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    object.push_back(0);
    object.push_back(1);
    object.push_back(2);
    object.push_back(3);

    // Equal to pop_front
    auto result = object.erase(object.begin());

    ASSERT_EQ(object.size(), 3);
    ASSERT_EQ(result, object.begin());

    ASSERT_EQ(object[0], 1);
    ASSERT_EQ(object[1], 2);
    ASSERT_EQ(object[2], 3);
}

TEST(Modifiers, EraseEnd)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    object.push_back(0);
    object.push_back(1);
    object.push_back(2);
    object.push_back(3);

    // Equal to pop_front
    auto result = object.erase(object.end() - 1);

    ASSERT_EQ(object.size(), 3);
    ASSERT_EQ(result, object.end());

    ASSERT_EQ(object[0], 0);
    ASSERT_EQ(object[1], 1);
    ASSERT_EQ(object[2], 2);
}

TEST(Modifiers, EraseMiddle)
{
    ringbuffer<uint32_t> object;
    object.reserve(5);

    object.push_back(0);
    object.push_back(1);
    object.push_back(2);
    object.push_back(3);
    object.push_back(4);

    auto result = object.erase(object.begin() + 2); // Remove value `2`

    ASSERT_EQ(object.size(), 4);
    ASSERT_EQ(result, object.begin() + 2);

    ASSERT_EQ(object[0], 0);
    ASSERT_EQ(object[1], 1);
    ASSERT_EQ(object[2], 3);
    ASSERT_EQ(object[3], 4);
}