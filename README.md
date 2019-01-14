# STL like ringbuffer
It's C++11 STL like static ringbuffer implementation.

## Status
| Linux CMake(Travis) | Coverage |
|---------------------|----------|
| [![Build Status](https://travis-ci.com/Megaxela/ringbuffer.svg?branch=master)](https://travis-ci.com/Megaxela/ringbuffer) | None |

## Build
It's .hpp only CMake based project, it can be used as subdirectory
(`add_subdirectory`) in your CMake project.

Also you can just copy `ringbuffer.hpp` to your project.

You may build tests and benchmark with following example:
1. Clone repo: `git clone https://github.com/Megaxela/ringbuffer`
1. Go into cloned repo: `cd ringbuffer`
1. Create build folder `mkdir build`
1. Go into build folder `cd build`
1. Setup project: `cmake -DBENCHMARK_ENABLE_TESTING=Off -DRINGBUFFER_BUILD_TESTS=On ..`
1. Build library: `cmake --build .` or `make` 

## Usage example
```cpp
#include <ringbuffer.hpp>

int main(int argc, char** argv)
{
    ringbuffer<int, 256> buffer;
    
    for (int i = 0; i < 1024; ++i)
    {
        buffer.push_back(i);
    }
    
    return 0;
}
```

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT) 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.