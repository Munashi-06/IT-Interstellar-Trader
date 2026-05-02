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

/*
        EJEMPLO DE USO DE LA CLASE ITEM, INVENTARIO Y FACTORY
        (ESTO NO VA EN EL MAIN, SOLO ES UN EJEMPLO DE CÓMO USARLAS
        REALMENTE IRIA EN LA CLASE WORLD O EN LA LÓGICA DE NEGOCIO DEL JUEGO,
        DEPENDIENDO DE CÓMO ORGANICEMOS EL CÓDIGO)

// 1. Cargamos el catálogo global
auto galaxyItems = ItemFactory::loadDatabase("assets/data/items.txt");

// 2. El jugador compra "Agua Purificada"
std::string itemABuscar = "Agua Purificada";

if (galaxyItems.count(itemABuscar)) {
    // IMPORTANTE: Creamos una COPIA para el inventario del jugador
    // (Podrías implementar un método virtual clone() en Item para esto)
    auto& prototipo = galaxyItems[itemABuscar];
    playerInventory.addItem(std::make_unique<Resource>(prototipo->getName(), prototipo->getPrice()), 1);
}
*/

int main() {
    mainConfig.loadFromSavedFile("config.txt");
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "IT: Interstellar Trader");
    sf::Clock worldClock;

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

    sf::Music music;
    if (!music.openFromFile("assets/audio/undertale_dogsong.ogg")){ //musica de fondo del menu para probar la funcionalidad de volumen en settings
        std::cerr << "Error cargando la música" << std::endl;
    }
    music.setLooping(true);
    music.setVolume((float)mainConfig.musicVolume);

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

    sf::Clock clock; // Para medir el tiempo entre frames

    sf::Vector2f mousePos;

    // --- LÓGICA DE INICIALIZACIÓN DEL MUNDO ---

    // 1. Cargamos el vector base de planetas
    std::vector<Planet> basePlanets = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets.txt");

    if (basePlanets.empty()) {
        std::cout << "[ERROR] No se cargaron planetas." << std::endl;
        return -1; // Error de carga
    }

    // 2. Creamos el Heap. 
    // IMPORTANTE: Pasamos una COPIA del primer planeta para no romper el vector planetasBase
    auto radar = std::make_unique<Heap>(Planet(basePlanets[0]));

    // 3. Llenamos el heap con el resto (también copias)
    for (size_t i = 1; i < basePlanets.size(); ++i) {
        radar->insert(Planet(basePlanets[i]), radar->getHeapArray(), cmp);
    }

    // 4. Inicializamos el mundo con TODO
    // Usamos std::move para pasarle la propiedad de los objetos al World
    World world(0.0f, std::move(radar), std::move(basePlanets));

    // --- FIN DE LA LÓGICA DE INICIALIZACIÓN ---

    // Ahora 'world' tiene el control de los planetas y el radar
    

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
    
    // imprimir en consola los planetas cargados para verificar que se han cargado correctamente
    std::cout << "Planetas cargados en el mundo:" << std::endl;
    for (const auto& planeta : world.getPlanets()) {
        std::cout << "Nombre: " << planeta.getName() << ", Orbita: " << planeta.getOrbit() << std::endl;
    }

    // Cargar la textura de alerta para eventos nuevos
    sf::Texture alertTexture;
    if (!alertTexture.loadFromFile("assets/alert_icon.png")) {
        std::cerr << "Error cargando la textura de alerta" << std::endl;
    }
    sf::Sprite alertSprite(alertTexture);
    alertSprite.setPosition({1100.f, 600.f}); // Esquina inferior derecha

    float alertTimer = 0.f; // Para que la imagen desaparezca después de unos segundos
    
    // Radar de Prioridad con la misma fuente cargada
    RadarUI radarUI(font);

#pragma region Bucle principal

    int musicPlaying = 0; // Controla cuando se reproduce la musica del menu (da problemas, revisar)
    bool showRadar = false; // Variable para controlar la visibilidad del radar
    bool aux = true; // Variable auxiliar para mostrar el radar solo una vez al inicio

    while (window.isOpen()) {
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float dt = clock.restart().asSeconds(); // Tiempo entre frames
        world.setDeltaTime(dt);

        // --- LÓGICA DE EVENTOS ---
        if (currentState == State::Playing) {
            if (world.update()) {
                std::cout << "[ALERTA] Nuevo evento aleatorio en la galaxia!" << std::endl;
                alertTimer = 3.0f; // La alerta durará 3 segundos
                radarUI.update(world.getRadar()->getHeapArray());
            }
            
            if (alertTimer > 0) alertTimer -= dt;
        }

        // 1. INPUT (Depende del estado)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == State::Menu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Soporte para Flechas + WASD
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) 
                        mainMenu.moveUp();
                        hoverSound.play();
                    if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) 
                        mainMenu.moveDown();
                        hoverSound.play();
                    
                    // Confirmar con Enter o Espacio
                    if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
                        clickSound.play();
                    }
                }
                
                if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2f mPos = window.mapPixelToCoords(mouseMoved->position);
                    if(mainMenu.updateHover(mPos)){
                        hoverSound.stop(); //detiene el sonido anterior si se cambia de opción rápidamente (opcional)
                        hoverSound.play();
                    }
                }
                // Detectar Click del Mouse
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        // Si el mouse está sobre el botón actual (actualizado por updateHover)
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
                        clickSound.play();
                    }
                }
            }

            else if (currentState == State::Options){
                if (event->is<sf::Event::Closed>()) window.close();
                
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
                            settingsMenu.changeValue(1); // Cambia el valor en 5 unidades, ajustar según sea necesario
                            hoverSound.play();
                        } else if (keyPressed->code == sf::Keyboard::Key::A || keyPressed->code == sf::Keyboard::Key::Left){
                            settingsMenu.changeValue(-1); // Cambia el valor en -5 unidades, ajustar según sea necesario
                            hoverSound.play();
                        }
                    }

                    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                        if(mouseEvent->button == sf::Mouse::Button::Left){
                            sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                            clickSound.play();
                            settingsMenu.handleMouseClick(mousePos);
                            std::string opt = settingsMenu.getSelectedOption();
                            if(opt == "APPLY"){
                                settingsMenu.applySettings(window, mainConfig);
                                music.setVolume((float)mainConfig.musicVolume);
                            }
                            else if(opt == "BACK"){
                                music.setVolume((float)mainConfig.musicVolume);
                                hoverSound.setVolume((float)mainConfig.sfxVolume);
                                clickSound.setVolume((float)mainConfig.sfxVolume);
                                settingsMenu.resetTempConfig(mainConfig);
                                currentState = State::Menu;
                            }
                        }
                    }

                    if (event->is<sf::Event::MouseButtonReleased>()){
                        settingsMenu.releaseSlider();
                    }

                    if (auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()){
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);
                        if(settingsMenu.updateHover(mousePos)){
                            hoverSound.stop(); //detiene el sonido anterior si se cambia de opción rápidamente (opcional)
                            hoverSound.play();
                        }
                        
                        settingsMenu.handleMouseMove(mousePos);
                        music.setVolume((float)settingsMenu.getTempMusicVolume());
                        hoverSound.setVolume((float)settingsMenu.getTempSfxVolume());
                        clickSound.setVolume((float)settingsMenu.getTempSfxVolume());
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

            music.setVolume((float)settingsMenu.getTempMusicVolume());
            hoverSound.setVolume((float)settingsMenu.getTempSfxVolume());
            clickSound.setVolume((float)settingsMenu.getTempSfxVolume());
        
            if(currentState == State::Playing) {

                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Permitir volver al menú con Escape (Borrar despues, esto es para probar el cambio de estados)
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }
                    // Aquí iría la lógica de input para el juego
                    sf::Vector2f dir(0.f, 0.f);
                    if (keyPressed->code == sf::Keyboard::Key::W) dir.y -= 1;
                    if (keyPressed->code == sf::Keyboard::Key::S) dir.y += 1;
                    if (keyPressed->code == sf::Keyboard::Key::A) dir.x -= 1;
                    if (keyPressed->code == sf::Keyboard::Key::D) dir.x += 1;
                    
                    player.move(dir, world.getDeltaTime());
                    player.update(world.getDeltaTime());
                }
                // Aquí iría la lógica de input para el juego
            }
        }

        // 2. UPDATE & DRAW
        window.clear();
        
        if (currentState == State::Menu) {
            if(musicPlaying == 1 || musicPlaying == 3){
                musicPlaying = 0;
            }
            window.draw(backgroundSprite);
            mainMenu.draw(window);
        } 
        else if (currentState == State::Options){
            window.draw(settingsBackgroundSprite);
            settingsMenu.draw(window);
        }
        else if (currentState == State::Playing) {
            if(musicPlaying == 0 || musicPlaying == 2){
                musicPlaying = 1;
            }
            window.clear(sf::Color(0, 0, 15));
            sf::Vector2f playerPos = player.getPosition();
            sf::RenderStates states;

            sf::Transform tFar;
            tFar.translate(-playerPos*0.2f);
            states.transform = tFar;
            window.draw(starsFar, states);

            sf::Transform tMid;
            tMid.translate(-playerPos*0.4f);
            states.transform = tMid;
            window.draw(starsMid, states);

            sf::Transform tNear;
            tNear.translate(-playerPos*0.7f);
            states.transform = tNear;
            window.draw(starsNear, states);

            sf::Vector2f center(640.f, 360.f);
            sf::CircleShape sun(15.f);
            sun.setFillColor(sf::Color::White);
            sun.setOrigin({15.f, 15.f});
            sun.setPosition(center);
            window.draw(sun);

            float time = worldClock.getElapsedTime().asSeconds(); 
            const auto& planets = world.getPlanets();

            for (const auto& planet : planets) {
            // Definimos la distancia según la órbita (ej: órbita 1 = 80px, órbita 10 = 350px)
                float distance = planet.getOrbit() * 33.f + 33.f;
                float speed = 0.5f / (planet.getOrbit() * 0.2f);
                float x = center.x + std::cos(time * speed) * distance;
                float y = center.y + std::sin(time * speed) * distance;

                sf::CircleShape planetShape(8.f);
                planetShape.setFillColor(sf::Color(150, 150, 150)); // Gris base
                planetShape.setOrigin({8.f, 8.f});
                planetShape.setPosition({x, y});
    
                window.draw(planetShape);
            }

            player.update(world.getDeltaTime());
            player.draw(window);

            if (alertTimer > 0) {
                window.draw(alertSprite);
            }

            // Dibujamos el Radar encima de todo (siempre va al final para que se dibuje
            // encima de los demás elementos)
            if(aux) {
                radarUI.update(world.getRadar()->getHeapArray());
                aux = false; // Solo actualizar el radar la primera vez para mostrarlo al inicio
            }
            radarUI.draw(window);
        }
         
        if(musicPlaying == 0){
            music.play();
            musicPlaying = 2;
        }
        else if(musicPlaying == 1){
            music.stop();
            musicPlaying = 3;
        }
        window.display();
    }
    return 0;
}

#pragma endregion