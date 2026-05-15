#include "Core/Engine.hpp"

namespace Game {

    Engine::Engine() 
        : window(sf::VideoMode({1280, 720}), "IT: Interstellar Trader"),
          currentState(State::Menu),
          selectedPlanetIndex(2),
          targetPosition(640.f + 132.f, 360.f),
          travelSpeed(400.f), 
          shipAnimX(-100.f),
          alertTimer(0.f),
          pirateEncounterActive(false), 
          auxRadar(true),
          currentAudioContext("menu"), 
          intro(1280.f, 720.f),
          gameIntro(1280.f, 720.f),
          spaceShip(640.f + 132.f, 360.f, "assets/player.png"),
          gameOverScene(nullptr)
    {
        std::cout << "[ENGINE] Constructor llamado" << std::endl;
    }

    bool Engine::init() {
        std::cout << "[ENGINE] Iniciando init()..." << std::endl;
        
        // 1. CARGAR TODOS LOS ARCHIVOS PRIMERO
        if (!AssetManager::loadAll()) {
            std::cerr << "[ENGINE] Error: AssetManager::loadAll() fallo" << std::endl;

            gameOverScene = std::make_unique<Game::GameOverScene>(mainFont);

            return false;
        }

        config.loadFromSavedFile("config.txt");

        audio.loadMusic("assets/audio/undertale_dogsong.ogg");
        audio.loadTheme("assets/audio/theme.ogg");
        audio.loadSFX("assets/audio/hover_sound.ogg", "assets/audio/option_selection_sound.ogg");
        audio.updateVolumesFromConfig(config.musicVolume, config.sfxVolume);
        audio.playMusic();

        pirates.loadAssets();

        window.setVerticalSyncEnabled(config.vsync);
        window.setFramerateLimit(config.vsync ? 0 : config.fpsLimit);

        mainMenu = std::make_unique<Menu>(1280.f, 720.f);
        settingsMenu = std::make_unique<Settings>(1280.f, 720.f, mainFont);
        settingsMenu->resetTempConfig(config);

        backgroundSprite.setTexture(AssetManager::getTexture("bg_main"));
        backgroundSprite.setScale({1280.f / AssetManager::getTexture("bg_main").getSize().x, 720.f / AssetManager::getTexture("bg_main").getSize().y});

        settingsBackgroundSprite.setTexture(AssetManager::getTexture("bg_settings"));

        generalBackground.setSize({ 1280.f, 720.f });
        generalBackground.setFillColor(sf::Color(0, 0, 15));


        std::vector<Planet> basePlanets = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets.txt");
        if (basePlanets.empty()) return false;

        auto radar = std::make_unique<Heap>(Planet(basePlanets[0]));
        for (size_t i = 1; i < basePlanets.size(); ++i) {
            radar->insert(Planet(basePlanets[i]), radar->getHeapArray(), cmp);
        }

        world = std::make_unique<World>(0.0f, std::move(radar), std::move(basePlanets));

        radarUI = std::make_unique<RadarUI>(mainFont);
        shipMenu = std::make_unique<ShipMenuUI>(mainFont, spaceShip.getShipTexture());
        tradeMenu = std::make_unique<TradeMenuUI>(mainFont);
        upgrades.initTrees(spaceShip);
        upgradeTree = std::make_unique<UpgradeTreeUI>(mainFont);
        debugMenu = std::make_unique<DebugMenuUI>(mainFont);
        debugMenu->initCatalog(world->getCatalog());

        alertSprite.setTexture(AssetManager::getTexture("alert_icon"));
        alertSprite.setPosition({1100.f, 600.f}); 

        planetNameText = sf::Text(mainFont, "");
        planetNameText.setCharacterSize(22);
        planetNameText.setFillColor(sf::Color::White);
        planetNameText.setOutlineColor(sf::Color::Black);
        planetNameText.setOutlineThickness(1);
        planetNameText.setPosition({ 30.f, 660.f });

        animBtn.setSize({200.f, 50.f});
        animBtn.setFillColor(sf::Color(100, 0, 150, 200)); 
        animBtn.setOutlineThickness(2);
        animBtn.setOutlineColor(sf::Color::White);
        animBtn.setPosition({ 1050.f, 130.f });

        animBtnText = sf::Text(mainFont, "REGRESAR A CASA");
        animBtnText.setCharacterSize(16);
        animBtnText.setFillColor(sf::Color::White);
        animBtnText.setOrigin({animBtnText.getLocalBounds().size.x / 2.f, animBtnText.getLocalBounds().size.y / 2.f});
        animBtnText.setPosition({ 1150.f, 150.f });

        adminShipBtn.setSize({200.f, 50.f});
        adminShipBtn.setPosition({ 1050.f, 60.f });
        adminShipText = sf::Text(mainFont, "SPACE SHIP");
        adminShipText.setCharacterSize(16);
        adminShipText.setOrigin({adminShipText.getLocalBounds().size.x / 2.f, adminShipText.getLocalBounds().size.y / 2.f});
        adminShipText.setPosition({ 1150.f, 80.f });

        confirmBg.setSize({400.f, 200.f});
        confirmBg.setFillColor(sf::Color(20, 20, 20, 240)); 
        confirmBg.setOutlineThickness(3);
        confirmBg.setOutlineColor(sf::Color::Cyan);
        confirmBg.setOrigin({200.f, 100.f});
        confirmBg.setPosition({640.f, 360.f}); 

        confirmText = sf::Text(mainFont, "");
        confirmText.setCharacterSize(20);

        optionsText = sf::Text(mainFont, "[Y] YES - LAND    [N] NO - CANCEL");
        optionsText.setCharacterSize(18);
        optionsText.setFillColor(sf::Color::Yellow);
        optionsText.setOrigin({optionsText.getLocalBounds().size.x / 2.f, 0.f});
        optionsText.setPosition({640.f, 400.f});

        intro.loadAssets(mainFont);
        gameIntro.loadAssets(mainFont);
        
        // CREAR GAMEOVERSCENE AQUÍ
        std::cout << "[ENGINE] Creando GameOverScene..." << std::endl;
        gameOverScene = std::make_unique<Game::GameOverScene>(mainFont);
        if (!gameOverScene) {
            std::cerr << "[ENGINE] Error: Failed to create GameOverScene" << std::endl;
            return false;
        }
        std::cout << "[ENGINE] GameOverScene creado correctamente" << std::endl;

        return true;
    }

    void Engine::executeAction(const std::string& option) {
        if (option == "NEW GAME" || option == "CONTINUE") currentState = State::GameIntro;
        else if (option == "SETTINGS") currentState = State::Options;
        else if (option == "EXIT") window.close();
    }

    void Engine::run() {
        while (window.isOpen()) {
            dt = clock.restart().asSeconds();
            world->setDeltaTime(dt);
            
            update();
            processEvents();
            render();
        }
    }

    void Engine::update() {
        // --- AUDIO STATE MANAGEMENT ---
        if (currentState == State::Menu || currentState == State::Options) {
            if (currentAudioContext != "menu") {
                audio.stopTheme();
                audio.loadMusic("assets/audio/undertale_dogsong.ogg");
                audio.playMusic();
                currentAudioContext = "menu";
            } else if (!audio.isMusicPlaying()) audio.playMusic();
        }
        else if (currentState == State::Animation1 || currentState == State::PirateEncounter) {
            if (currentAudioContext != "none") {
                audio.stopMusic();
                audio.stopTheme();
                currentAudioContext = "none";
            }
        }
        else if (currentState == State::GameIntro) {
            if (currentAudioContext != "gameintro") currentAudioContext = "gameintro";
        }
        else if (currentState == State::GameOver) {
            if (currentAudioContext != "none") {
                audio.stopMusic();
                audio.stopTheme();
                currentAudioContext = "none";
            }
        }
        else {
            if (currentAudioContext != "gameplay") {
                audio.stopMusic();
                audio.loadTheme("assets/audio/theme.ogg");
                audio.playTheme();
                currentAudioContext = "gameplay";
            }
        }

        // --- GAME LOGIC UPDATE ---
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (currentState == State::Playing) {
            if (world->update(spaceShip)) {
                alertTimer = 3.0f;
                if(spaceShip.gethasAdvancedRadar()) radarUI->update(world->getRadar()->getHeapArray());
            }
            if (alertTimer > 0) alertTimer -= dt;

            // Botones hover
            if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                adminShipBtn.setFillColor(sf::Color(80, 80, 80, 255));
                adminShipBtn.setOutlineColor(sf::Color::Cyan);
            } else {
                adminShipBtn.setFillColor(sf::Color(50, 50, 50, 200));
                adminShipBtn.setOutlineColor(sf::Color::White);
            }

            if(spaceShip.getHasWarpDrive()) {
                if (animBtn.getGlobalBounds().contains(mousePos)) {
                    animBtn.setFillColor(sf::Color(150, 0, 200, 255));
                    animBtn.setOutlineColor(sf::Color::Cyan);
                } else {
                    animBtn.setFillColor(sf::Color(100, 0, 150, 200));
                    animBtn.setOutlineColor(sf::Color::White);
                }
            }

            float time = worldClock.getElapsedTime().asSeconds();
            sf::Vector2f center(640.f, 360.f);
            auto& planets = world->getPlanets();

            for (size_t i = 0; i < planets.size(); ++i) {
                planets[i].updateOrbitPosition(time, center);
                
                if (planets[i].getOrbit() == spaceShip.getCurrentOrbit()) {
                    targetPosition = planets[i].getPosition();
                }

                if (i == (size_t)selectedPlanetIndex) {
                    planets[i].setHighlighted(true);
                    short tOrbit = planets[i].getOrbit();
                    bool engineOk = (tOrbit >= spaceShip.getMinOrbit() && tOrbit <= spaceShip.getMaxOrbit());
                    bool hullOk = (tOrbit >= spaceShip.getMinOrbitReach() && tOrbit <= spaceShip.getMaxOrbitReach());
                    if (spaceShip.getHasWarpDrive() || (engineOk && hullOk)) targetPosition = planets[i].getPosition();
                } else {
                    planets[i].setHighlighted(false);
                }
                planets[i].updateScale(dt);
            }

            spaceShip.travelTo(targetPosition, dt, travelSpeed);
            spaceShip.update(dt);
        }
        else if (currentState == State::GameIntro) {
            gameIntro.update(dt);
            if (gameIntro.isFinished()) {
                currentState = State::Playing;
                gameIntro.reset();
            }
        }
        else if (currentState == State::Animation1) {
            intro.update(dt);
            if (intro.getState() == Interface::AnimState::Finished) currentState = State::Menu;
        }
        else if (currentState == State::InPlanet) {
            shipAnimX += 400.f * dt; 
            if (shipAnimX > 1380.f) shipAnimX = -100.f;
        }
        else if (currentState == State::PirateEncounter) {
            pirates.update(dt);
        }
        else if (currentState == State::GameOver) {
            if (gameOverScene) {
                gameOverScene->update(dt);
            }
        }
        else if (currentState == State::TradeMenu) tradeMenu->update(mousePos);
        else if (currentState == State::ShipMenu) shipMenu->update(mousePos);
        else if (currentState == State::UpgradeTree) upgradeTree->update(mousePos, upgrades);
    }

    void Engine::processEvents() {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == State::Menu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                        mainMenu->moveUp(); audio.playHover();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                        mainMenu->moveDown(); audio.playHover();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        std::string opt = mainMenu->getSelectedOption();
                        if (opt == "CONTINUE" && !SaveSystem::saveExists()) continue;
                        
                        executeAction(opt);
                        
                        if (opt == "NEW GAME") {
                            SaveSystem::setGameSeed("ALFA-77");
                            spaceShip.resetToDefaults();
                            upgrades.resetTrees();
                            for (auto& planet : world->getPlanets()) planet.refreshMarket(world->getCatalog());
                            SaveSystem::saveGame(spaceShip, upgrades);
                        }
                        else if (opt == "CONTINUE") {
                            SaveData data;
                            if (SaveSystem::loadGame(data)) {
                                SaveSystem::setGameSeed(data.seedText); 
                                spaceShip.setMoney(data.money);
                                spaceShip.setCurrentOrbit(data.currentOrbit);
                                spaceShip.clearInv();
                                for (const auto& pair : data.inventory) {
                                    const auto& itemData = world->getCatalog().at(pair.first);
                                    spaceShip.getInventory().addItem(pair.first, pair.second, itemData->getMaxStackSize(), itemData->getBasePrice());
                                }

                                upgrades.resetTrees(); 
                                upgrades.loadPurchasedUpgrades(data.purchasedUpgrades); 
                                for (auto& planet : world->getPlanets()) planet.refreshMarket(world->getCatalog());
                            }
                        }
                        audio.playClick();
                    }
                }
                else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                    if(mainMenu->updateHover(window.mapPixelToCoords(mouseMoved->position))) audio.playHover();
                }
                else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        std::string opt = mainMenu->getSelectedOption();
                        if (opt == "CONTINUE" && !SaveSystem::saveExists()) continue;
                        
                        executeAction(opt);
                        if (opt == "NEW GAME") {
                            SaveSystem::setGameSeed("ALFA-77");
                            spaceShip.resetToDefaults();
                            upgrades.resetTrees();
                            for (auto& planet : world->getPlanets()) planet.refreshMarket(world->getCatalog());
                            SaveSystem::saveGame(spaceShip, upgrades);
                        }
                        else if (opt == "CONTINUE") {
                            SaveData data;
                            if (SaveSystem::loadGame(data)) {
                                SaveSystem::setGameSeed(data.seedText); 
                                spaceShip.setMoney(data.money);
                                spaceShip.setCurrentOrbit(data.currentOrbit);
                                spaceShip.getInventory().clearAll();
                                for (const auto& pair : data.inventory) {
                                    const auto& itemData = world->getCatalog().at(pair.first);
                                    spaceShip.getInventory().addItem(pair.first, pair.second, itemData->getMaxStackSize(), itemData->getBasePrice());
                                }
                                upgrades.resetTrees(); 
                                upgrades.loadPurchasedUpgrades(data.purchasedUpgrades); 
                                for (auto& planet : world->getPlanets()) planet.refreshMarket(world->getCatalog());
                            }
                        }
                        audio.playClick();
                    }
                }
            }
            else if (currentState == State::Options){
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                    if(keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up){
                        settingsMenu->moveUp(); audio.playHover();
                    }
                    else if(keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down){
                        settingsMenu->moveDown(); audio.playHover();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Escape){
                        settingsMenu->resetTempConfig(config); currentState = State::Menu;
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space){
                        settingsMenu->handleAction(currentState, window, config, audio); audio.playClick();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::D || keyPressed->code == sf::Keyboard::Key::Right){
                        settingsMenu->changeValue(1); audio.playHover();
                        audio.setMusicVolume((float)settingsMenu->getTempMusicVolume());
                        audio.setSFXVolume((float)settingsMenu->getTempSfxVolume());
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::A || keyPressed->code == sf::Keyboard::Key::Left){
                        settingsMenu->changeValue(-1); audio.playHover();
                        audio.setMusicVolume((float)settingsMenu->getTempMusicVolume());
                        audio.setSFXVolume((float)settingsMenu->getTempSfxVolume());
                    }
                }
                else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseEvent->button == sf::Mouse::Button::Left){
                        audio.playClick();
                        settingsMenu->handleMouseClick(mousePos);
                        std::string opt = settingsMenu->getSelectedOption();
                        if(opt == "APPLY") settingsMenu->applySettings(window, config, audio);
                        else if(opt == "BACK") { settingsMenu->resetTempConfig(config); currentState = State::Menu; }
                    }
                }
                else if (event->is<sf::Event::MouseButtonReleased>()){
                    settingsMenu->releaseSlider();
                }
                else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()){
                    if(settingsMenu->updateHover(mousePos)) audio.playHover();
                    settingsMenu->handleMouseMove(mousePos);
                    audio.setMusicVolume((float)settingsMenu->getTempMusicVolume());
                    audio.setSFXVolume((float)settingsMenu->getTempSfxVolume());
                }
            }
            else if (currentState == State::GameIntro) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) gameIntro.handleInput(keyPressed->code);
            }
            else if (currentState == State::Animation1) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) intro.handleInput(keyPressed->code);
            }
            else if (currentState == State::Playing) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                            audio.playClick(); currentState = State::ShipMenu; 
                        } else if(spaceShip.getHasWarpDrive() && animBtn.getGlobalBounds().contains(mousePos)) {
                            audio.playClick(); intro.reset(); currentState = State::Animation1; 
                        }
                    }
                }
                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }
                    else if(keyPressed->code == sf::Keyboard::Key::G || keyPressed->code == sf::Keyboard::Key::S) SaveSystem::saveGame(spaceShip, upgrades);
                    
                    const auto& planetas = world->getPlanets();
                    if (!planetas.empty()) {
                        if (keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::D ||
                            keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                            selectedPlanetIndex = (selectedPlanetIndex + 1) % planetas.size();
                            audio.playHover();
                        }
                        else if (keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::A ||
                                   keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                            selectedPlanetIndex = (selectedPlanetIndex - 1 + (int)planetas.size()) % planetas.size();
                            audio.playHover();
                        }
                        else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                            short targetOrbit = planetas[selectedPlanetIndex].getOrbit();
                            bool engineOk = (targetOrbit >= spaceShip.getMinOrbit() && targetOrbit <= spaceShip.getMaxOrbit());
                            bool hullOk = (targetOrbit >= spaceShip.getMinOrbitReach() && targetOrbit <= spaceShip.getMaxOrbitReach());

                            if (spaceShip.getHasWarpDrive() || (engineOk && hullOk)) {
                                audio.playClick(); currentState = State::TravelConfirmation; 
                            }
                            else {
                                std::cout << "[SYSTEM] No puedes viajar a esa orbita.\n";
                            }
                        }
                    }
                }
                debugMenu->handleInput(*event, mousePos, spaceShip, spaceShip.getInventory());
            }
            else if (currentState == State::TravelConfirmation) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Y) {
                        shipAnimX = -100.f; currentState = State::InPlanet; 
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::N) currentState = State::Playing;  
                }
            }
            else if (currentState == State::InPlanet) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Playing; 
                        audio.playHover();
                        if (pirates.rollForEncounter(0.3f)) {
                            pirateEncounterActive = true; pirates.reset(); currentState = State::PirateEncounter;
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::T) {
                        currentState = State::TradeMenu; audio.playHover();
                    }
                }
            }
            else if (currentState == State::TradeMenu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        if (tradeMenu->isInfoPopupOpen()) tradeMenu->closeInfoPopup();
                        else currentState = State::InPlanet;
                    }
                }
                tradeMenu->handleInput(*event, mousePos, spaceShip.getInventory(), world->getPlanets()[selectedPlanetIndex], spaceShip, world->getCatalog());
            }
            else if (currentState == State::ShipMenu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) currentState = State::Playing;
                }
                shipMenu->handleInput(*event, mousePos, spaceShip.getInventory().getUsedSlots(), spaceShip.getInventory(), world->getCatalog(), currentState, spaceShip);
            }
            else if (currentState == State::UpgradeTree) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) currentState = State::ShipMenu;
                }
                upgradeTree->handleInput(*event, mousePos, upgrades, spaceShip.getMoneyRef());
            }
            else if (currentState == State::PirateEncounter) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    bool gameOverTriggered = false;
                    
                    pirates.handleEncounterLogic(keyPressed->code, spaceShip, gameOverTriggered);
                    
                    // Verificar condición real de Game Over
                    bool shouldGameOver = Game::GameOverScene::checkCondition(spaceShip, world->getCatalog());
                    
                    if (shouldGameOver) {
                        if (gameOverScene) {
                            gameOverScene->setActive(true);
                            currentState = State::GameOver;
                        }
                    } else if (!pirates.isActive()) {
                        pirateEncounterActive = false; 
                        currentState = State::Playing;
                    }
                }
            }
            else if (currentState == State::GameOver) {
                if (gameOverScene && gameOverScene->handleInput(*event)) {
                    std::cout << "[ENGINE] Returning to main menu from Game Over" << std::endl;
                    currentState = State::Menu;
                    spaceShip.resetToDefaults();
                    upgrades.resetTrees();
                    for (auto& planet : world->getPlanets()) {
                        planet.refreshMarket(world->getCatalog());
                    }
                }
            }
        }
    }

    void Engine::render() {
        window.clear();

        if (currentState == State::Menu) {
            window.draw(backgroundSprite);
            mainMenu->draw(window);
        } 
        else if (currentState == State::Options){
            window.draw(settingsBackgroundSprite);
            settingsMenu->draw(window);
        }
        else if (currentState == State::GameIntro) {
            gameIntro.draw(window);
        }
        else if (currentState == State::GameOver) {
            if (gameOverScene) {
                gameOverScene->draw(window);
            } else {
                // Fallback si gameOverScene es null
                sf::RectangleShape blackScreen({1280.f, 720.f});
                blackScreen.setFillColor(sf::Color::Black);
                window.draw(blackScreen);
                
                sf::Text fallbackText(mainFont, "GAME OVER\nPress ENTER", 40);
                fallbackText.setOrigin({fallbackText.getLocalBounds().size.x / 2.f, 0});
                fallbackText.setPosition({640.f, 360.f});
                window.draw(fallbackText);
            }
        }
        else if (currentState == State::Playing || currentState == State::ShipMenu || currentState == State::TravelConfirmation) {
            window.draw(generalBackground); 
            bgStars.draw(window, spaceShip.getPosition());

            sf::Vector2f center(640.f, 360.f);
            sf::CircleShape sun(15.f);
            sun.setFillColor(sf::Color::White);
            sun.setOrigin({15.f, 15.f});
            sun.setPosition(center);
            window.draw(sun);

            const auto& planets = world->getPlanets();
            for (size_t i = 0; i < planets.size(); ++i) {
                if (planets[i].hasSprite()) {
                    window.draw(*planets[i].getSprite());
                } else {
                    sf::CircleShape planetShape(8.f);
                    planetShape.setOrigin({8.f, 8.f});
                    planetShape.setPosition(planets[i].getPosition());
                    planetShape.setFillColor(i == (size_t)selectedPlanetIndex ? sf::Color::Cyan : sf::Color(150, 150, 150));
                    if(i == (size_t)selectedPlanetIndex) { planetShape.setOutlineThickness(2); planetShape.setOutlineColor(sf::Color::White); }
                    window.draw(planetShape);
                }
            }

            if (!planets.empty()) {
                planetNameText.setString(planets[selectedPlanetIndex].getName());
                if (planets[selectedPlanetIndex].hasSprite()){
                    sf::Sprite uiSprite = *planets[selectedPlanetIndex].getSprite();
                    float uiScale = 150.f / std::max(uiSprite.getTexture().getSize().x, uiSprite.getTexture().getSize().y);
                    uiSprite.setScale({uiScale, uiScale});
                    uiSprite.setPosition({85.f, 590.f});
                    window.draw(uiSprite);
                }
            }

            spaceShip.draw(window);
            window.draw(planetNameText);
            window.draw(adminShipBtn);
            window.draw(adminShipText);

            if(spaceShip.gethasAdvancedRadar()) {
                if (alertTimer > 0) window.draw(alertSprite);
                if(auxRadar) {
                    radarUI->update(world->getRadar()->getHeapArray());
                    auxRadar = false;
                }
                radarUI->draw(window);
            }
            if (spaceShip.getHasWarpDrive()) {
                window.draw(animBtn); window.draw(animBtnText);
            }

            if (currentState == State::TravelConfirmation) {
                confirmText.setString("DESEAS VIAJAR A " + world->getPlanets()[selectedPlanetIndex].getName() + "?");
                confirmText.setPosition({640.f, 330.f});
                confirmText.setOrigin({confirmText.getLocalBounds().size.x / 2.f, 0.f});
                window.draw(confirmBg);
                window.draw(confirmText);
                window.draw(optionsText);
            }
            if (currentState == State::ShipMenu) shipMenu->draw(window, spaceShip, world->getGlobalCatalog());

            debugMenu->draw(window);
        }
        else if (currentState == State::Animation1) {
            intro.draw(window);
        }
        else if (currentState == State::InPlanet) {
            window.clear(backgroundSprite.getColor());
            window.draw(generalBackground);
            
            sf::Vector2f originalPos = spaceShip.getPosition();
            spaceShip.setPosition({shipAnimX, 500.f});
            spaceShip.setRotation(90.f); 
            spaceShip.draw(window);
            spaceShip.setPosition(originalPos);
            spaceShip.setRotation(0.f); 

            sf::Text msg(mainFont, "ESTAS EN EL PLANETA: " + world->getPlanets()[selectedPlanetIndex].getName());
            msg.setOrigin({msg.getLocalBounds().size.x / 2.f, 0.f});
            msg.setPosition({640.f, 300.f});
            window.draw(msg);

            sf::Text escMsg(mainFont, "Presiona ESC para regresar, ENTER o T para ir a la tienda");
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
            tradeMenu->draw(window, spaceShip.getInventory(), world->getPlanets()[selectedPlanetIndex], spaceShip.getMoney(), world->getGlobalCatalog(), spaceShip, spaceShip.getLevel());
        }
        else if (currentState == State::UpgradeTree) {
            window.draw(generalBackground);
            bgStars.draw(window, spaceShip.getPosition());
            upgradeTree->draw(window, upgrades); 
        }
        else if (currentState == State::PirateEncounter) {
            window.clear(sf::Color::Black);
            bgStars.draw(window, spaceShip.getPosition());
            window.setView(window.getDefaultView()); 
            pirates.draw(window, mainFont); 
            
            sf::RectangleShape darkOverlay({1280.f, 720.f}); darkOverlay.setFillColor(sf::Color(0, 0, 0, 100));
            window.draw(darkOverlay);

            if (!pirates.isShowingButtons()) {
                sf::Text warningText(mainFont, "PIRATAS TE ATACAN!"); 
                warningText.setCharacterSize(30);
                warningText.setFillColor(sf::Color::Red);
                warningText.setOutlineThickness(2);
                warningText.setOrigin({warningText.getLocalBounds().size.x / 2.f, warningText.getLocalBounds().size.y / 2.f});
                warningText.setPosition({640.f, 150.f});
                window.draw(warningText);
                
                sf::Text continueText(mainFont, "Presiona ENTER para ver opciones");
                continueText.setCharacterSize(16);
                continueText.setFillColor(sf::Color::White);
                continueText.setOrigin({continueText.getLocalBounds().size.x / 2.f, 0.f});
                continueText.setPosition({640.f, 650.f});
                window.draw(continueText);
            }
        }
        window.display();
    }
}