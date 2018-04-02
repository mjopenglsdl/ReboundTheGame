#pragma once

#include "SettingsPanel.hpp"

#include "drawables/TextDrawable.hpp"
#include "ui/UIButton.hpp"
#include "ui/UIButtonGroup.hpp"
#include "ui/UIPointer.hpp"

#include "language/LangID.hpp"

struct Settings;
class ResourceManager;
class InputManager;
class LocalizationManager;

class LanguageSelectSettingsPanel final : public SettingsPanel
{
    TextDrawable title;
    std::vector<std::unique_ptr<UIButton>> languageButtons;
    UIButton backButton;
    UIButtonGroup buttonGroup;

public:
    LanguageSelectSettingsPanel(Settings& settings, InputManager& im, ResourceManager& rm, LocalizationManager &lm,
        SettingsBase* curSettings, UIPointer& pointer);
    virtual ~LanguageSelectSettingsPanel() = default;

    virtual void update(FrameTime curTime) override;
    virtual void render(Renderer& renderer) override;

    virtual void activate() override;
    virtual void deactivate() override;
};