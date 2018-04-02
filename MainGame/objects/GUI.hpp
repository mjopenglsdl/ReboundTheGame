#pragma once

#include "drawables/Sprite.hpp"
#include "drawables/GUIMeter.hpp"
#include "drawables/TextDrawable.hpp"
#include "objects/Player.hpp"
#include <chronoUtils.hpp>
#include "drawables/GUIMap.hpp"

#include "language/LocalizationManager.hpp"

class GameScene;
class Renderer;

class GUIBossUpdater
{
public:
    virtual size_t getCurrentHealth() const = 0;
    virtual size_t getMaxHealth() const = 0;
};

GUIBossUpdater* nullBossMeter();

class GUI final
{
    GameScene& gameScene;
    
    Sprite guiLeft, guiRight;
    GUIMeter playerMeter, dashMeter, bossMeter;
    Sprite bombSprites[MaxBombs];
    TextDrawable levelLabel, levelID;
    size_t levelNumber;
    bool drawDash;

    const GUIBossUpdater* currentBoss;
    GUIMap guiMap;
    std::string lastIconName;
    
    void configureText();
    
    FrameTime lastTime;
    float healthBlinkPhase;

    LocalizationManager::CallbackEntry callbackEntry;
    
public:
    GUI(GameScene& scene);
    ~GUI() {}

    void update(FrameTime curTime);
    void render(Renderer& renderer);

    void setLevelNumber(size_t number);
    void setVisibleMaps(const std::vector<bool>& visibleMaps);
    void resetBlinker();
    
    auto getCurrentBoss() const { return currentBoss; }
    void setCurrentBoss(const GUIBossUpdater* boss) { currentBoss = boss; }
};
