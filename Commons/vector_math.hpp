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


#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>

template <typename T>
T degreesToRadians(T degrees) { return T(0.0174532925199432957692369077) * degrees; }

template <typename T>
T radiansToDegrees(T radians) { return T(57.2957795130823208767981548141) * radians; }

template <typename T>
T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

template <typename T>
T dot(const sf::Vector3<T>& v1, const sf::Vector3<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template <typename T>
auto lengthSquared(const T& v) { return dot(v,v); }

template <typename T>
auto length(const T& v) { return std::sqrt(lengthSquared(v)); }

template <typename T>
auto normalize(const T& v) { return v/length(v); }

template <typename T>
auto distanceSquared(const T& v1, const T& v2) { return lengthSquared(v1-v2); }

template <typename T>
auto distance(const T& v1, const T& v2) { return std::sqrt(distanceSquared(v1,v2)); }

template <typename T>
auto project(const T& v, const T& vp)
{
    return vp * dot(v,vp)/dot(vp,vp);
}

template <typename T>
T cross(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

template <typename T>
sf::Vector3<T> cross(const sf::Vector3<T>& v1, const sf::Vector3<T>& v2)
{
    return { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
}

template <typename T>
sf::Vector2<T> fromRadiusAngleRadians(T radius, T angle)
{
    return radius * sf::Vector2<T>{ std::cos(angle), std::sin(angle) };
}

template <typename T>
sf::Vector2<T> fromRadiusAngleDegrees(T radius, T angle)
{
    return fromRadiusAngleRadians(radius, degreesToRadians(angle));
}

template <typename T>
auto angle(const sf::Vector2<T>& v)
{
    return std::atan2(v.y, v.x);
}

template <typename T>
auto angleBetween(const T& v1, const T& v2)
{
    return std::acos(dot(v1,v2)/std::sqrt(lengthSquared(v1)*lengthSquared(v2)));
}
