#include "UIButtonGroup.hpp"

#include "UIButton.hpp"
#include "UIPointer.hpp"

#include <functional>
#include <algorithm>
#include <cmath>

UIButtonGroup::UIButtonGroup(InputManager& inputManager, const InputSettings& settings, TravelingMode travelingMode)
    : currentId(-1), active(true), pointer(nullptr), previousTravelValue(0)
{
    select.registerSource(inputManager, settings.keyboardSettings.okInput, 0);
    select.registerSource(inputManager, settings.joystickSettings.okInput, 1);
    
    if (travelingMode == TravelingMode::Horizontal)
    {
        travel.registerButton(inputManager, settings.keyboardSettings.moveLeft,  AxisDirection::Negative, 0);
        travel.registerButton(inputManager, settings.keyboardSettings.moveRight, AxisDirection::Positive, 0);
        travel.registerAxis(inputManager, settings.joystickSettings.movementAxisX,    0);
        travel.registerAxis(inputManager, settings.joystickSettings.movementAxisXAlt, 1);
    }    
    else
    {
        travel.registerButton(inputManager, settings.keyboardSettings.moveUp,    AxisDirection::Negative, 0);
        travel.registerButton(inputManager, settings.keyboardSettings.moveDown,  AxisDirection::Positive, 0);
        travel.registerAxis(inputManager, settings.joystickSettings.movementAxisY,    0);
        travel.registerAxis(inputManager, settings.joystickSettings.movementAxisYAlt, 1);
    }
    
    select.setAction([=](bool pressed)
    {
        if (!active || buttons.empty()) return;
        if (currentId < buttons.size())
        {
            if (pointer) pointer->hide();
            
            if (pressed) getCurrentButton()->state = UIButton::State::Pressed;
            else
            {
                getCurrentButton()->state = UIButton::State::Active;
                if (getCurrentButton()->pressAction) getCurrentButton()->pressAction();
            }
        }
    });
    
    travel.setAction([=](float val)
    {
        if (pointer) pointer->hide();

        float prev = previousTravelValue;
        previousTravelValue = val;

        if (!active || (val > -0.5 && val < 0.5) || buttons.size() <= 1) return;
        if (currentId < buttons.size() && getCurrentButton()->state == UIButton::State::Pressed) return;
        
        if (val >= 0.5 && prev < 0.5) setCurrentId(currentId >= buttons.size() ? 0 : (currentId + 1) % buttons.size());
        else if (val <= -0.5 && prev > -0.5)
            setCurrentId(currentId >= buttons.size() ? buttons.size()-1 : (currentId + buttons.size() - 1) % buttons.size());
    });
}

void UIButtonGroup::setButtons(std::initializer_list<std::reference_wrapper<UIButton>> buttons)
{
    this->buttons.clear();
    for (auto& button : buttons)
    {
        this->buttons.push_back(&button.get());
        button.get().parentGroup = this;
    }
}

void UIButtonGroup::setButtons(UIButton* buttons, size_t size)
{
    this->buttons.clear();
    for (size_t i = 0; i < size; i++)
    {
        this->buttons.push_back(&buttons[i]);
        buttons[i].parentGroup = this;
    }
}

void UIButtonGroup::setButtons(const std::vector<UIButton*>& buttons)
{
    this->buttons = buttons;
    
    for (auto button : buttons)
        button->parentGroup = this;
}

void UIButtonGroup::setCurrentId(size_t id)
{
    if (currentId < buttons.size())
    {
        getCurrentButton()->state = UIButton::State::Normal;
        if (getCurrentButton()->outAction) getCurrentButton()->outAction();
    }
    currentId = id;
    if (currentId < buttons.size())
    {
        getCurrentButton()->state = UIButton::State::Active;
        if (getCurrentButton()->overAction) getCurrentButton()->overAction();
    }
}

void UIButtonGroup::setCurrentButton(UIButton* cur)
{
    setCurrentId(std::find(buttons.begin(), buttons.end(), cur) - buttons.begin());
}
