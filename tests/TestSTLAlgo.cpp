
#include <gtest/gtest.h>
#include <ringbuffer.hpp>

TEST(STLAlgorithms, Inserter)
{
    ringbuffer<char> buffer;
    buffer.reserve(64 * 1024); // 64 kiB

    std::fill_n(
            std::back_inserter(buffer),
            1024,
            0xAA
    );

    ASSERT_EQ(buffer.size(), 1024);

    for (ringbuffer<char>::size_type i = 0; i < 1024; ++i)
    {
        ASSERT_EQ(buffer[i], static_cast<char>(0xAA));
    }
}

TEST(STLAlgorithms, InserterMoved)
{
    ringbuffer<char> buffer;
    buffer.reserve(64 * 1024); // 64 kiB

    buffer.push_back(0x10);
    buffer.push_back(0x20);
    buffer.push_back(0x30);
    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();
    buffer.push_back(0x10);
    buffer.push_back(0x20);
    buffer.push_back(0x30);

    std::fill_n(
            std::back_inserter(buffer),
            1024,
            0xAA
    );

    ASSERT_EQ(buffer.size(), 1024 + 3);

    for (ringbuffer<char>::size_type i = 0; i < 1024; ++i)
    {
        ASSERT_EQ(buffer[i + 3], static_cast<char>(0xAA));
    }
}