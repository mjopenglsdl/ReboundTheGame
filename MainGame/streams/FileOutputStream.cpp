//
// Copyright (c) 2016-2018 João Baptista de Paula e Silva.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//


#include "FileOutputStream.hpp"

FileOutputStream::~FileOutputStream()
{
    if (file) std::fclose(file);
}

bool FileOutputStream::open(const std::string& filename)
{
    if (file) std::fclose(file);
    return (file = std::fopen(filename.c_str(), "wb")) != nullptr;
}

bool FileOutputStream::openForAppending(const std::string& filename)
{
    if (file) std::fclose(file);
    return (file = std::fopen(filename.c_str(), "ab")) != nullptr;
}

bool FileOutputStream::write(const void* data, size_t size)
{
    if (!file) return false;
    return std::fwrite(data, size, 1, file) > 0;
}
