#include "Menu.hpp"
#include "Settings.hpp"
#include "Player.hpp"
#include "Inventory.hpp"
#include "ItemFactory.hpp"
#include "Planet.hpp"
#include "Economy.hpp"
#include "State.hpp"
#include "WorldStatemanager.hpp"
#include "PlanetManager.hpp"
#include "World.hpp"
#include "RadarUI.hpp"
#include <iostream>
#include <optional>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath> // Necesario para atan2 y sqrt

GameConfig mainConfig;

void ejecuteAction(std::string option, State& state, sf::RenderWindow& window) {
    if (option == "START") {
        state = State::Playing; 
    } else if (option == "SETTINGS") {
        state = State::Options;
    } else if (option == "EXIT") {
        window.close();
    }
}

int main() {
    bool estado = false;
    mainConfig.loadFromSavedFile("config.txt");
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "IT: Interstellar Trader");
    sf::Clock worldClock;

    if (mainConfig.vsync){
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(0); 
    }
    else{
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(mainConfig.fpsLimit);
    }
    sf::Font font;
    if(!font.openFromFile("assets/fonts/04B_03__.TTF")) {
        std::cerr << "Error cargando la fuente" << std::endl;
    }

    sf::Music music;
    if (!music.openFromFile("assets/audio/undertale_dogsong.ogg")){ 
        std::cerr << "Error cargando la música" << std::endl;
    }
    music.setLooping(true);
    music.setVolume((float)mainConfig.musicVolume);
    music.play();

    sf::SoundBuffer hoverBuffer, clickBuffer;
    if (!hoverBuffer.loadFromFile("assets/audio/hover_sound.ogg") || !clickBuffer.loadFromFile("assets/audio/option_selection_sound.ogg")){
        std::cerr << "Error cargando los efectos de sonido" << std::endl;
    }

    sf::Sound hoverSound(hoverBuffer);
    sf::Sound clickSound(clickBuffer);

    hoverSound.setVolume((float)mainConfig.sfxVolume);
    clickSound.setVolume((float)mainConfig.sfxVolume);

    Menu mainMenu(1280.f, 720.f);
    Settings settingsMenu(1280.f, 720.f, font);
    settingsMenu.resetTempConfig(mainConfig);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/mainMenu_background.jpeg")) {
        std::cerr << "Error cargando la imagen de fondo" << std::endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = 1280 / static_cast<float>(textureSize.x);
    float scaleY = 720 / static_cast<float>(textureSize.y);
    backgroundSprite.setScale({scaleX, scaleY});

    sf::Texture settingsBackgroundTexture;
    if (!settingsBackgroundTexture.loadFromFile("assets/settingsMenu_background.png")) {
        std::cerr << "Error cargando la imagen de fondo de settings" << std::endl;
    }
    sf::Sprite settingsBackgroundSprite(settingsBackgroundTexture);

    State currentState = State::Menu;

    Player player(640.f, 360.f, "assets/player.png");

    sf::Clock clock; 

    sf::Vector2f mousePos;

    std::vector<Planet> planetasBase = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets.txt");

    if (planetasBase.empty()) {
        std::cout << "[ERROR] No se cargaron planetas." << std::endl;
        return -1; 
    }

    auto radarInicial = std::make_unique<Heap>(Planet(planetasBase[0]));

    for (size_t i = 1; i < planetasBase.size(); ++i) {
        radarInicial->insert(Planet(planetasBase[i]), radarInicial->getHeapArray(), cmp);
    }

    World world(0.0f, std::move(radarInicial), std::move(planetasBase));

    sf::VertexArray starsFar(sf::PrimitiveType::Points, 800);
    for (int i = 0; i < 800; i++) {
        starsFar[i].position = {(float)(rand()%2000), (float)(rand()%2000)};
        int c = 60 + (rand()% 40);
        starsFar[i].color = sf::Color(c, c, c);
    }

    sf::VertexArray starsMid(sf::PrimitiveType::Points, 400);
    for (int i = 0; i < 400; i++) {
        starsMid[i].position = {(float)(rand()%2000), (float)(rand()%2000)};
        int c = 120 + (rand()% 60);
        starsMid[i].color = sf::Color(c, c, c);
    }

    sf::VertexArray starsNear(sf::PrimitiveType::Points, 200);
    for (int i = 0; i < 200; i++) {
        starsNear[i].position = {(float)(rand()%2000), (float)(rand()%2000)};
        int c = 200 + (rand()% 55);
        starsNear[i].color = sf::Color(c, c, c);
    }
    
    sf::Texture alertTexture;
    if (!alertTexture.loadFromFile("assets/alert_icon.png")) {
        std::cerr << "Error cargando la textura de alerta" << std::endl;
    }
    sf::Sprite alertSprite(alertTexture);
    alertSprite.setPosition({1100.f, 600.f}); 

    float alertTimer = 0.f; 
    
    RadarUI radarUI(font);

    sf::Text planetNameText(font, "");
    planetNameText.setCharacterSize(22);
    planetNameText.setFillColor(sf::Color::White);
    planetNameText.setOutlineColor(sf::Color::Black);
    planetNameText.setOutlineThickness(1);
    planetNameText.setPosition({ 30.f, 660.f });

    sf::CircleShape uiPlanetSprite(40.f); 
    uiPlanetSprite.setOrigin({20.f, 20.f});
    uiPlanetSprite.setPosition({ 55.f, 580.f }); 

    sf::RectangleShape adminShipBtn({200.f, 50.f});
    adminShipBtn.setFillColor(sf::Color(50, 50, 50, 200));
    adminShipBtn.setOutlineThickness(2);
    adminShipBtn.setOutlineColor(sf::Color::White);
    adminShipBtn.setPosition({ 1050.f, 60.f });

    sf::Text adminShipText(font, "ADMINISTRAR NAVE");
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

    sf::Text optionsText(font, "[Y] SI - VIAJAR    [N] NO - CANCELAR");
    optionsText.setCharacterSize(18);
    optionsText.setFillColor(sf::Color::Yellow);
    optionsText.setOrigin({optionsText.getLocalBounds().size.x / 2.f, 0.f});
    optionsText.setPosition({640.f, 400.f});

    sf::Texture shipMenuTexture;
    if (!shipMenuTexture.loadFromFile("assets/player.png")) {
        std::cerr << "Error cargando textura de previsualización" << std::endl;
    }

    sf::RectangleShape shipMenuBg({640.f, 400.f});
    shipMenuBg.setFillColor(sf::Color(15, 15, 25, 180));
    shipMenuBg.setOutlineThickness(3);
    shipMenuBg.setOutlineColor(sf::Color::Cyan);
    shipMenuBg.setOrigin({320.f, 200.f});
    shipMenuBg.setPosition({640.f, 360.f});

    sf::Text shipMenuTitle(font, "ESTADO DE LA NAVE");
    shipMenuTitle.setCharacterSize(25);
    shipMenuTitle.setFillColor(sf::Color::Cyan);
    shipMenuTitle.setPosition({480.f, 180.f});

    sf::Sprite shipPreview(shipMenuTexture); 
    shipPreview.setScale({2.f, 2.f});
    shipPreview.setPosition({210.f, 120.f});

    sf::RectangleShape upgradeBtn({200.f, 40.f});
    upgradeBtn.setFillColor(sf::Color(0, 150, 0));
    upgradeBtn.setPosition({650.f, 320.f});

    sf::Text upgradeText(font, "SUBIR DE NIVEL");
    upgradeText.setCharacterSize(18);
    sf::FloatRect upRect = upgradeText.getLocalBounds();
    upgradeText.setOrigin({upRect.size.x / 2.f, upRect.size.y / 2.f});
    upgradeText.setPosition({750.f, 340.f}); 

    int selectedPlanetIndex = 0;
    sf::Vector2f targetPosition(640.f, 360.f); 
    float travelSpeed = 400.f; // Velocidad de la nave

    while (window.isOpen()) {
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float dt = clock.restart().asSeconds(); 
        world.setDeltaTime(dt);

        if (currentState == State::Playing) {
            if (world.update()) {
                radarUI.update(world.getRadar()->getHeapArray());
                alertTimer = 3.0f; 
            }
            if (alertTimer > 0) alertTimer -= dt;
        }

        if (currentState == State::Playing) {
            if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                adminShipBtn.setFillColor(sf::Color(80, 80, 80, 255));
                adminShipBtn.setOutlineColor(sf::Color::Cyan);
            } else {
                adminShipBtn.setFillColor(sf::Color(50, 50, 50, 200));
                adminShipBtn.setOutlineColor(sf::Color::White);
            }
        }
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == State::Menu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                        mainMenu.moveUp();
                        hoverSound.play();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                        mainMenu.moveDown();
                        hoverSound.play();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
                        clickSound.play();
                    }
                }
                else if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2f mPos = window.mapPixelToCoords(mouseMoved->position);
                    if(mainMenu.updateHover(mPos)){
                        hoverSound.stop();
                        hoverSound.play();
                    }
                }
                else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
                        clickSound.play();
                    }
                }
            }
            else if (currentState == State::Options){
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                    if(keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up){
                        settingsMenu.moveUp();
                        hoverSound.play();
                    } else if(keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down){
                        settingsMenu.moveDown();
                        hoverSound.play();
                    } else if (keyPressed->code == sf::Keyboard::Key::Escape){
                        settingsMenu.resetTempConfig(mainConfig);
                        currentState = State::Menu;
                    } else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space){
                        settingsMenu.handleAction(currentState, window, mainConfig);
                        clickSound.play();
                    } else if (keyPressed->code == sf::Keyboard::Key::D || keyPressed->code == sf::Keyboard::Key::Right){
                        settingsMenu.changeValue(1); 
                        hoverSound.play();
                    } else if (keyPressed->code == sf::Keyboard::Key::A || keyPressed->code == sf::Keyboard::Key::Left){
                        settingsMenu.changeValue(-1); 
                        hoverSound.play();
                    }
                }
                else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseEvent->button == sf::Mouse::Button::Left){
                        sf::Vector2f mPos = window.mapPixelToCoords(mouseEvent->position);
                        clickSound.play();
                        settingsMenu.handleMouseClick(mPos);
                        std::string opt = settingsMenu.getSelectedOption();
                        if(opt == "APPLY"){
                            settingsMenu.applySettings(window, mainConfig);
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
                        hoverSound.stop();
                        hoverSound.play();
                    }
                    settingsMenu.handleMouseMove(mPos);
                }
                music.setVolume((float)settingsMenu.getTempMusicVolume());
                hoverSound.setVolume((float)settingsMenu.getTempSfxVolume());
                clickSound.setVolume((float)settingsMenu.getTempSfxVolume());
            }
            else if (currentState == State::Playing) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                            clickSound.play();
                            currentState = State::ShipMenu; 
                        }
                    }
                }
                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }
                    const auto& planetas = world.getPlanets();
                    if (!planetas.empty()) {
                        if (keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::D ||
                            keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                            selectedPlanetIndex = (selectedPlanetIndex + 1) % planetas.size();
                            hoverSound.play();
                        }
                        else if (keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::A ||
                                keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                            selectedPlanetIndex = (selectedPlanetIndex - 1 + (int)planetas.size()) % planetas.size();
                            hoverSound.play();
                        }
                        else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                            clickSound.play();
                            currentState = State::TravelConfirmation; 
                        }
                    }
                }
            }
            else if (currentState == State::TravelConfirmation) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Y) {
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
                        hoverSound.play();             
                    }
                }
            }
            else if (currentState == State::ShipMenu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Playing;
                    }
                }
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        if (upgradeBtn.getGlobalBounds().contains(mousePos)) {
                            clickSound.play();
                            std::cout << "Nivel de nave aumentado!" << std::endl;
                        }
                    }
                }
            }
        }

        window.clear();
        
        if (currentState == State::Menu) {
            window.draw(backgroundSprite);
            mainMenu.draw(window);
        } 
        else if (currentState == State::Options){
            window.draw(settingsBackgroundSprite);
            settingsMenu.draw(window);
        }
        else if (currentState == State::Playing || currentState == State::ShipMenu || currentState == State::TravelConfirmation) {
            window.clear(sf::Color(0, 0, 15));
            sf::Vector2f playerPos = player.getPosition();
            sf::RenderStates states;

            sf::Transform tFar; tFar.translate(-playerPos*0.2f);
            states.transform = tFar; window.draw(starsFar, states);

            sf::Transform tMid; tMid.translate(-playerPos*0.4f);
            states.transform = tMid; window.draw(starsMid, states);

            sf::Transform tNear; tNear.translate(-playerPos*0.7f);
            states.transform = tNear; window.draw(starsNear, states);

            sf::Vector2f center(640.f, 360.f);
            sf::CircleShape sun(15.f); sun.setFillColor(sf::Color::White);
            sun.setOrigin({15.f, 15.f}); sun.setPosition(center);
            window.draw(sun);

            float time = worldClock.getElapsedTime().asSeconds(); 
            const auto& planets = world.getPlanets();

            for (size_t i = 0; i < planets.size(); ++i) {
                float distance = planets[i].getOrbit() * 33.f + 33.f;
                float speed = 0.5f / (planets[i].getOrbit() * 0.2f);
                float x = center.x + std::cos(time * speed) * distance;
                float y = center.y + std::sin(time * speed) * distance;

                sf::CircleShape planetShape(8.f);
                planetShape.setOrigin({8.f, 8.f});
                planetShape.setPosition({x, y});

                if (i == (size_t)selectedPlanetIndex) {
                    planetShape.setFillColor(sf::Color::Cyan); 
                    planetShape.setOutlineThickness(2);
                    planetShape.setOutlineColor(sf::Color::White);
                    targetPosition = {x, y}; // Actualización dinámica del objetivo
                } else {
                    planetShape.setFillColor(sf::Color(150, 150, 150));
                }
                window.draw(planetShape);
            }

            // --- MOVIMIENTO SUAVE Y ROTACIÓN (CORREGIDO) ---
            sf::Vector2f currentPos = player.getPosition();
            sf::Vector2f direction = targetPosition - currentPos;
            float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (dist > 1.5f) {
                float angleRadians = std::atan2(direction.y, direction.x);
                float angleDegrees = angleRadians * 180.f / 3.14159265f;
                player.setRotation(angleDegrees + 90.f);

                direction /= dist; // Normalizar
                float moveDistance = travelSpeed * dt;

                if (moveDistance > dist) {
                    player.setPosition(targetPosition);
                } else {
                    player.setPosition(currentPos + direction * moveDistance);
                }
            } else {
                player.setPosition(targetPosition);
            }

            if (!planets.empty()) {
                planetNameText.setString(planets[selectedPlanetIndex].getName());
                uiPlanetSprite.setFillColor(sf::Color::Cyan); 
                uiPlanetSprite.setOutlineThickness(2);
                uiPlanetSprite.setOutlineColor(sf::Color::White);
            }

            player.update(dt);
            player.draw(window);
            window.draw(uiPlanetSprite);
            window.draw(planetNameText);
            if (alertTimer > 0) window.draw(alertSprite);
            radarUI.draw(window);
            window.draw(adminShipBtn);
            window.draw(adminShipText);

            if (currentState == State::TravelConfirmation) {
                confirmText.setString("DESEAS VIAJAR A " + world.getPlanets()[selectedPlanetIndex].getName() + "?");
                confirmText.setOrigin({confirmText.getLocalBounds().size.x / 2.f, 0.f});
                confirmText.setPosition({640.f, 330.f});
                window.draw(confirmBg);
                window.draw(confirmText);
                window.draw(optionsText);
            }

            if (currentState == State::ShipMenu) {
                window.draw(shipMenuBg);
                window.draw(shipMenuTitle);
                window.draw(shipPreview);
                
                if (upgradeBtn.getGlobalBounds().contains(mousePos)) {
                    upgradeBtn.setFillColor(sf::Color(0, 200, 0));
                } else {
                    upgradeBtn.setFillColor(sf::Color(0, 150, 0));
                }
                window.draw(upgradeBtn);
                window.draw(upgradeText);
                
                sf::Text backMsg(font, "ESC para volver");
                backMsg.setCharacterSize(14);
                backMsg.setPosition({580.f, 530.f});
                window.draw(backMsg);
            }
        }
        else if (currentState == State::InPlanet) {
            window.clear(sf::Color::Black); 
            sf::Text msg(font, "ESTAS EN EL PLANETA: " + world.getPlanets()[selectedPlanetIndex].getName());
            msg.setPosition({400.f, 300.f});
            window.draw(msg);
        }

        window.display();
    }

    return 0;
}