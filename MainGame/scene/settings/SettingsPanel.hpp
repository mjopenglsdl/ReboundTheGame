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

#include <chronoUtils.hpp>
#include "ui/UIPointer.hpp"
#include "language/LangID.hpp"

class SettingsBase;
class Renderer;

class SettingsPanel
{
protected:
    SettingsBase* curSettings;
    UIPointer& pointer;

    SettingsPanel(SettingsBase* curSettings, UIPointer& pointer) : curSettings(curSettings), pointer(pointer) {}

    glm::vec2 getCenterPosition() const;
    const LangID& getBackId() const;
    void executeBackAction();

public:
    virtual ~SettingsPanel() = default;

    virtual void update(FrameTime curTime) = 0;
    virtual void render(Renderer& renderer) = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};