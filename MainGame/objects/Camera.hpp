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
#include <vector>

#include <rect.hpp>

class GameScene;

class Camera final
{
public:
    enum class Direction { Right, Down, Left, Up };

private:
    glm::vec2 position;
    GameScene& gameScene;
    FrameTime curTime, transitionTime, shakeTime;
    std::vector<glm::vec2> shakeSamples;
    FrameDuration shakePeriod;
    Direction transitionDirection;

    glm::vec2 getShakeDisplacement() const;

public:
    Camera(GameScene& scene) : position(), gameScene(scene) {}
    ~Camera() {}
    
    void scheduleTransition(Direction direction);
    bool transitionOccuring() const;

    bool isVisible(glm::vec2 point) const;
    bool isVisible(util::rect rect) const;
    
    void update(FrameTime curTime);
    glm::vec2 getGlobalDisplacement() const;

    void applyShake(FrameDuration duration, FrameDuration period, float amp);
};
