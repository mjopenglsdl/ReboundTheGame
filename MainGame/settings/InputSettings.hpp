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

#include "input/InputSource.hpp"

struct KeyboardSettings
{
    InputSource dashInput;
    InputSource jumpInput;
    InputSource bombInput;

    InputSource switchScreenLeft;
    InputSource switchScreenRight;
    InputSource pauseInput;
    InputSource okInput;
    InputSource cancelInput;

    InputSource moveUp;
    InputSource moveDown;
    InputSource moveLeft;
    InputSource moveRight;
};

struct JoystickSettings
{
    InputSource dashInput;
    InputSource jumpInput;
    InputSource bombInput;

    InputSource switchScreenLeft;
    InputSource switchScreenRight;
    InputSource pauseInput;
    InputSource okInput;
    InputSource cancelInput;

    InputSource movementAxisX;
    InputSource movementAxisY;
    InputSource movementAxisXAlt;
    InputSource movementAxisYAlt;
};

struct InputSettings
{
    KeyboardSettings keyboardSettings;
    JoystickSettings joystickSettings;
};

bool readFromStream(InputStream &stream, InputSettings& settings);
bool writeToStream(OutputStream& stream, const InputSettings& settings);
