#include <iostream>
#include <chronoUtils.hpp>
#include <thread>
#include <utility>
#include <clocale>
#include <SFML/Graphics.hpp>

#include <grid.hpp>
#include <assert.hpp>
#include "input/InputManager.hpp"
#include "input/InputPlayerController.hpp"
#include "scene/Scene.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/FilesystemResourceLocator.hpp"
#include "drawables/Tilemap.hpp"
#include "scene/SceneManager.hpp"
#include "scene/GameScene.hpp"
#include "objects/Player.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/WindowHandler.hpp"
#include "settings/Settings.hpp"
#include "language/LocalizationManager.hpp"
#include "language/LanguageDescriptor.hpp"
#include "language/KeyboardKeyName.hpp"

#include "scene/TitleScene.hpp"

#include <chronoUtils.hpp>

#define DEBUG_STEADY 0

using namespace std::literals::chrono_literals;

bool clearMapTextures();
bool GlobalUpdateWindowHandler;
int main(int argc, char **argv)
{
    auto locale = std::setlocale(LC_ALL, "");

    bool success;
    auto settings = loadSettingsFile(&success);
    if (!success)
    {
        std::cout << "WARNING! Settings file not loaded properly!" << std::endl;
        settings.languageFile = languageDescriptorForLocale(locale);
    }
    
    GlobalUpdateWindowHandler = false;
    WindowHandler windowHandler(settings.videoSettings.fullscreen, settings.videoSettings.vsyncEnabled);

    InputManager inputManager;

    ResourceManager resourceManager;
    resourceManager.setResourceLocator(new FilesystemResourceLocator());
    
    LocalizationManager localizationManager(true);
    localizationManager.loadLanguageDescriptor(settings.languageFile);
    
    auto scene = new TitleScene(settings, inputManager, resourceManager, localizationManager);
    
    auto updateTime = FrameClock::now();
    
    SceneManager sceneManager;
    sceneManager.pushScene(scene);

    while (windowHandler.getWindow().isOpen())
    {
        sf::Event event;

        while (windowHandler.getWindow().pollEvent(event))
        {
            if (inputManager.handleEvent(event)) continue;
            switch (event.type)
            {
                case sf::Event::EventType::Closed:
                    windowHandler.getWindow().close();
                    break;
                default: break;
            }
        }

        auto curTime = FrameClock::now();

        while (curTime <= updateTime)
        {
            std::this_thread::sleep_until(updateTime);
            curTime = FrameClock::now();
        }

        while (curTime > updateTime)
        {
            updateTime += UpdatePeriod;
            sceneManager.update(updateTime);
#if DEBUG_STEADY
            break;
#endif
        }
        
        if (!sceneManager.hasScenes())
        {
            windowHandler.getWindow().close();
            break;
        }

        if (GlobalUpdateWindowHandler)
        {
            windowHandler.setFullscreen(settings.videoSettings.fullscreen);
            windowHandler.setVsyncEnabled(settings.videoSettings.vsyncEnabled);
            GlobalUpdateWindowHandler = false;
        }

        windowHandler.prepareForDraw();
        sceneManager.render(windowHandler.getRenderer());
        windowHandler.display();
    }

    clearMapTextures();

    if (!storeSettingsFile(settings))
        std::cout << "WARNING! Settings file not stored properly!" << std::endl;

    return 0;
}
