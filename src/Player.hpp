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
private:
    std::unique_ptr<sf::Sprite> sprite; 
    sf::Texture texture;
    sf::RectangleShape hitbox; // Para futuras colisiones
    
    sf::Vector2f velocity{0.f, 0.f};
    float acceleration = 800.f; // Qué tan rápido gana velocidad
    float drag = 0.987f;         // Fricción espacial (0.99 = muy resbaladizo)
    float maxSpeed = 200.f;
    
    float targetRotation = 0.f;
    float rotationSpeed = 4.f;  // Qué tan suave gira (más alto = más rápido)

    float money = 50.f; // Dinero del jugador
}; 