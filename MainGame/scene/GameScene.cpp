#include "GameScene.hpp"

#include "objects/Player.hpp"
#include "rendering/Renderer.hpp"
#include "resources/ResourceManager.hpp"
#include <chronoUtils.hpp>
#include <assert.hpp>
#include "data/RoomData.hpp"
#include "defaults.hpp"
#include "gameplay/MapGenerator.hpp"
#include "input/InputManager.hpp"

#include "SceneManager.hpp"
#include "scene/pause/PauseScene.hpp"

#include <functional>
#include <iterator>
#include <SFML/System.hpp>

#include <iostream>

#ifdef GENERATE_MAPS_IF_EMPTY
#include "settings/FileOutputStream.hpp"
#include <execDir.hpp>
#endif

template <typename T>
T clamp(T cur, T min, T max)
{
    return cur < min ? min : cur > max ? max : cur;
}

GameScene::GameScene(Settings& settings, InputManager& im, ResourceManager &rm, LocalizationManager& lm)
    : room(*this), resourceManager(rm), localizationManager(lm), inputManager(im), settings(settings),
    playerController(im, settings.inputSettings), objectsLoaded(false), curRoomID(-1), requestedID(-1),
    gui(*this), camera(*this), levelTransition(*this), pauseScreenRequested(false), pausing(false), pauseLag(0)
#if CP_DEBUG
, debug(gameSpace)
#endif
{
    gameSpace.setGravity(cpVect{0.0f, 1024.0f});
}

void GameScene::loadLevel(std::string levelName)
{
    levelData = resourceManager.load<LevelData>(levelName);
    
#ifdef GENERATE_MAPS_IF_EMPTY
    if (levelData->roomMaps.empty())
    {
        generateAllMapsForLevel(*levelData, resourceManager);
        
        FileOutputStream stream;
        ASSERT(stream.open(getExecutableDirectory() + "/Resources/" + levelName));
        ASSERT(writeToStream(stream, *levelData));
    }
#endif
    
    reloadLevel();
}
 
void GameScene::reloadLevel()
{
    levelPersistentData.clear();
    
    visibleMaps.clear();
    visibleMaps.resize(levelData->roomMaps.size());
    visibleMaps.at(levelData->startingRoom) = true;
    
    loadRoom(levelData->startingRoom);
    loadRoomObjects();
    
    gui.setLevelNumber(levelData->levelNumber);
    gui.setVisibleMaps(visibleMaps);
    
    reset(pauseLag);
}

void GameScene::loadRoom(size_t id, bool transition, cpVect displacement)
{
    curRoomID = id;
    auto roomName = levelData->roomResourceNames.at(id) + ".map";
    
    if (!transition)
    {
        gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const auto& obj) { return !obj->isPersistent; }), gameObjects.end());
    }
    else
    {
        room.clearTransition();
        gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const auto& obj) { return obj->transitionState; }), gameObjects.end());
        
        for (auto& obj : gameObjects)
        {
            bool shouldKeep = obj->notifyScreenTransition(displacement);
            if (!obj->isPersistent)
            {
                if (shouldKeep) obj->transitionState = true;
                else obj.reset(nullptr);
            }
        }
        
        gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const auto& obj) { return !obj; }), gameObjects.end());
            
    }
    
    currentRoomData = resourceManager.load<RoomData>(roomName);
    room.loadRoom(*currentRoomData, transition, displacement);
    visibleMaps.at(id) = true;
    objectsLoaded = false;
}

void GameScene::loadRoomObjects()
{
    if (objectsLoaded) return;
    
    for (const auto& descriptor : currentRoomData->gameObjectDescriptors)
    {
        auto obj = createObjectFromDescriptor(*this, descriptor);
        if (obj) gameObjects.push_back(std::move(obj));
    }

    objectsLoaded = true;
}

void GameScene::addObject(std::unique_ptr<GameObject> obj)
{
    if (!obj) return;
    objectsToAdd.push_back(std::move(obj));
}

GameObject* GameScene::getObjectByName(std::string str)
{
    for (const auto& obj : gameObjects)
        if (obj && obj->getName() == str)
            return obj.get();

    return nullptr;
}

std::vector<GameObject*> GameScene::getObjectsByName(std::string str)
{
    std::vector<GameObject*> objs;

    for (const auto& obj : gameObjects)
        if (obj && obj->getName() == str)
            objs.push_back(obj.get());

    return objs;
}

void GameScene::removeObjectsByName(std::string str)
{
    for (auto obj : getObjectsByName(str)) if (obj) obj->remove();
}

cpVect GameScene::wrapPosition(cpVect pos)
{
    cpFloat width = DefaultTileSize * currentRoomData->mainLayer.width();
    cpFloat height = DefaultTileSize * currentRoomData->mainLayer.height();
    
    pos.x -= width * std::floor(pos.x/width);
    pos.y -= height * std::floor(pos.y/height);

    return pos;
}

sf::Vector2f GameScene::fitIntoRoom(sf::Vector2f vec)
{
    vec.x = clamp<float>(vec.x, PlayfieldWidth/2,
        DefaultTileSize * currentRoomData->mainLayer.width() - PlayfieldWidth/2);
    vec.y = clamp<float>(vec.y, PlayfieldHeight/2,
        DefaultTileSize * currentRoomData->mainLayer.height() - PlayfieldHeight/2);
    return vec;
}

void GameScene::update(std::chrono::steady_clock::time_point curTime)
{
    this->curTime = curTime;
    
    if (pausing)
    {
        using namespace std::chrono;
        pauseLag += duration_cast<decltype(pauseLag)>(curTime.time_since_epoch());
        std::cout << pauseLag.count() << std::endl;
        pausing = false;
    }
    
    if (this == getSceneManager().currentScene() && pauseScreenRequested)
    {
        auto player = getObjectByName<Player>("player");
        auto scene = new PauseScene(settings, inputManager, resourceManager, localizationManager);
        scene->setMapLevelData(levelData, curRoomID, player->getDisplayPosition(), visibleMaps);
        getSceneManager().pushScene(scene);
        pauseScreenRequested = false;
        return;
    }
    
    playerController.update();
    gameSpace.step(toSeconds<cpFloat>(UpdateFrequency));

    room.update(curTime);

    {
        using namespace std::chrono;
        auto laggedTime = curTime - duration_cast<steady_clock::duration>(pauseLag);
        for (const auto& obj : gameObjects) obj->update(laggedTime);
    }

    gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
        [](const auto& obj) { return obj->shouldRemove; }), gameObjects.end());

    std::move(objectsToAdd.begin(), objectsToAdd.end(), std::back_inserter(gameObjects));
    objectsToAdd.clear();

    checkWarps();
    
    gui.update(curTime);
    camera.update(curTime);
    levelTransition.update(curTime);
    
    if (requestedID != -1)
    {
        loadRoom(requestedID);
        loadRoomObjects();
        requestedID = -1;
    }
}

void GameScene::notifyTransitionEnded()
{
    room.clearTransition();
    gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
        [](const auto& obj) { return obj->transitionState; }), gameObjects.end());
}

void GameScene::checkWarps()
{
    auto player = getObjectByName<Player>("player");

    if (player)
    {
        auto pos = player->getPosition();
        
        if (pos.x >= DefaultTileSize * currentRoomData->mainLayer.width())
        {
            camera.scheduleTransition(Camera::Direction::Right);
            checkWarp(player, WarpData::Dir::Right, pos);
        }
        else if (pos.y >= DefaultTileSize * currentRoomData->mainLayer.height())
        {
            camera.scheduleTransition(Camera::Direction::Down);
            checkWarp(player, WarpData::Dir::Down, pos);
        }
        else if (pos.x < 0)
        {
            camera.scheduleTransition(Camera::Direction::Left);
            checkWarp(player, WarpData::Dir::Left, pos);
        }
        else if (pos.y < 0)
        {
            camera.scheduleTransition(Camera::Direction::Up);
            checkWarp(player, WarpData::Dir::Up, pos);
        }
        if (!objectsLoaded) loadRoomObjects();
    }
}

void GameScene::checkWarp(Player* player, WarpData::Dir direction, cpVect pos)
{
    bool isHorizontal = direction == WarpData::Dir::Right || direction == WarpData::Dir::Left;
    auto c = isHorizontal ? pos.y : pos.x;

    WarpData warp = { 0, 0, (uint16_t)-1, 16383, WarpData::Dir::Up };
    for (const auto& curWarp : currentRoomData->warps)
    {
        if (curWarp.warpDir != direction) continue;
        if (c < curWarp.c1 || c >= curWarp.c2) continue;
        warp = curWarp;
    }

    if (warp.roomId != (uint16_t)-1)
    {
        cpVect displacement = { 0, 0 };
        auto roomName = levelData->roomResourceNames.at(warp.roomId) + ".map";
        auto destRoomData = resourceManager.load<RoomData>(roomName);
        auto delta = destRoomData->warps.at(warp.warpId).c1 - warp.c1;
        
        switch (direction)
        {
            case WarpData::Dir::Right:
                displacement.x = -(cpFloat)DefaultTileSize * currentRoomData->mainLayer.width();
                displacement.y = delta; break;
            case WarpData::Dir::Down:
                displacement.y = -(cpFloat)DefaultTileSize * currentRoomData->mainLayer.height();
                displacement.x = delta; break;
            case WarpData::Dir::Left:
                displacement.x = (cpFloat)DefaultTileSize * destRoomData->mainLayer.width();
                displacement.y = delta; break;
            case WarpData::Dir::Up:
                displacement.y = (cpFloat)DefaultTileSize * destRoomData->mainLayer.height();
                displacement.x = delta; break;
        }
        
        loadRoom(warp.roomId, true, displacement);
    }
}

void GameScene::render(Renderer& renderer)
{
    renderer.pushTransform();

    gui.render(renderer);
    levelTransition.render(renderer);
    
    renderer.currentTransform.translate(camera.getGlobalDisplacement());
    room.render(renderer, camera.transitionOccuring());
    for (const auto& obj : gameObjects) obj->render(renderer);

#if CP_DEBUG
    renderer.pushDrawable(debug, {}, 800);
#endif

    renderer.popTransform();
}

void GameScene::pause()
{
    using namespace std::chrono;
    
    pausing = true;
    pauseLag -= duration_cast<decltype(pauseLag)>(curTime.time_since_epoch());
}
