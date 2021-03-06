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


#include "TitleScene.hpp"

#include "resources/ResourceManager.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/Texture.hpp"
#include "ui/UIButton.hpp"
#include "ui/UIButtonCommons.hpp"
#include "defaults.hpp"
#include "scene/SceneManager.hpp"
#include "language/LocalizationManager.hpp"

#include "GameScene.hpp"
#include "scene/settings/SettingsScene.hpp"
#include "scene/FileSelectScene.hpp"
#include "ColorList.hpp"

#include <transforms.hpp>

using namespace std::literals::chrono_literals;

constexpr float ButtonHeight = 48;
constexpr float ButtonSpace = 12;
constexpr float SetBottomSpace = 32;
constexpr size_t ButtonCaptionSize = 42;
constexpr float ButtonTop = ScreenHeight - SetBottomSpace + ButtonSpace;

const LangID ButtonIdentifiers[] =
{
    "title-button-new-game",
    "title-button-load-game",
    "title-button-settings",
    "title-button-exit",
};

TitleScene::TitleScene(Services& services)
    : background(services.resourceManager.load<Texture>("title-background.png"), glm::vec2(0, 0)),
    foreground(services.resourceManager.load<Texture>("title-foreground.png"), glm::vec2(0, 0)),
    pointer(services), buttonGroup(services)
{
    rtl = services.localizationManager.isRTL();
    
    size_t k = 0;
    for (auto& button : buttons)
    {
        button.initialize(services.inputManager);
        
        createCommonTextualButton(button, services, "ui-select-field.png", "ui-select-field.png",
            util::rect(16, 0, 8, 1), util::rect(0, 0, ScreenWidth - 2 * ButtonSpace, ButtonHeight),
            ButtonIdentifiers[k], ButtonCaptionSize, Colors::White, 1, Colors::Black, glm::vec2(24, 0));
        
        button.getPressedSprite()->setBlendColor(Colors::Yellow);
        button.getActiveSprite()->setOpacity(0.5);
        button.getActiveSprite()->setOpacity(0.5);
        button.setPosition(glm::vec2(ScreenWidth/2, ButtonTop + ButtonHeight/2 - (4-k) * (ButtonHeight + ButtonSpace)));
        button.setDepth(10);
        
        k++;
    }
    
    buttons[0].setPressAction([&,this]
    {
        if (this != getSceneManager().currentScene()) return;

        playConfirm(services);
        auto scene = new GameScene(services, SavedGame());
        scene->loadLevel("level1.lvl");
        getSceneManager().replaceSceneTransition(scene, 1s);
    });
    
    buttons[1].setPressAction([&,this]
    {
        if (this != getSceneManager().currentScene()) return;

        playConfirm(services);
        auto scene = new FileSelectScene(services, SavedGame(), FileSelectScene::FileAction::Load);
        getSceneManager().pushSceneTransition(scene, 1s);
    });
    
    buttons[2].setPressAction([&,this]
    {
        if (this != getSceneManager().currentScene()) return;

        playConfirm(services);
        auto scene = new SettingsScene(services);
        getSceneManager().replaceSceneTransition(scene, 1s);
    });
    
    buttons[3].setPressAction([&,this]
    {
        if (this != getSceneManager().currentScene()) return;
        playConfirm(services);
        getSceneManager().popScene();
    });
    
    buttonGroup.setButtons(buttons);
    buttonGroup.setPointer(pointer);
}

void TitleScene::update(FrameTime curTime)
{
}

void TitleScene::pause()
{
    for (auto& button : buttons) button.deactivate();
    buttonGroup.deactivate();
}

void TitleScene::resume()
{
    for (auto& button : buttons) button.activate();
    buttonGroup.activate();
}

void TitleScene::render(Renderer &renderer)
{
    renderer.pushDrawable(background, 0);
    for (auto& button : buttons) button.render(renderer);
    
    renderer.pushTransform();
    if (rtl)
    {
        renderer.currentTransform *= util::translate(ScreenWidth, 0);
        renderer.currentTransform *= util::scale(-1, 1);
    }
    renderer.pushDrawable(foreground, 20);
    renderer.popTransform();
    
    pointer.render(renderer);
}
