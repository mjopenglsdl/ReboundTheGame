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


#include "Renderer.hpp"

#include <iostream>
#include "defaults.hpp"

Renderer::Renderer() noexcept
{
	clearState();
}

Renderer::Renderer(Renderer&& other) noexcept : Renderer()
{
    swap(*this, other);
}
    
Renderer& Renderer::operator=(Renderer other)
{
    swap(*this, other);
    return *this;
}

void swap(Renderer& r1, Renderer& r2)
{
    using std::swap;
    
    swap(r1.drawableList, r2.drawableList);
    swap(r1.transformStack, r2.transformStack);
    swap(r1.currentTransform, r2.currentTransform);
}

inline static std::ostream& operator<<(std::ostream& out, const glm::mat3& transform)
{
	auto mtx = glm::value_ptr(transform);
    out << '(' << mtx[0];
    for (size_t i = 1; i < 9; i++) out << ',' << mtx[i];
    return out << ')';
}

void Renderer::pushDrawable(Drawable &drawable, long depth)
{
    drawableList.emplace(depth, std::make_pair(std::ref(drawable), currentTransform));
}

void Renderer::clearState()
{
    drawableList.clear();
    while (!transformStack.empty()) transformStack.pop();
	currentTransform = util::scale(1.0f/ScreenWidth, 1.0f/ScreenHeight);
}

void Renderer::pushTransform()
{
    transformStack.push(currentTransform);
}

void Renderer::popTransform()
{
    currentTransform = transformStack.top();
    transformStack.pop();
}
