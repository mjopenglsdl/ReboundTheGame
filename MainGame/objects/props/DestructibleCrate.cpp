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


#include "DestructibleCrate.hpp"

#include "scene/GameScene.hpp"
#include "rendering/Renderer.hpp"
#include "resources/ResourceManager.hpp"
#include <vector_math.hpp>
#include "objects/Bomb.hpp"
#include "particles/TextureExplosion.hpp"

#include "objects/GameObjectFactory.hpp"

#include <functional>
#include <limits>
#include <cppmunk/Body.h>
#include <cppmunk/PolyShape.h>
#include <cppmunk/Space.h>
#include <cppmunk/Arbiter.h>

using namespace props;
using namespace cp;
using namespace std::literals::chrono_literals;

const util::rect FixedVelocityRect(-64, -192, 128, 128);
constexpr auto ExplosionDuration = 2s;
constexpr float CrateHalfSize = 48, CrateBevel = 16;

DestructibleCrate::DestructibleCrate(GameScene& gameScene, std::string texture, uint32_t type)
    : GameObject(gameScene), sprite(gameScene.getResourceManager().load<Texture>(texture))
{
    interactionHandler = [this,type] (uint32_t ty, void* ptr)
    {
        if (ty == type && isDestructionViable())
        {
            explode(ptr);
            remove();
        }
    };
}

bool DestructibleCrate::configure(const DestructibleCrate::ConfigStruct& config)
{
    belongingRoomID = gameScene.getCurrentRoomID();
    
    if (gameScene.getLevelPersistentData().existsDataOfType<bool>(getDestroyedKey()) &&
        gameScene.getLevelPersistentData().getData<bool>(getDestroyedKey()))
        return false;

    setupPhysics();
    setPosition(cpVect{(cpFloat)config.position.x, (cpFloat)config.position.y});

    return true;
}

void DestructibleCrate::setupPhysics()
{
    auto body = std::make_shared<Body>(Body::Kinematic);

    constexpr auto Offset = CrateHalfSize - CrateBevel;
    shape = std::make_shared<PolyShape>(body, std::vector<cpVect>
        { cpVect{-Offset, -Offset}, cpVect{+Offset, -Offset},
          cpVect{+Offset, +Offset}, cpVect{-Offset, +Offset} }, CrateBevel);
    
    shape->setDensity(1);
    shape->setElasticity(0.6);
    shape->setCollisionType(Interactable);
    shape->setUserData(&interactionHandler);

    gameScene.getGameSpace().add(body);
    gameScene.getGameSpace().add(shape);

    body->setUserData((void*)this);
}

void DestructibleCrate::explode(void* ptr)
{
    explode(FixedVelocityRect);
}

void BombCrate::explode(void* ptr)
{
    auto bomb = static_cast<Bomb*>(ptr);
    auto velPtr = getDisplayPosition() - bomb->getDisplayPosition();
    auto dirPtr = normalize(velPtr) * 128.0f;

    auto rect = FixedVelocityRect;
    rect.x += dirPtr.x;
    rect.y += dirPtr.y;
    DestructibleCrate::explode(rect);
}

void DashCrate::explode(void* ptr)
{
    auto rect = FixedVelocityRect;
    auto player = static_cast<Player*>(ptr);

    auto ppos = player->getDisplayPosition(), pos = getDisplayPosition();
    if (ppos.x - pos.x < -0.5 * (PlayerRadius + CrateHalfSize)) rect.x += 256;
    else if (ppos.x - pos.x > 0.5 * (PlayerRadius + CrateHalfSize)) rect.x -= 256;
    else if (ppos.y - pos.y > 0.5 * (PlayerRadius + CrateHalfSize)) rect.y -= 256;

    DestructibleCrate::explode(rect);
}

void DestructibleCrate::explode(util::rect velocityRect)
{
    auto grav = gameScene.getGameSpace().getGravity();
    auto displayGravity = glm::vec2(grav.x, grav.y);
    
    auto explosion = std::make_unique<TextureExplosion>(gameScene, sprite.getTexture(), ExplosionDuration,
        velocityRect, displayGravity, TextureExplosion::Density, 8, 8, 25);
    explosion->setPosition(getDisplayPosition());
    gameScene.addObject(std::move(explosion));
    
    gameScene.getLevelPersistentData().setData(getDestroyedKey(), true);
}

DestructibleCrate::~DestructibleCrate()
{
    if (shape)
    {
        auto body = shape->getBody();
        gameScene.getGameSpace().remove(shape);
        gameScene.getGameSpace().remove(body);
    }
}

void DestructibleCrate::update(FrameTime curTime)
{

}

void DestructibleCrate::render(Renderer& renderer)
{
    renderer.pushTransform();
    renderer.currentTransform *= util::translate(getDisplayPosition());
    renderer.pushDrawable(sprite, 25);
    renderer.popTransform();
}

bool DestructibleCrate::notifyScreenTransition(cpVect displacement)
{
    shape->getBody()->setPosition(shape->getBody()->getPosition() + displacement);
    return true;
}

bool DashCrate::isDestructionViable() const
{
    auto player = gameScene.getObjectByName<Player>("player");
    return player && player->canBreakDash();
}

REGISTER_GAME_OBJECT(props::BombCrate);
REGISTER_GAME_OBJECT(props::DashCrate);
