#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:
    Player() = default;
    Player(float x, float y, const std::string& texturePath);
    

    void draw(sf::RenderWindow& window);
    void update(float deltaTime);
    void move(sf::Vector2f direction, float deltaTime);
    sf::Vector2f getPosition() const {return sprite->getPosition();}
    void setPosition(sf::Vector2f pos) { sprite->setPosition(pos); hitbox.setPosition(pos); }
    void setRotation(float angle);
    float getMoney() const { return money; }
    void setMoney(float amount) { money = amount; }

    int getShipLevel() const { return shipLevel; }
    bool upgradeShip();
    int getUpgradeCost() const;
private:
    std::unique_ptr<sf::Sprite> sprite; 
    sf::Texture texture;
    sf::RectangleShape hitbox; // For future collisions
    
    sf::Vector2f velocity{0.f, 0.f};
    float acceleration = 800.f; // How fast does it gain speed
    float drag = 0.987f;        // Space friction (0.99 = very slippery)
    float maxSpeed = 200.f;
    
    float targetRotation = 0.f;
    float rotationSpeed = 4.f;  // How smoothly it spins (higher = faster)

    float money = 50.f; // Player money
    int shipLevel = 1;
    static constexpr int MAX_SHIP_LEVEL = 3;
}; 