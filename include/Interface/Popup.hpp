#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>

class Popup {
private:
    sf::RectangleShape background;
    std::vector<sf::Text> messageLines;
    sf::Text promptText;
    bool active;
    float autoCloseTimer;
    const sf::Font& font;

public:
    Popup(const sf::Font& f);
    
    void show(const std::string& message, float timeToClose = 0.f);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void hide();
    
    bool isActive() const { return active; }
    void handleInput(sf::Keyboard::Key key);
    void handleMouseClick(const sf::Vector2f& mousePos);
};