//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

static std::vector<int> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(ElementAccess, Front)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.front(), dataSource[0]);
}

TEST(ElementAccess, Back)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.back(), dataSource[dataSource.size() - 1]);
}

TEST(ElementAccess, Operator)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    for (ringbuffer<int>::size_type i = 0; i < object.size(); ++i)
    {
        ASSERT_EQ(dataSource[i], object[i]);
    }
}

TEST(ElementAccess, At)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

    for (ringbuffer<int>::size_type i = 0; i < object.size(); ++i)
    {
        ASSERT_EQ(dataSource[i], object.at(i));
    }
}

TEST(ElementAccess, AtException)
{
    ringbuffer<int> object(dataSource.begin(), dataSource.end());

//    try
//    {
//        object.at(object.size());
//    }
//    catch (std::out_of_range& e)
//    {
//        return;
//    }

    ASSERT_THROW(
        object.at(object.size()),
        std::out_of_range
    );
}

TEST(ElementAccess, ConstFront)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.front(), dataSource[0]);
}

TEST(ElementAccess, ConstBack)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_EQ(object.back(), dataSource[dataSource.size() - 1]);
}



TEST(ElementAccess, ConstOperator)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    for (ringbuffer<int>::size_type i = 0; i < object.size(); ++i)
    {
        ASSERT_EQ(dataSource[i], object[i]);
    }
}

TEST(ElementAccess, ConstAt)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    for (ringbuffer<int>::size_type i = 0; i < object.size(); ++i)
    {
        ASSERT_EQ(dataSource[i], object.at(i));
    }
}

TEST(ElementAccess, ConstAtException)
{
    const ringbuffer<int> object(dataSource.begin(), dataSource.end());

    ASSERT_THROW(
        object.at(object.size()),
        std::out_of_range
    );
}