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


#include "LanguageSelectSettingsPanel.hpp"

#include "SettingsScene.hpp"
#include "RootSettingsPanel.hpp"

#include "resources/ResourceManager.hpp"
#include "language/LocalizationManager.hpp"
#include "language/convenienceConfigText.hpp"
#include "rendering/Renderer.hpp"

#include "settings/Settings.hpp"
#include "input/InputSource.hpp"

#include "ui/UIButtonCommons.hpp"
#include "ColorList.hpp"

#include <defaults.hpp>

constexpr float FrameWidth = PlayfieldWidth;
constexpr float ButtonHeight = 32;
constexpr float ButtonSpace = 8;
constexpr float ButtonCaptionSize = 28;
constexpr float SmallButtonSpace = 4;
constexpr float SmallButtonHeight = 24;
constexpr float SmallCaptionSize = 20;
constexpr float TotalHeight = 472;

LanguageSelectSettingsPanel::LanguageSelectSettingsPanel(Services& services, SettingsBase* curSettings, UIPointer& pointer)
    : SettingsPanel(curSettings, pointer), backButton(services.inputManager), buttonGroup(services)
{
    auto pos = getCenterPosition() + glm::vec2(0, -TotalHeight/2 + ButtonHeight/2);

    title.setFontHandler(loadDefaultFont(services));
    title.setString(services.localizationManager.getString("settings-language-select-title"));
    title.setFontSize(ButtonCaptionSize);
    title.setDefaultColor(Colors::Yellow);
    title.setOutlineThickness(1);
    title.setDefaultOutlineColor(Colors::Black);
    title.setHorizontalAnchor(TextDrawable::HorAnchor::Center);
    title.setVerticalAnchor(TextDrawable::VertAnchor::Center);
    configTextDrawable(title, services.localizationManager);
    title.buildGeometry();

    pos += glm::vec2(0, ButtonHeight + ButtonSpace);

    for (std::string lang : getAllLanguageDescriptors())
    {
        languageButtons.emplace_back(std::make_unique<UIButton>(services.inputManager));
        auto& button = languageButtons.back();

        createCommonTextualButton(*button, services, "ui-select-field.png", "ui-select-field.png",
            util::rect(16, 0, 8, 1), util::rect(0, 0, FrameWidth - 2 * ButtonSpace, SmallButtonHeight),
            "", SmallCaptionSize, Colors::White, 1, Colors::Black, glm::vec2(24, 0),
            TextDrawable::Alignment::Direct);
        button->getCaption()->setString(getLanguageDescriptorName(lang));
        button->getCaption()->buildGeometry();

        button->setPosition(pos);

        button->getPressedSprite()->setBlendColor(Colors::Yellow);
        button->getActiveSprite()->setOpacity(0.5);
        button->getActiveSprite()->setOpacity(0.5);
        button->setDepth(5000);

        button->setPressAction([&, lang, curSettings = this->curSettings]
        {
            playConfirm(services);
            services.localizationManager.loadLanguageDescriptor(services.settings.languageFile = lang);
            curSettings->changeSettingsPanel(new RootSettingsPanel(services, curSettings, pointer));
        });

        pos += glm::vec2(0, SmallButtonHeight + SmallButtonSpace);
    }

    languageButtons.shrink_to_fit();

    createCommonTextualButton(backButton, services, "ui-select-field.png", "ui-select-field.png",
        util::rect(16, 0, 8, 1), util::rect(0, 0, FrameWidth - 2 * ButtonSpace, ButtonHeight),
        "settings-back-to-root", ButtonCaptionSize, Colors::White, 1, Colors::Black, glm::vec2(24, 0),
        TextDrawable::Alignment::Center);
    backButton.setPosition(getCenterPosition() + glm::vec2(0, TotalHeight/2 - ButtonHeight/2));

    backButton.getPressedSprite()->setBlendColor(Colors::Yellow);
    backButton.getActiveSprite()->setOpacity(0.5);
    backButton.getActiveSprite()->setOpacity(0.5);
    backButton.setDepth(5000);

    backButton.setPressAction([&, curSettings = this->curSettings]
    {
        playConfirm(services);
        curSettings->changeSettingsPanel(new RootSettingsPanel(services, curSettings, pointer));
    });

    std::vector<UIButton*> buttons;
    for (auto& button : languageButtons) buttons.push_back(button.get());
    buttons.push_back(&backButton);
    buttonGroup.setButtons(buttons);
    buttonGroup.setPointer(pointer);
}

void LanguageSelectSettingsPanel::update(FrameTime curTime)
{

}

void LanguageSelectSettingsPanel::render(Renderer &renderer)
{
    renderer.pushTransform();
    renderer.currentTransform *= util::translate(getCenterPosition());
    renderer.currentTransform *= util::translate(0, -TotalHeight/2 + ButtonHeight/2);
    renderer.pushDrawable(title, 5000);
    renderer.popTransform();

    for (auto& btn : languageButtons) btn->render(renderer);
    backButton.render(renderer);
}

void LanguageSelectSettingsPanel::activate()
{
    for (auto& btn : languageButtons) btn->activate();
    backButton.activate();
    buttonGroup.activate();
}

void LanguageSelectSettingsPanel::deactivate()
{
    for (auto& btn : languageButtons) btn->deactivate();
    backButton.deactivate();
    buttonGroup.deactivate();
}
