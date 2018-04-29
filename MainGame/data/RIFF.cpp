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


#include "RIFF.hpp"

#include <streamReaders.hpp>
#include <streamWriters.hpp>

bool readFromStream(sf::InputStream& stream, RIFF& riff)
{
    uint32_t chunkSize;
    if (!readFromStream(stream, riff.code, chunkSize)) return false;

    riff.data.resize(chunkSize);
    return stream.read(riff.data.data(), chunkSize) == chunkSize;
}

bool writeToStream(OutputStream& stream, const RIFF& riff)
{
    if (!writeToStream(stream, riff.code, (uint32_t)riff.data.size())) return false;
    return stream.write(riff.data.data(), riff.data.size());
}