#include <iostream>
#include <vector>
#include "include/ringbuffer.hpp"

int main()
{
    std::vector<int> a = {
        1, 2, 3, 4, 5
    };

    std::vector<uint16_t> b;

    ringbuffer<int> rb(a.begin(), a.end());

    auto begin = rb.begin();
    auto end = rb.end();

    auto res = begin == end;

    std::cout << res << std::endl;

    for (; begin != end; ++begin)
    {
        std::cout << *begin << std::endl;
    }
}