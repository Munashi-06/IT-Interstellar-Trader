#include "Menu.hpp"
#include "Player.hpp"

enum class State {
    Menu,
    DifficultySelection,
    Playing,
    Pause,
    Options,
    GameOver
};

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
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "IT: Interstellar Trader");
    window.setVerticalSyncEnabled(true);
    Menu mainMenu(1280.f, 720.f);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/mainMenu_background.jpg")) {
        std::cerr << "Error cargando la imagen de fondo" << std::endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = 1280 / static_cast<float>(textureSize.x);
    float scaleY = 720 / static_cast<float>(textureSize.y);
    backgroundSprite.setScale({scaleX, scaleY});

    State currentState = State::Menu;

    Player player(640.f, 360.f, "assets/player.png");

    sf::Clock clock; // Para medir el tiempo entre frames

    sf::Vector2f mousePos; 

    while (window.isOpen()) {
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float deltaTime = clock.restart().asSeconds(); // Tiempo desde el último frame

        // 1. INPUT (Depende del estado)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == State::Menu) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Soporte para Flechas + WASD
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) 
                        mainMenu.moveUp();
                    if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) 
                        mainMenu.moveDown();
                    
                    // Confirmar con Enter o Espacio
                    if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
                    }
                }

                // Detectar Click del Mouse
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        // Si el mouse está sobre el botón actual (actualizado por updateHover)
                        ejecuteAction(mainMenu.getSelectedOption(), currentState, window);
                    }
                }
            }

            if(currentState == State::DifficultySelection) {
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
                    
                    player.move(dir, deltaTime);
                    player.update(deltaTime);
                }
                // Aquí iría la lógica de input para el juego
            }
        }

        // 2. UPDATE & DRAW
        window.clear();
        
        if (currentState == State::Menu) {
            mainMenu.updateHover(mousePos);
            window.draw(backgroundSprite);
            mainMenu.draw(window);
        } 
        else if (currentState == State::Playing) {
            window.clear(sf::Color(0, 0, 20));
            player.update(deltaTime);
            player.draw(window);
            // world.update() y world.draw(window) irían aquí
        }

        window.display();
    }

    return 0;
}