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

#include <iostream>
#include <optional>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath> 

GameConfig mainConfig;

void ejecuteAction(std::string option, State& state, sf::RenderWindow& window) {
    if (option == "NEW GAME") {
        state = State::Playing;
    }
    else if(option == "CONTINUE") {
        state = State::Playing;
    }
    else if (option == "SETTINGS") {
        state = State::Options;
    }
    else if (option == "EXIT") {
        window.close();
    }
}

int main() {
    bool state = false;
    mainConfig.loadFromSavedFile("config.txt");
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "IT: Interstellar Trader");
    sf::Clock worldClock;
    
    AudioManager audio;
    audio.loadMusic("assets/audio/undertale_dogsong.ogg");
    audio.loadTheme("assets/audio/theme.ogg");
    audio.loadSFX("assets/audio/hover_sound.ogg", "assets/audio/option_selection_sound.ogg");
    audio.updateVolumesFromConfig(mainConfig.musicVolume, mainConfig.sfxVolume);
    audio.playMusic();

    Interface::PirateEncounter pirates;
    pirates.loadAssets();

    if (mainConfig.vsync){
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(0); // Deactivate FPS limit for VSync to control refresh rate
    }
    else{
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(mainConfig.fpsLimit);
    }

    sf::Font font;
    if(!font.openFromFile("assets/fonts/04B_03__.TTF")) {
        std::cerr << "Error loading source" << std::endl;
    }

    Menu mainMenu(1280.f, 720.f);
    Settings settingsMenu(1280.f, 720.f, font);
    settingsMenu.resetTempConfig(mainConfig);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/mainMenu_background.jpeg")) {
        std::cerr << "Error loading background image" << std::endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    
    sf::RectangleShape generalBackground;
    generalBackground.setSize({ 1280.f, 720.f });
    generalBackground.setFillColor(sf::Color(0, 0, 15)); // We darken the background scene to make the UI elements more visible

    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = 1280 / static_cast<float>(textureSize.x);
    float scaleY = 720 / static_cast<float>(textureSize.y);
    backgroundSprite.setScale({scaleX, scaleY});

    sf::Texture settingsBackgroundTexture;
    if (!settingsBackgroundTexture.loadFromFile("assets/settingsMenu_background.png")) {
        std::cerr << "Error loading background image from settings" << std::endl;
    }
    sf::Sprite settingsBackgroundSprite(settingsBackgroundTexture);

    State currentState = State::Menu; // Game's initial state is the Main Menu

    // The player controls a spaceship, which is initialized at the center of the screen with a specific texture
    Player spaceShip(640.f + 132.f, 360.f, "assets/player.png");

    sf::Clock clock; // To measure the time between frames

    sf::Vector2f mousePos; // To store the mouse position in world coordinates, useful for interactions with the menu and the game world

    float alertTimer = 0.f; 
    bool pirateEncounterActive = false; // NUEVO: Para controlar el encuentro con piratas
    
    // Priority Radar with the same charged source
    RadarUI radarUI(font);
#pragma region "test"

    // We load the base vector of planets
    std::vector<Planet> basePlanets = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets.txt");

    if (basePlanets.empty()) {
        std::cout << "[ERROR] No planets were loaded." << std::endl;
        return -1; 
    }

    // We created a heap
    auto radar = std::make_unique<Heap>(Planet(basePlanets[0]));

    // We fill the heap with the rest (including copies)
    for (size_t i = 1; i < basePlanets.size(); ++i) {
        radar->insert(Planet(basePlanets[i]), radar->getHeapArray(), cmp);
    }

    // We initialize the world with EVERYTHING
    // We use std::move to pass the ownership of the objects to the World
    World world(0.0f, std::move(radar), std::move(basePlanets));

    // Generate initial stock for all planets
    // for (auto& planet : world.getPlanets()) {
    //     planet.refreshMarket(world.getCatalog());
    // }
    
    bool showRadar = false;
    ShipMenuUI shipMenu(font, spaceShip.getShipTexture()); // Ship Menu
    TradeMenuUI tradeMenu(font); // Trade Menu

    UpgradeManager upgrades;
    upgrades.initTrees(spaceShip);
    UpgradeTreeUI upgradeTree(font);

#pragma endregion
    
    sf::Texture alertTexture;
    if (!alertTexture.loadFromFile("assets/alert_icon.png")) {
        std::cerr << "Error cargando la textura de alerta" << std::endl;
    }
    sf::Sprite alertSprite(alertTexture);
    alertSprite.setPosition({1100.f, 600.f}); 

    DebugMenuUI debugMenu(font);
    debugMenu.initCatalog(world.getCatalog());

    sf::Text planetNameText(font, "");
    planetNameText.setCharacterSize(22);
    planetNameText.setFillColor(sf::Color::White);
    planetNameText.setOutlineColor(sf::Color::Black);
    planetNameText.setOutlineThickness(1);
    planetNameText.setPosition({ 30.f, 660.f });

    sf::RectangleShape animBtn({200.f, 50.f});
    animBtn.setFillColor(sf::Color(100, 0, 150, 200)); 
    animBtn.setOutlineThickness(2);
    animBtn.setOutlineColor(sf::Color::White);
    animBtn.setPosition({ 1050.f, 130.f });

    sf::Text animBtnText(font, "REGRESAR A CASA");
    animBtnText.setCharacterSize(16);
    animBtnText.setFillColor(sf::Color::White);
    sf::FloatRect animTextRect = animBtnText.getLocalBounds();
    animBtnText.setOrigin({animTextRect.size.x / 2.f, animTextRect.size.y / 2.f});
    animBtnText.setPosition({ 1150.f, 150.f });

    Interface::IntroAnimation intro(1280.f, 720.f);
    if (!intro.loadAssets(font)) {
        std::cerr << "Error loading animation assets" << std::endl;
    }

    // NUEVO: Inicializar GameIntroAnimation
    Interface::GameIntroAnimation gameIntro(1280.f, 720.f);
    if (!gameIntro.loadAssets(font)) {
        std::cerr << "Error loading game intro assets" << std::endl;
    }

    sf::RectangleShape adminShipBtn({200.f, 50.f});
    adminShipBtn.setFillColor(sf::Color(50, 50, 50, 200));
    adminShipBtn.setOutlineThickness(2);
    adminShipBtn.setOutlineColor(sf::Color::White);
    adminShipBtn.setPosition({ 1050.f, 60.f });

    sf::Text adminShipText(font, "SPACE SHIP");
    adminShipText.setCharacterSize(16);
    adminShipText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = adminShipText.getLocalBounds();
    adminShipText.setOrigin({textRect.size.x / 2.f, textRect.size.y / 2.f});
    adminShipText.setPosition({ 1150.f, 80.f });

    sf::RectangleShape confirmBg({400.f, 200.f});
    confirmBg.setFillColor(sf::Color(20, 20, 20, 240)); 
    confirmBg.setOutlineThickness(3);
    confirmBg.setOutlineColor(sf::Color::Cyan);
    confirmBg.setOrigin({200.f, 100.f});
    confirmBg.setPosition({640.f, 360.f}); 

    sf::Text confirmText(font, "");
    confirmText.setCharacterSize(20);
    confirmText.setFillColor(sf::Color::White);

    sf::Text optionsText(font, "[Y] YES - LAND    [N] NO - CANCEL");
    optionsText.setCharacterSize(18);
    optionsText.setFillColor(sf::Color::Yellow);
    optionsText.setOrigin({optionsText.getLocalBounds().size.x / 2.f, 0.f});
    optionsText.setPosition({640.f, 400.f});

    sf::RectangleShape shipMenuBg({640.f, 400.f});
    shipMenuBg.setFillColor(sf::Color(15, 15, 25, 180));
    shipMenuBg.setOutlineThickness(3);
    shipMenuBg.setOutlineColor(sf::Color::Cyan);
    shipMenuBg.setOrigin({320.f, 200.f});
    shipMenuBg.setPosition({640.f, 360.f});

    sf::Text shipMenuTitle(font, "SHIP'S STATE");
    shipMenuTitle.setCharacterSize(25);
    shipMenuTitle.setFillColor(sf::Color::Cyan);
    shipMenuTitle.setPosition({480.f, 180.f});

    sf::Sprite shipPreview(spaceShip.getShipTexture()); 
    shipPreview.setScale({2.f, 2.f});
    shipPreview.setPosition({210.f, 120.f});

    sf::RectangleShape upgradeBtn({200.f, 40.f});
    upgradeBtn.setFillColor(sf::Color(0, 150, 0));
    upgradeBtn.setPosition({650.f, 320.f});

    sf::Text upgradeText(font, "UPGRADE");
    upgradeText.setCharacterSize(18);
    sf::FloatRect upRect = upgradeText.getLocalBounds();
    upgradeText.setOrigin({upRect.size.x / 2.f, upRect.size.y / 2.f});
    upgradeText.setPosition({750.f, 340.f}); 

    int selectedPlanetIndex = 2;
    sf::Vector2f targetPosition(640.f + 132.f, 360.f); 
    float travelSpeed = 400.f;
    float shipAnimX = -100.f;

    bool aux = true;
    BackgroundStars bgStars;
    std::string currentAudioContext = "menu";

    while (window.isOpen()) {
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float dt = clock.restart().asSeconds(); 
        world.setDeltaTime(dt);

        // --- CONTROL DE AUDIO SEGÚN EL ESTADO ---
        // MENU: Menu y Options suenan undertale
        if (currentState == State::Menu || currentState == State::Options) {
            // Solo cambiar si no estamos ya en contexto menu o si la música no está sonando
            if (currentAudioContext != "menu") {
                audio.stopTheme();
                // Recargar la música del menú explícitamente
                audio.loadMusic("assets/audio/undertale_dogsong.ogg");
                audio.playMusic();
                currentAudioContext = "menu";
                std::cout << "[AUDIO] Cambiando a música del menú (undertale)" << std::endl;
            } else {
                // Si ya estamos en contexto menu, asegurar que la música siga sonando
                // (por si acaso se detuvo por algún motivo)
                if (!audio.isMusicPlaying()) {
                    audio.playMusic();
                }
            }
        }
        // ANIMACION: sin música
        else if (currentState == State::Animation1) {
            if (currentAudioContext != "none") {
                audio.stopMusic();
                audio.stopTheme();
                currentAudioContext = "none";
                std::cout << "[AUDIO] Silenciando durante animación" << std::endl;
            }
        }
        //GameIntro - sin música
        // else if (currentState == State::GameIntro) {
        //     if (currentAudioContext != "none") {
        //         audio.stopMusic();
        //         audio.stopTheme();
        //         currentAudioContext = "none";
        //         std::cout << "[AUDIO] Silenciando durante game intro" << std::endl;
        //     }
        // }
        //GameIntro - NO detener nada, dejar que suene su propia música
        else if (currentState == State::GameIntro) {
            // No detener ninguna música, solo cambiar el contexto
            if (currentAudioContext != "gameintro") {

                currentAudioContext = "gameintro";
                std::cout << "[AUDIO] Modo Game Intro - música manejada por GameIntroAnimation" << std::endl;
            }
        }
        // PIRATA ENCOUNTER: sin música
        else if (currentState == State::PirateEncounter) {
            if (currentAudioContext != "none") {
                audio.stopMusic();
                audio.stopTheme();
                currentAudioContext = "none";
                std::cout << "[AUDIO] Silenciando durante encuentro con piratas" << std::endl;
            }
        }
        // GAMEPLAY: todos los estados del juego suenan theme
        else if (currentState == State::Playing || currentState == State::InPlanet || 
                 currentState == State::TravelConfirmation || currentState == State::ShipMenu ||
                 currentState == State::TradeMenu || currentState == State::UpgradeTree) {
            if (currentAudioContext != "gameplay") {
                audio.stopMusic();
                audio.loadTheme("assets/audio/theme.ogg");
                audio.playTheme();
                currentAudioContext = "gameplay";
                std::cout << "[AUDIO] Cambiando a theme del juego" << std::endl;
            }
        }

        // --- EVENT LOGIC ---
        if (currentState == State::Playing) {
            if (world.update(spaceShip)) {
                alertTimer = 3.0f; // The alert will last 3 seconds
                if(spaceShip.gethasAdvancedRadar()) {
                    radarUI.update(world.getRadar()->getHeapArray());
                }
            }
            if (alertTimer > 0) alertTimer -= dt;

            if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                adminShipBtn.setFillColor(sf::Color(80, 80, 80, 255));
                adminShipBtn.setOutlineColor(sf::Color::Cyan);
            } 
            else {
                adminShipBtn.setFillColor(sf::Color(50, 50, 50, 200));
                adminShipBtn.setOutlineColor(sf::Color::White);
            }
            if(spaceShip.getHasWarpDrive()) {
                    if (animBtn.getGlobalBounds().contains(mousePos)) { ///
                        animBtn.setFillColor(sf::Color(150, 0, 200, 255));
                        animBtn.setOutlineColor(sf::Color::Cyan);
                    }
                    else {
                        animBtn.setFillColor(sf::Color(100, 0, 150, 200));
                        animBtn.setOutlineColor(sf::Color::White);
                    }
                }

            }

        // --- INPUT LOGIC ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == State::Menu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Support for Arrow Keys + WASD
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                        mainMenu.moveUp();
                        audio.playHover();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                        mainMenu.moveDown();
                        audio.playHover();
                    }

                    // Confirm with Enter or Space
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        std::string opt = mainMenu.getSelectedOption();
                        
                        // If the user clicks CONTINUE but there is no saved data, we ignore it
                        if (opt == "CONTINUE" && !SaveSystem::saveExists()) {
                            // audio.playError();
                            continue; 
                        }

                        ejecuteAction(opt, currentState, window);
                        
                        if (opt == "NEW GAME") {
                            SaveSystem::setGameSeed("ALFA-77"); // Default seed
                            spaceShip.resetToDefaults();
                            upgrades.resetTrees();

                            for (auto& planet : world.getPlanets()) {
                                planet.refreshMarket(world.getCatalog());
                            }
                            SaveSystem::saveGame(spaceShip, upgrades); // Autosave on startup
                        }

                        else if (opt == "CONTINUE") {
                            SaveData data;
                            if (SaveSystem::loadGame(data)) {
                                SaveSystem::setGameSeed(data.seedText); 
                                spaceShip.setMoney(data.money);
                                spaceShip.setCurrentOrbit(data.currentOrbit);
                                
                                // Restore inventory
                                spaceShip.getInventory().clearAll();
                                for (const auto& pair : data.inventory) {
                                    const auto& itemData = world.getCatalog().at(pair.first);
                                    spaceShip.getInventory().addItem(pair.first, pair.second, itemData->getMaxStackSize(), itemData->getBasePrice());
                                }

                                // We turn off the tree first, and then turn on only the saved ones
                                upgrades.resetTrees(); 
                                upgrades.loadPurchasedUpgrades(data.purchasedUpgrades); 

                                for (auto& planet : world.getPlanets()) {
                                    planet.refreshMarket(world.getCatalog());
                                }
                            }
                        }
                        
                        audio.playClick();
                    }
                }
                else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2f mPos = window.mapPixelToCoords(mouseMoved->position);
                    if(mainMenu.updateHover(mPos)){
                        audio.playHover();
                    }
                }
                else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        std::string opt = mainMenu.getSelectedOption();
                        
                        // If the user clicks CONTINUE but there is no saved data, we ignore it
                        if (opt == "CONTINUE" && !SaveSystem::saveExists()) {
                            // audio.playError();
                            continue; 
                        }

                        ejecuteAction(opt, currentState, window);
                        
                        if (opt == "NEW GAME") {
                            SaveSystem::setGameSeed("ALFA-77"); // Default seed
                            spaceShip.resetToDefaults();
                            upgrades.resetTrees();

                            for (auto& planet : world.getPlanets()) {
                                planet.refreshMarket(world.getCatalog());
                            }
                            SaveSystem::saveGame(spaceShip, upgrades); // Autosave on startup
                        }
                        else if (opt == "CONTINUE") {
                            SaveData data;
                            if (SaveSystem::loadGame(data)) {
                                SaveSystem::setGameSeed(data.seedText); 
                                spaceShip.setMoney(data.money);
                                spaceShip.setCurrentOrbit(data.currentOrbit);
                                
                                // Restore inventory
                                spaceShip.getInventory().clearAll();
                                for (const auto& pair : data.inventory) {
                                    const auto& itemData = world.getCatalog().at(pair.first);
                                    spaceShip.getInventory().addItem(pair.first, pair.second, itemData->getMaxStackSize(), itemData->getBasePrice());
                                }

                                // We turn off the tree first, and then turn on only the saved ones
                                upgrades.resetTrees(); 
                                upgrades.loadPurchasedUpgrades(data.purchasedUpgrades); 

                                for (auto& planet : world.getPlanets()) {
                                    planet.refreshMarket(world.getCatalog());
                                }
                            }
                        }
                        
                        audio.playClick();
                    }
                }
            }
            else if (currentState == State::Options){
                if (event->is<sf::Event::Closed>()) window.close();
                
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                    if(keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up){
                        settingsMenu.moveUp();
                        audio.playHover();
                    }
                    else if(keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down){
                        settingsMenu.moveDown();
                        audio.playHover();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Escape){
                        settingsMenu.resetTempConfig(mainConfig);
                        currentState = State::Menu;
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space){
                        settingsMenu.handleAction(currentState, window, mainConfig, audio);
                        audio.playClick();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::D || keyPressed->code == sf::Keyboard::Key::Right){
                        settingsMenu.changeValue(1);
                        audio.playHover();
                        audio.setMusicVolume((float)settingsMenu.getTempMusicVolume());
                        audio.setSFXVolume((float)settingsMenu.getTempSfxVolume());
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::A || keyPressed->code == sf::Keyboard::Key::Left){
                        settingsMenu.changeValue(-1);
                        audio.playHover();
                        audio.setMusicVolume((float)settingsMenu.getTempMusicVolume());
                        audio.setSFXVolume((float)settingsMenu.getTempSfxVolume());
                    }
                }
                else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseEvent->button == sf::Mouse::Button::Left){
                        sf::Vector2f mPos = window.mapPixelToCoords(mouseEvent->position);
                        audio.playClick();
                        settingsMenu.handleMouseClick(mPos);
                        std::string opt = settingsMenu.getSelectedOption();
                        if(opt == "APPLY"){
                            settingsMenu.applySettings(window, mainConfig, audio);
                        }
                        else if(opt == "BACK"){
                            settingsMenu.resetTempConfig(mainConfig);
                            currentState = State::Menu;
                        }
                    }
                }
                else if (event->is<sf::Event::MouseButtonReleased>()){
                    settingsMenu.releaseSlider();
                }
                else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()){
                    sf::Vector2f mPos = window.mapPixelToCoords(mouseMoved->position);
                    if(settingsMenu.updateHover(mPos)){
                        audio.playHover();
                    }
                    settingsMenu.handleMouseMove(mPos);

                    audio.setMusicVolume((float)settingsMenu.getTempMusicVolume());
                    audio.setSFXVolume((float)settingsMenu.getTempSfxVolume());
                }
            }
            else if (currentState == State::GameIntro) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    gameIntro.handleInput(keyPressed->code);
                }
            }
            else if (currentState == State::Playing) {
                
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                            audio.playClick();
                            currentState = State::ShipMenu; 
                        }
                        else if(spaceShip.getHasWarpDrive()) {
                            if (animBtn.getGlobalBounds().contains(mousePos)) {
                                audio.playClick();
                                intro.reset();
                                currentState = State::Animation1; 
                            }
                        }
                    }
                }

                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }
                    else if(keyPressed->code == sf::Keyboard::Key::G || keyPressed->code == sf::Keyboard::Key::S) {
                        SaveSystem::saveGame(spaceShip, upgrades);
                    }
                    
                    const auto& planetas = world.getPlanets();
                    if (!planetas.empty()) {
                        if (keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::D ||
                            keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                            
                            selectedPlanetIndex = (selectedPlanetIndex + 1) % planetas.size(); // Movemos el cursor
                            audio.playHover();
                            
                            short targetOrbit = planetas[selectedPlanetIndex].getOrbit();
                            bool engineOk = (targetOrbit >= spaceShip.getMinOrbit() && targetOrbit <= spaceShip.getMaxOrbit());
                            bool hullOk = (targetOrbit >= spaceShip.getMinOrbitReach() && targetOrbit <= spaceShip.getMaxOrbitReach());
                            
                            if (spaceShip.getHasWarpDrive() || (engineOk && hullOk)) {
                                spaceShip.setCurrentOrbit(targetOrbit); 
                            }
                        }
                        else if (keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::A ||
                                keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                            
                            selectedPlanetIndex = (selectedPlanetIndex - 1 + (int)planetas.size()) % planetas.size(); // Movemos el cursor
                            audio.playHover();
                            
                            short targetOrbit = planetas[selectedPlanetIndex].getOrbit();
                            bool engineOk = (targetOrbit >= spaceShip.getMinOrbit() && targetOrbit <= spaceShip.getMaxOrbit());
                            bool hullOk = (targetOrbit >= spaceShip.getMinOrbitReach() && targetOrbit <= spaceShip.getMaxOrbitReach());
                            
                            if (spaceShip.getHasWarpDrive() || (engineOk && hullOk)) {
                                spaceShip.setCurrentOrbit(targetOrbit);
                            }
                        }
                        else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                            short targetOrbit = planetas[selectedPlanetIndex].getOrbit();
                            
                            bool engineOk = (targetOrbit >= spaceShip.getMinOrbit() && targetOrbit <= spaceShip.getMaxOrbit());
                            bool hullOk = (targetOrbit >= spaceShip.getMinOrbitReach() && targetOrbit <= spaceShip.getMaxOrbitReach());

                            if (spaceShip.getHasWarpDrive() || (engineOk && hullOk)) {
                                audio.playClick();
                                currentState = State::TravelConfirmation; 
                            } else {
                                std::cout << "[SYSTEM] You cannot travel to Orbit " << targetOrbit 
                                          << ". You need to upgrade your engines or heat shields!" << std::endl;
                            }
                        }
                    }

                }
                debugMenu.handleInput(*event, mousePos, spaceShip, spaceShip.getInventory());
            }
            else if (currentState == State::Animation1) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                    intro.handleInput(keyPressed->code);
                    
                    // if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    //     currentState = State::Playing;
                    // }
                }
            }
            else if (currentState == State::TravelConfirmation) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Y) {
                        shipAnimX = -100.f;
                        currentState = State::InPlanet; 
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::N) {
                        currentState = State::Playing;  
                    }
                }
            }
            else if (currentState == State::InPlanet) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Playing; 
                        audio.playHover();
                        
                        // NUEVO: Tirar dado para encuentro con piratas al salir del planeta
                        if (pirates.rollForEncounter(0.3f)) {
                            pirateEncounterActive = true;
                            pirates.reset();
                            currentState = State::PirateEncounter;
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::T) {
                        currentState = State::TradeMenu;
                        audio.playHover();
                    }
                }
            }
            else if (currentState == State::TradeMenu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        if (tradeMenu.isInfoPopupOpen()) {
                            tradeMenu.closeInfoPopup();
                        }
                        else {
                            currentState = State::InPlanet; // We leave the store if there are no pop-ups
                        }
                    }
                }
                // We pass the event to the class
                tradeMenu.handleInput(*event, mousePos, spaceShip.getInventory(), world.getPlanets()[selectedPlanetIndex], spaceShip, world.getCatalog());
            }
            else if (currentState == State::ShipMenu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Playing;
                    }
                }
                shipMenu.handleInput(*event, mousePos, spaceShip.getInventory().getUsedSlots(), spaceShip.getInventory(), world.getCatalog(), currentState, spaceShip);
            }
            else if (currentState == State::UpgradeTree) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::ShipMenu;
                    }
                }
                upgradeTree.handleInput(*event, mousePos, upgrades, spaceShip.getMoneyRef());
            }
            else if (currentState == State::PirateEncounter) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    
                    // 1. SI ESTAMOS EN EL AVISO FINAL
                    if (pirates.getCurrentMenu() == Interface::PirateMenu::Result) {
                        if (keyPressed->code == sf::Keyboard::Key::Enter) {
                            pirates.stop();
                            pirateEncounterActive = false;
                            currentState = State::Playing;
                        }
                    } 
                    // 2. SI ESTAMOS ELIGIENDO (Lógica de Balance)
                    else if (pirates.isShowingButtons()) {
                        pirates.handleInput(keyPressed->code);

                        if (keyPressed->code == sf::Keyboard::Key::Enter) {
                            int sel = pirates.getSelectedButton();
                            int roll = std::rand() % 100;
                            // NOTA: Usa el nombre de tu variable del main (ej: spaceShip o player)
                            int playerLevel = spaceShip.getLevel(); 

                            if (pirates.getCurrentMenu() == Interface::PirateMenu::Main) {
                                if (sel == 0) { // DEFENDERSE
                                    int winChance = 30 + (playerLevel * 12); 
                                    if (roll < winChance) {
                                        pirates.setResult("VICTORIA: Sistemas Nvl " + std::to_string(playerLevel) + " repelieron el ataque.");
                                    } else {
                                        spaceShip.setMoney(0.0f);
                                        pirates.setResult("DERROTA: Saquearon tus creditos.");
                                    }
                                } 
                                else if (sel == 1) { pirates.setMenu(Interface::PirateMenu::Bribery); }
                                else if (sel == 2) { 
                                    spaceShip.getInventory().clearAll();
                                    pirates.setResult("RENDIDO: Han tomado toda tu carga.");
                                }
                            } 
                            else if (pirates.getCurrentMenu() == Interface::PirateMenu::Bribery) {
                                float currentMoney = spaceShip.getMoney();
                                if (sel == 0) { // SOBORNO 80% (Casi seguro)
                                    spaceShip.setMoney(currentMoney * 0.2f);
                                    if (roll < 5) {
                                        spaceShip.setMoney(0.0f);
                                        pirates.setResult("TRAICION: Se llevaron el pago y el resto.");
                                    } else {
                                        pirates.setResult("PAGADO: Eres libre de irte.");
                                    }
                                }
                                else if (sel == 1) { // SOBORNO 40% (Arriesgado)
                                    spaceShip.setMoney(currentMoney * 0.6f);
                                    if (roll < 45) {
                                        spaceShip.setMoney(0.0f);
                                        pirates.setResult("RECHAZADO: El soborno fue insuficiente.");
                                    } else {
                                        pirates.setResult("SUERTE: Aceptaron el trato minimo.");
                                    }
                                }
                                else { pirates.setMenu(Interface::PirateMenu::Main); }
                            }
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                        pirates.setShowButtons(true);
                    }
                }
            }
        }
        // --- UPDATE & DRAW
        window.clear();

        if (currentState == State::Menu) {
            window.draw(backgroundSprite);
            mainMenu.draw(window);
        } 
        else if (currentState == State::Options){
            window.draw(settingsBackgroundSprite);
            settingsMenu.draw(window);
        }
        
        else if (currentState == State::GameIntro) {
            gameIntro.update(dt);
            gameIntro.draw(window);
            
            if (gameIntro.isFinished()) {
                currentState = State::Playing;
                gameIntro.reset();
            }
        }
        else if (currentState == State::Playing || currentState == State::ShipMenu || currentState == State::TravelConfirmation)
        {
            window.draw(generalBackground); 

            sf::Vector2f playerPos = spaceShip.getPosition();
            
            bgStars.draw(window, spaceShip.getPosition());

            sf::Vector2f center(640.f, 360.f);
            sf::CircleShape sun(15.f); sun.setFillColor(sf::Color::White);
            sun.setOrigin({15.f, 15.f}); sun.setPosition(center);
            window.draw(sun);

            float time = worldClock.getElapsedTime().asSeconds(); 
            const auto& planets = world.getPlanets();

            for (size_t i = 0; i < planets.size(); ++i) {
                auto& planet = world.getPlanets()[i];
                
                float distance = planet.getOrbit() * 33.f + 33.f;
                float speed = 0.5f / (planet.getOrbit() * 0.2f);
                float x = center.x + std::cos(time * speed) * distance;
                float y = center.y + std::sin(time * speed) * distance;

                if (planet.getOrbit() == spaceShip.getCurrentOrbit()) {
                    targetPosition = {x, y};
                }

                if (planet.hasSprite()) {
                    planet.getSprite()->setPosition({x, y});
                    
                    if (i == (size_t)selectedPlanetIndex) {
                        planet.setHighlighted(true); // El anillo cyan sigue al cursor
                    }
                            else {
                        planet.setHighlighted(false);
                    }
                    planet.updateScale(dt);
                    window.draw(*planet.getSprite());
                    
                }
                else {
                    sf::CircleShape planetShape(8.f);
                    planetShape.setOrigin({8.f, 8.f});
                    planetShape.setPosition({x, y});
                    
                    if (i == (size_t)selectedPlanetIndex) {
                        planetShape.setFillColor(sf::Color::Cyan);
                        planetShape.setOutlineThickness(2);
                        planetShape.setOutlineColor(sf::Color::White);
                        
                        short targetOrbit = planet.getOrbit();
                        bool engineOk = (targetOrbit >= spaceShip.getMinOrbit() && targetOrbit <= spaceShip.getMaxOrbit());
                        bool hullOk = (targetOrbit >= spaceShip.getMinOrbitReach() && targetOrbit <= spaceShip.getMaxOrbitReach());

                        if (spaceShip.getHasWarpDrive() || (engineOk && hullOk)) {
                            targetPosition = {x, y};
                        }

                    }
                    else {
                        planetShape.setFillColor(sf::Color(150, 150, 150));
                    }
                    window.draw(planetShape);
                }
            }
            
            sf::Vector2f currentPos = spaceShip.getPosition();
            sf::Vector2f direction = targetPosition - currentPos;
            float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (dist > 1.5f) {
                float angleRadians = std::atan2(direction.y, direction.x);
                float angleDegrees = angleRadians * 180.f / 3.14159265f;
                spaceShip.setRotation(angleDegrees + 90.f);

                direction /= dist; 
                float moveDistance = travelSpeed * dt;

                if (moveDistance > dist) {
                    spaceShip.setPosition(targetPosition);
                }
                else {
                    spaceShip.setPosition(currentPos + direction * moveDistance);
                }
            }
            else {
                spaceShip.setPosition(targetPosition);
            }

            if (!planets.empty()) {
                planetNameText.setString(planets[selectedPlanetIndex].getName());
                auto& selectedPlanet = world.getPlanets()[selectedPlanetIndex];

                if (selectedPlanet.hasSprite()){
                    sf::Sprite uiSprite = *selectedPlanet.getSprite();
                    auto texSize = selectedPlanet.getSprite()->getTexture().getSize();
                    float uiScale = 150.f/std::max(texSize.x, texSize.y);
                    uiSprite.setScale({uiScale, uiScale});
                    uiSprite.setPosition({85.f, 590.f});

                    window.draw(uiSprite);
                }
            }

            spaceShip.update(dt);
            spaceShip.draw(window);


            window.draw(planetNameText);

            window.draw(adminShipBtn);
            window.draw(adminShipText);

            // Draw the Alert & Update the Radar only if the player has it unlocked
            if(spaceShip.gethasAdvancedRadar()) {
                if (alertTimer > 0) {
                    window.draw(alertSprite);
                }

                if(aux) {
                    radarUI.update(world.getRadar()->getHeapArray());
                    aux = false; // Solo actualizar el radar la primera vez para mostrarlo al inicio
                }
                radarUI.draw(window);
            }
            if (spaceShip.getHasWarpDrive()) {
                window.draw(animBtn);
                window.draw(animBtnText);
            }

            if (currentState == State::TravelConfirmation) {
                confirmText.setString("DESEAS VIAJAR A " + world.getPlanets()[selectedPlanetIndex].getName() + "?");
                confirmText.setOrigin({confirmText.getLocalBounds().size.x / 2.f, 0.f});
                confirmText.setPosition({640.f, 330.f});
                window.draw(confirmBg);
                window.draw(confirmText);
                window.draw(optionsText);
            }

            if (currentState == State::ShipMenu) {
                shipMenu.update(mousePos);
                shipMenu.draw(window, spaceShip, world.getGlobalCatalog());
            }

        debugMenu.draw(window);
        }
        else if (currentState == State::Animation1) {
            intro.update(dt);
            intro.draw(window);

            if (intro.getState() == Interface::AnimState::Finished) {
                currentState = State::Menu;
            }
        }
        else if (currentState == State::InPlanet) {
            window.clear(backgroundSprite.getColor());
            window.draw(generalBackground);
            
            shipAnimX += 400.f * dt; 
            if (shipAnimX > 1380.f) shipAnimX = -100.f;

            sf::Vector2f originalPos = spaceShip.getPosition();
            
            spaceShip.setPosition({shipAnimX, 500.f}); 
            spaceShip.setRotation(90.f); 
            spaceShip.draw(window);
            spaceShip.setPosition(originalPos);
            spaceShip.setRotation(0.f); 

            sf::Text msg(font, "ESTAS EN EL PLANETA: " + world.getPlanets()[selectedPlanetIndex].getName());
            msg.setOrigin({msg.getLocalBounds().size.x / 2.f, 0.f});
            msg.setPosition({640.f, 300.f});
            window.draw(msg);

            sf::Text escMsg(font, "Presiona ESC para regresar, ENTER o T para ir a la tienda");
            escMsg.setCharacterSize(15);
            escMsg.setOrigin({escMsg.getLocalBounds().size.x / 2.f, 0.f});
            escMsg.setPosition({640.f, 650.f});
            window.draw(escMsg);
            bgStars.draw(window, spaceShip.getPosition());
        }
        else if (currentState == State::TradeMenu) {
            window.clear(backgroundSprite.getColor()); 
            window.draw(generalBackground); 
            bgStars.draw(window, spaceShip.getPosition());
            tradeMenu.draw(window, spaceShip.getInventory(), world.getPlanets()[selectedPlanetIndex], spaceShip.getMoney(), world.getGlobalCatalog(), spaceShip, spaceShip.getShipLevel());
            tradeMenu.update(mousePos);
        }
        else if (currentState == State::UpgradeTree) {
            window.draw(generalBackground);
            bgStars.draw(window, spaceShip.getPosition());
            
            upgradeTree.update(mousePos, upgrades);
            upgradeTree.draw(window, upgrades); 
        }

        //Dibujar el encuentro con piratas
        else if (currentState == State::PirateEncounter) {
            window.clear(sf::Color::Black);
            bgStars.draw(window, spaceShip.getPosition());
            window.setView(window.getDefaultView()); 

            pirates.update(dt);
            pirates.draw(window, font); 
            
            sf::RectangleShape darkOverlay;
            darkOverlay.setSize({1280.f, 720.f});
            darkOverlay.setFillColor(sf::Color(0, 0, 0, 100));
            window.draw(darkOverlay);

            if (!pirates.isShowingButtons()) {
                sf::Text warningText(font, "PIRATAS TE ATACAN!"); 
                warningText.setCharacterSize(30);
                warningText.setFillColor(sf::Color::Red);
                warningText.setOutlineColor(sf::Color::Black);
                warningText.setOutlineThickness(2);
                sf::FloatRect textBounds = warningText.getLocalBounds();
                warningText.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
                warningText.setPosition({640.f, 150.f});
                window.draw(warningText);
                
                sf::Text continueText(font, "Presiona ENTER para ver opciones");
                continueText.setCharacterSize(16);
                continueText.setFillColor(sf::Color::White);
                sf::FloatRect contBounds = continueText.getLocalBounds();
                continueText.setOrigin({contBounds.size.x / 2.f, 0.f});
                continueText.setPosition({640.f, 650.f});
                window.draw(continueText);
            }
        }

        window.display();
    } // Cierre del bucle while (window.isOpen())

    return 0;
} // Cierre de la función int main()