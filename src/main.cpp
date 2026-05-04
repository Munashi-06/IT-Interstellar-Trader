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
#include "ShipMenuUI.hpp"
#include "TradeMenuUI.hpp"
#include "AudioManager.hpp"
#include "BackgroundStars.hpp"
#include <iostream>
#include <optional>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath> 

GameConfig mainConfig;

void ejecuteAction(std::string option, State& state, sf::RenderWindow& window) {
    if (option == "START") {
        // state = State::DifficultySelection; // Submenu del Menu Principal
        state = State::Playing; // Temporal, para probar el cambio de estados
    } else if (option == "SETTINGS") {
        state = State::Options;
    } else if (option == "EXIT") {
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
    audio.loadSFX("assets/audio/hover_sound.ogg", "assets/audio/option_selection_sound.ogg");
    audio.updateVolumesFromConfig(mainConfig.musicVolume, mainConfig.sfxVolume);
    audio.playMusic();

    if (mainConfig.vsync){
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(0); // Desactivar límite de FPS para que VSync controle la tasa de refresco
    }
    else{
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(mainConfig.fpsLimit);
    }
    sf::Font font;
    if(!font.openFromFile("assets/fonts/04B_03__.TTF")) {
        std::cerr << "Error cargando la fuente" << std::endl;
    }

    Menu mainMenu(1280.f, 720.f);
    Settings settingsMenu(1280.f, 720.f, font);
    settingsMenu.resetTempConfig(mainConfig);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/mainMenu_background.jpeg")) {
        std::cerr << "Error cargando la imagen de fondo" << std::endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    
    sf::RectangleShape generalBackground;
    generalBackground.setSize({ 1280.f, 720.f });
    generalBackground.setFillColor(sf::Color(0, 0, 15)); // Oscurecemos la escena de fondo

    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = 1280 / static_cast<float>(textureSize.x);
    float scaleY = 720 / static_cast<float>(textureSize.y);
    backgroundSprite.setScale({scaleX, scaleY});

    sf::Texture settingsBackgroundTexture;
    if (!settingsBackgroundTexture.loadFromFile("assets/settingsMenu_background.png")) {
        std::cerr << "Error cargando la imagen de fondo de settings" << std::endl;
    }
    sf::Sprite settingsBackgroundSprite(settingsBackgroundTexture);

    State currentState = State::Menu; // Estado inicial del juego, muestra el menú principal

    Player spaceShip(640.f, 360.f, "assets/player.png"); // El jugador controla una nave espacial, que se inicializa en el centro de la pantalla con una textura específica

    Inventory shipInventory; // Inventario de la nave del jugador

    // Para probar el inventario y la interfaz de la nave, le damos algunos ítems al jugador desde el catalogo global
    auto galaxyItems = ItemFactory::loadDatabase("assets/data/items.txt");
    for (const auto& [id, itemPtr] : galaxyItems) {
        shipInventory.addItem(id, 3, itemPtr->getMaxStackSize(), itemPtr->getPrice());
    }

    TradeMenuUI tradeMenu(font); // Interfaz del menú de comercio, que se inicializa con la fuente cargada para mostrar los textos

    sf::Clock clock; // Para medir el tiempo entre frames

    sf::Vector2f mousePos; // Para almacenar la posición del mouse en coordenadas del mundo, útil para interacciones con el menú y el mundo de juego

    float alertTimer = 0.f; 
    
    // Radar de Prioridad con la misma fuente cargada
    RadarUI radarUI(font);

    // Cargamos el vector base de planetas
    std::vector<Planet> basePlanets = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets.txt");

    if (basePlanets.empty()) {
        std::cout << "[ERROR] No se cargaron planetas." << std::endl;
        return -1; 
    }

    // Creamos el Heap. 
    // IMPORTANTE: Pasamos una COPIA del primer planeta para no romper el vector planetasBase
    auto radar = std::make_unique<Heap>(Planet(basePlanets[0]));

    // Llenamos el heap con el resto (también copias)
    for (size_t i = 1; i < basePlanets.size(); ++i) {
        radar->insert(Planet(basePlanets[i]), radar->getHeapArray(), cmp);
    }

    // Inicializamos el mundo con TODO
    // Usamos std::move para pasarle la propiedad de los objetos al World
    World world(0.0f, std::move(radar), std::move(basePlanets));

    // --- Generar stock inicial para todos los planetas ---
    for (auto& planet : world.getPlanets()) {
        planet.refreshMarket(world.getCatalog());
    }

    // Ahora 'world' tiene el control de los planetas y el radar
    
    BackgroundStars bgStars;
    
    sf::Texture alertTexture;
    if (!alertTexture.loadFromFile("assets/alert_icon.png")) {
        std::cerr << "Error cargando la textura de alerta" << std::endl;
    }
    sf::Sprite alertSprite(alertTexture);
    alertSprite.setPosition({1100.f, 600.f}); 

    sf::Text planetNameText(font, "");
    planetNameText.setCharacterSize(22);
    planetNameText.setFillColor(sf::Color::White);
    planetNameText.setOutlineColor(sf::Color::Black);
    planetNameText.setOutlineThickness(1);
    planetNameText.setPosition({ 30.f, 660.f });

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

    ShipMenuUI shipMenu(font, shipMenuTexture);

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
    float travelSpeed = 400.f; 
    float shipAnimX = -100.f; // Variable para la animación horizontal

    bool showRadar = false; // Variable para controlar la visibilidad del radar
    bool aux = true; // Variable auxiliar para mostrar el radar solo una vez al inicio

    while (window.isOpen()) {
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float dt = clock.restart().asSeconds(); 
        world.setDeltaTime(dt);

        // --- LÓGICA DE EVENTOS ---
        if (currentState == State::Playing) {
            if (world.update()) {
                std::cout << "[ALERTA] Nuevo evento aleatorio en la galaxia!" << std::endl;
                alertTimer = 3.0f; // La alerta durará 3 segundos
                radarUI.update(world.getRadar()->getHeapArray());
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
        }

// --- INPUT (Depende del estado) ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == State::Menu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Soporte para Flechas + WASD
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                        mainMenu.moveUp();
                        audio.playHover();
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                        mainMenu.moveDown();
                        audio.playHover();
                    }

                    // Confirmar con Enter o Espacio
                    else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
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
                        // Si el mouse está sobre el botón actual (actualizado por updateHover)
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
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
                        settingsMenu.changeValue(1); // Cambia el valor en 5 unidades, ajustar según sea necesario
                        audio.playHover();
                        audio.setMusicVolume((float)settingsMenu.getTempMusicVolume());
                        audio.setSFXVolume((float)settingsMenu.getTempSfxVolume());
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::A || keyPressed->code == sf::Keyboard::Key::Left){
                        settingsMenu.changeValue(-1); // Cambia el valor en -5 unidades, ajustar según sea necesario
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
            else if(currentState == State::DifficultySelection) {
            // Aquí iría la lógica de input para el submenu de selección de dificultad
                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Permitir volver al menú con Escape
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }
                    
                    if(keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        // Aquí iría la lógica para confirmar la selección de dificultad y pasar a jugar
                        currentState = State::Playing; // Temporal, para probar el cambio de estados
                    }
                }
            }
            else if (currentState == State::Playing) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        if (adminShipBtn.getGlobalBounds().contains(mousePos)) {
                            audio.playClick();
                            currentState = State::ShipMenu; 
                        }
                    }
                }
                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Permitir volver al menú con Escape (Borrar despues, esto es para probar el cambio de estados)
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }
                    const auto& planetas = world.getPlanets();
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
                            audio.playClick();
                            currentState = State::TravelConfirmation; 
                        }
                    }
                }
            }
            else if (currentState == State::TravelConfirmation) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Y) {
                        shipAnimX = -100.f; // Reiniciamos posición de la animación
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
                            currentState = State::InPlanet; // Salimos de la tienda si no hay popups
                        }
                    }
                }
                // Le pasamos el evento a la clase
                tradeMenu.handleInput(*event, mousePos, shipInventory, world.getPlanets()[selectedPlanetIndex], spaceShip, world.getCatalog());
            }
            else if (currentState == State::ShipMenu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Playing;
                    }
                }
                // Le pasamos el evento a la clase
                shipMenu.handleInput(*event, mousePos, shipInventory.getUsedSlots(), shipInventory, world.getCatalog());
            }
        }

// --- UPDATE & DRAW (Depende del estado) ---
        window.clear();
        
        if (currentState == State::Menu) {
            audio.playMusic();
            window.draw(backgroundSprite);
            mainMenu.draw(window);
        } 
        else if (currentState == State::Options){
            window.draw(settingsBackgroundSprite);
            settingsMenu.draw(window);
        }
        else if (currentState == State::Playing || currentState == State::ShipMenu || currentState == State::TravelConfirmation)
        {
            audio.stopMusic();

            // window.clear(backgroundSprite.getColor()); // Limpiamos con un color sólido para evitar que el fondo parpadee al redimensionar
            window.draw(generalBackground); // Dibujamos el fondo oscuro para resaltar los elementos del juego

            sf::Vector2f playerPos = spaceShip.getPosition();
            
            bgStars.draw(window, spaceShip.getPosition());

            sf::Vector2f center(640.f, 360.f);
            sf::CircleShape sun(15.f); sun.setFillColor(sf::Color::White);
            sun.setOrigin({15.f, 15.f}); sun.setPosition(center);
            window.draw(sun);

            float time = worldClock.getElapsedTime().asSeconds(); 
            const auto& planets = world.getPlanets();

            for (size_t i = 0; i < planets.size(); ++i) {
                // Definimos la distancia según la órbita (ej: órbita 1 = 80px, órbita 10 = 350px)
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
                    targetPosition = {x, y};
                }
                else {
                    planetShape.setFillColor(sf::Color(150, 150, 150));
                }
                window.draw(planetShape);
            }

            for(auto& planet : world.getPlanets()){
                float distance = planet.getOrbit() * 33.f + 33.f;
                float speed = 0.5f / (planet.getOrbit() * 0.2f);
                float x = center.x + std::cos(time * speed) * distance;
                float y = center.y + std::sin(time * speed) * distance;

                if(planet.hasSprite()){
                    planet.getSprite()->setPosition({x, y});
        
                    // Detección de proximidad
                    sf::Vector2f planetPos(x, y);
                    sf::Vector2f playerPos = spaceShip.getPosition();
        
                    if (planet.isPointNear(playerPos, planetPos)) {
                        planet.setHighlighted(true);
                    } else {
                        planet.setHighlighted(false);
                    }
        
                // Actualizar escala suave
                planet.updateScale(dt);
        
                window.draw(*planet.getSprite());
                } else {
                    sf::CircleShape planetShape(8.f);
                    planetShape.setFillColor(sf::Color(150, 150, 150));
                    planetShape.setOrigin({8.f, 8.f});
                    planetShape.setPosition({x, y});
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

            if (alertTimer > 0) {
                window.draw(alertSprite);
            }

            window.draw(planetNameText);

            if (alertTimer > 0) window.draw(alertSprite);

            window.draw(adminShipBtn);
            window.draw(adminShipText);

            // Dibujamos el Radar encima de todo (siempre va al final para que se dibuje
            // encima de los demás elementos)
            if(aux) {
                radarUI.update(world.getRadar()->getHeapArray());
                aux = false; // Solo actualizar el radar la primera vez para mostrarlo al inicio
            }
            radarUI.draw(window);

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
                // Le pasas el inventario y el catálogo global
                shipMenu.draw(window, shipInventory, world.getGlobalCatalog());
            }
        }
        else if (currentState == State::InPlanet) {
            window.clear(backgroundSprite.getColor());
            window.draw(generalBackground);
            
            shipAnimX += 400.f * dt; // Velocidad de la animación
            if (shipAnimX > 1380.f) shipAnimX = -100.f;

            sf::Vector2f originalPos = spaceShip.getPosition();
            
            spaceShip.setPosition({shipAnimX, 500.f}); 
            spaceShip.setRotation(90.f); // Mirando a la derecha
            spaceShip.draw(window);
            spaceShip.setPosition(originalPos);
            spaceShip.setRotation(0.f); 
            // -----------------------

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
            window.clear(backgroundSprite.getColor()); // Limpiamos con un color sólido para evitar que el fondo parpadee al redimensionar
            window.draw(generalBackground); // Dibujamos el fondo oscuro para resaltar los elementos del juego
            bgStars.draw(window, spaceShip.getPosition());
            tradeMenu.draw(window, shipInventory, world.getPlanets()[selectedPlanetIndex], spaceShip.getMoney(), world.getGlobalCatalog());
            tradeMenu.update(mousePos);
        }
        window.display();
    }
    
    return 0;
}