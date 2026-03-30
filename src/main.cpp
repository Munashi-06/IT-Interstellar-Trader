#include "Menu.hpp"

enum class State {
    Menu,
    Playing,
    Options,
    GameOver
};

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
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "IT: Interstellar Trader");
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

    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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

            if(currentState == State::Playing) {
                if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    // Permitir volver al menú con Escape (Borrar despues, esto es para probar el cambio de estados)
                    if(keyPressed->code == sf::Keyboard::Key::Escape) {
                        currentState = State::Menu;
                    }

                    // Aquí iría la lógica de input para el juego
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
            // world.update() y world.draw(window)
            window.clear(sf::Color(0, 0, 20)); 
        }

        window.display();
    }

    return 0;
}