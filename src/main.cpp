#include <SFML/Graphics.hpp>

int main() {
    // SFML 3 usa sf::VideoMode({ancho, alto})
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Interstellar Trader - SFML 3");
    
    sf::CircleShape player(25.f);
    player.setFillColor(sf::Color::Green);
    player.setPosition({375.f, 275.f}); // SFML 3 usa sf::Vector2f o llaves {}

    float speed = 0.5f;

    while (window.isOpen()) {
        // En SFML 3, pollEvent devuelve un std::optional
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Movimiento actualizado para SFML 3
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  player.move({-speed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) player.move({speed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    player.move({0.f, -speed});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  player.move({0.f, speed});

        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}