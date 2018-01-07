//
// Created by megaxela on 11/24/17.
//

#include <gtest/gtest.h>
#include <ringbuffer.hpp>

constexpr std::size_t Size = 14;

static std::array<uint32_t, Size> dataSource = {
    1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14
};

TEST(ElementAccess, Total)
{
    ringbuffer<uint32_t, Size> rb;

    for (auto&& el : dataSource)
    {
        rb.push_back(el);
    }

    // Checking
    for (ringbuffer<uint32_t, Size>::size_type i = 0;
         i < rb.size();
         ++i)
    {
        ASSERT_EQ(rb[i], dataSource[i]);
    }

    for (unsigned int i : dataSource)
    {
        ASSERT_EQ(rb.front(), i);
        rb.pop_front();
    }

    ASSERT_THROW(rb.pop_front(), std::overflow_error);
}