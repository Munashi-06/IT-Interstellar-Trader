#pragma once
#include "Core/World.hpp"
#include "Core/Settings.hpp"

#include "Interface/RadarUI.hpp"
#include "Interface/ShipMenuUI.hpp"
#include "Interface/TradeMenuUI.hpp"
#include "Interface/BackgroundStars.hpp"
#include "Interface/IntroAnimation.hpp"
#include "Interface/UpgradeTreeUI.hpp"
#include "Interface/DebugMenuUI.hpp"
#include "Interface/PirateEncounter.hpp"
#include "Interface/GameIntroAnimation.hpp"

#include "Systems/SaveSystem.hpp"
#include "Systems/UpgradeManager.hpp"
#include "Systems/AssetManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <iostream>
#include <cmath>


namespace Game {

    class Engine {
    private:
        // --- CORE WINDOW & TIMING ---
        sf::RenderWindow window;
        sf::Clock clock;
        sf::Clock worldClock;
        float dt;

        // --- GAME STATE & CONFIG ---
        State currentState;
        GameConfig config;

        // --- SYSTEMS & ENTITIES ---
        AudioManager audio;
        std::unique_ptr<World> world;
        Player spaceShip;
        UpgradeManager upgrades;
        BackgroundStars bgStars;
        Interface::PirateEncounter pirates;

        // --- UI MENUS & ANIMATIONS ---
        std::unique_ptr<Menu> mainMenu;
        std::unique_ptr<Settings> settingsMenu;
        std::unique_ptr<RadarUI> radarUI;
        std::unique_ptr<ShipMenuUI> shipMenu;
        std::unique_ptr<TradeMenuUI> tradeMenu;
        std::unique_ptr<UpgradeTreeUI> upgradeTree;
        std::unique_ptr<DebugMenuUI> debugMenu;
        Interface::IntroAnimation intro;
        Interface::GameIntroAnimation gameIntro;

        sf::Font& mainFont = AssetManager::getFont("main_font");

        // --- ASSETS & VARIABLES ---
        sf::Sprite backgroundSprite{AssetManager::getTexture("bg_main")};
        sf::Sprite settingsBackgroundSprite{AssetManager::getTexture("bg_settings")};
        sf::Sprite alertSprite{AssetManager::getTexture("alert_icon")};

        sf::Text planetNameText{mainFont};
        sf::Text animBtnText{mainFont, "REGRESAR A CASA"};
        sf::Text adminShipText{mainFont, "SPACE SHIP"};
        sf::Text confirmText{mainFont, ""};
        sf::Text optionsText{mainFont, "[Y] YES - LAND    [N] NO - CANCEL"};
        sf::Text shipMenuTitle{mainFont, "SHIP'S STATE"};
        sf::Text upgradeText{mainFont, "UPGRADE"};
        sf::RectangleShape generalBackground, animBtn, adminShipBtn, confirmBg, shipMenuBg, upgradeBtn;

        int selectedPlanetIndex;
        sf::Vector2f targetPosition;
        float travelSpeed;
        float shipAnimX;
        float alertTimer;
        bool pirateEncounterActive;
        bool auxRadar;
        std::string currentAudioContext;

        void processEvents();
        void update();
        void render();
        void executeAction(const std::string& option);
    public:
        Engine();
        bool init();
        void run();
    };

}