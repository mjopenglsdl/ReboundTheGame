#pragma once

#include <SFML/Window.hpp>
#include <unordered_map>
#include <functional>
#include "../utility/non_copyable_movable.hpp"
#include "../utility/ContainerEntry.hpp"

using Action = uintmax_t;
enum class ActionState { Down, Up };

struct KeyHash
{
    uintmax_t operator()(sf::Keyboard::Key key) const noexcept
    {
        return std::hash<uintmax_t>()(key);
    }
};

class InputManager final : util::non_copyable_movable
{
public:
    using CallbackFunction = std::function<void(ActionState)>;

private:
    std::unordered_map<sf::Keyboard::Key,Action,KeyHash> keyMappings;
    std::unordered_multimap<Action,CallbackFunction> mapCallbacks;
    std::unordered_multimap<sf::Keyboard::Key,CallbackFunction,KeyHash> rawCallbacks;

public:
    using CallbackEntry = util::ContainerEntry<decltype(mapCallbacks)>;
    using RawCallbackEntry = util::ContainerEntry<decltype(rawCallbacks)>;

    InputManager();
    ~InputManager();

    void handleKeyEvent(sf::Keyboard::Key key, ActionState state);
    void setMapping(sf::Keyboard::Key key, Action action);

    CallbackEntry registerCallback(Action action, CallbackFunction callback);
    RawCallbackEntry registerRawCallback(sf::Keyboard::Key key, CallbackFunction callback);
};

