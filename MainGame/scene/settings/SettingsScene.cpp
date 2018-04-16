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


#include "SettingsScene.hpp"

#include "scene/SceneManager.hpp"
#include "scene/TitleScene.hpp"

#include "resources/ResourceManager.hpp"
#include "language/LocalizationManager.hpp"

#include "scene/pause/PauseScene.hpp"
#include "rendering/Renderer.hpp"

#include "ui/UIButtonCommons.hpp"

#include "RootSettingsPanel.hpp"

#include <defaults.hpp>

constexpr float FrameWidth = PlayfieldWidth;
constexpr float ButtonHeight = 36;
constexpr float ButtonSpace = 8;
constexpr float ButtonCaptionSize = 28;
constexpr float TotalHeight = 11 * ButtonHeight + 10 * ButtonSpace;

const LangID Titles[] =
{
    "settings-video-settings",
    "settings-audio-settings",
    "settings-input-settings",
};

const LangID VideoIdentifiers[] =
{
    "settings-fullscreen",
    "settings-vsync",
    "settings-change-language",
};

const LangID SoundIdentifiers[] =
{
    "settings-music-volume",
    "settings-sound-volume",
};


const LangID InputIdentifiers[] =
{
    "settings-keyboard-controls",
    "settings-joystick-controls",
};

extern bool GlobalUpdateWindowHandler;

SettingsBase::SettingsBase(Settings& settings, InputManager& im, ResourceManager& rm, LocalizationManager &lm,
    sf::Vector2f centerPos, UIPointer& pointer, LangID backId) : centerPosition(centerPos), backId(backId),
    nextSettingsPanel(nullptr)
{
    curSettingsPanel.reset(new RootSettingsPanel(settings, im, rm, lm, this, pointer));
}

void SettingsBase::update(FrameTime curTime)
{
    if (nextSettingsPanel)
    {
        curSettingsPanel.reset(nextSettingsPanel);
        nextSettingsPanel = nullptr;
    }

    curSettingsPanel->update(curTime);
}

void SettingsBase::render(Renderer& renderer) { curSettingsPanel->render(renderer); }
void SettingsBase::activate() { curSettingsPanel->activate(); }
void SettingsBase::deactivate() { curSettingsPanel->deactivate(); }
void SettingsBase::changeSettingsPanel(SettingsPanel *panel) { nextSettingsPanel = panel; }

SettingsScene::SettingsScene(Settings& settings, InputManager& im, ResourceManager& rm, LocalizationManager &lm)
    : SettingsBase(settings, im, rm, lm, sf::Vector2f(ScreenWidth, ScreenHeight)/2.0f, pointer, "settings-scene-back"),
    sceneFrame(rm.load<sf::Texture>("settings-scene-frame.png")), pointer(im, rm)
{
    using namespace std::literals::chrono_literals;
    backAction = [&,this]
    {
        auto scene = new TitleScene(settings, im, rm, lm);
        getSceneManager().replaceSceneTransition(scene, 1s);
    };
}

void SettingsScene::render(Renderer& renderer)
{
    SettingsBase::render(renderer);
    
    renderer.pushTransform();
    renderer.currentTransform.translate(ScreenWidth/2, ScreenHeight/2);
    renderer.pushDrawable(sceneFrame, {}, 3000);
    renderer.popTransform();
    
    pointer.render(renderer);
}

SettingsPauseFrame::SettingsPauseFrame(Settings& settings, InputManager& im, ResourceManager& rm,
    LocalizationManager& lm, UIPointer& pointer, PauseScene* scene) : SettingsBase(settings, im, rm, lm,
    sf::Vector2f(ScreenWidth/2, ScreenHeight/2 + 32), pointer, "settings-pause-resume")
{
    backAction = [=] { scene->unpause(); };
}

void SettingsPauseFrame::render(Renderer& renderer)
{
    renderer.currentTransform.translate(0, -64);
    SettingsBase::render(renderer);
}
