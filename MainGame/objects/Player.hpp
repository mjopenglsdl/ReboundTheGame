#pragma once

#include "objects/GameObject.hpp"
#include "drawables/Sprite.hpp"

#include <SFML/Graphics.hpp>
#include <cppmunk/Shape.h>
#include <cppmunk/Body.h>
#include <chrono>

#include <type_traits>

class PlayerController;
class ResourceManager;
class GameScene;
class Renderer;

template <typename T, typename = std::enable_if_t<std::is_default_constructible<T>::value>>
void reset(T& val) { val = T(); }

extern size_t global_AbilityLevel;

class Player final : public GameObject
{
    Sprite sprite;
    std::shared_ptr<Chipmunk::Shape> playerShape;
    
    float angle;
    bool wallJumpPressedBefore, doubleJumpConsumed;
    
    std::chrono::steady_clock::time_point
        wallJumpTriggerTime, dashTime, curTime;

    enum class DashDir { None, Left, Right, Up } dashDirection;

    size_t abilityLevel;
    size_t health, maxHealth;

public:
    Player(GameScene &scene);
    ~Player();

    void setupPhysics();

    virtual void update(std::chrono::steady_clock::time_point curTime) override;
    virtual void render(Renderer& renderer) override;

    auto getPosition() const { return playerShape->getBody()->getPosition(); }
    void setPosition(cpVect pos) { playerShape->getBody()->setPosition(pos); }

    auto getBody() const { return playerShape->getBody(); }

    auto getDisplayPosition() const
    {
        auto vec = getPosition();
        return sf::Vector2f(std::floor(vec.x), std::floor(vec.y));
    }

    auto getHealth() const { return health; }
    auto getMaxHealth() const { return maxHealth; }

    void heal(size_t amount);
    void damage(size_t amount);

    auto canPushCrates() const { return abilityLevel >= 2; }
    void upgradeToAbilityLevel(size_t level)
    {
        if (abilityLevel < level)
            abilityLevel = level;
        global_AbilityLevel = abilityLevel;
    }

    void jump();
    void decayJump();
    void wallJump();
    void dash();
    void lieBomb(std::chrono::steady_clock::time_point curTime);

    static constexpr cpCollisionType CollisionType = 'plyr';

#pragma pack(push, 1)
    struct ConfigStruct
    {
        sf::Vector2<int16_t> position;
    };
#pragma pack(pop)

    bool configure(const ConfigStruct& config);
};

