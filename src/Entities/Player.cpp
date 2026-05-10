#include "Entities/Player.hpp"
#include <cmath>


Player::Player(float x, float y, const std::string& texturePath) 
    : hitbox({30.f, 35.f})
{
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Error loading player texture");
    }
    else {
        // 1. Link the texture HERE, once loaded
        sprite = std::make_unique<sf::Sprite>(texture);
        sprite->setPosition({x, y});

        // 2. Reset the texture Rect (just in case)
        sprite->setTextureRect(sf::IntRect({0, 0}, { (int)texture.getSize().x, (int)texture.getSize().y }));

        sf::Vector2u size = texture.getSize();
        
        // 3. The origin must be half the real size of the image
        sprite->setOrigin({ static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f });

        // 4. Rescaling
        float rescale = 60.f;
        float scale = rescale / static_cast<float>(size.x);
        sprite->setScale({ scale, scale });
        
        sprite->setPosition({ x, y });
    }

    // Hitbox configuration
    
    // Red outline for debugging (remove in production)
    // hitbox.setOutlineColor(sf::Color::Red);
    // hitbox.setOutlineThickness(2);

    hitbox.setFillColor(sf::Color::Transparent);
    // Center the hitbox as well to match the sprite
    hitbox.setOrigin(hitbox.getSize() / 2.f);
    hitbox.setPosition(sprite->getPosition());
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(hitbox); // Draw the hitbox for debugging (remove in production)
    window.draw(*sprite);
}

void Player::setRotation(float angle) {
    if (sprite) {
        sprite->setRotation(sf::degrees(angle)); 
    }
}

void Player::update(float deltaTime) {
    if (!sprite) return;

    // Subtle space friction
    velocity *= std::pow(drag, deltaTime * 60.f);

    // Interpolated Rotation (Smooth)
    float currentRot = sprite->getRotation().asDegrees();
    float diff = targetRotation - currentRot;
    
    // Normalize angle to always rotate via the shortest path
    while (diff > 180) diff -= 360;
    while (diff < -180) diff += 360;

    // Apply smooth rotation
    sprite->rotate(sf::degrees(diff * rotationSpeed * deltaTime));

    // Move the ship with the accumulated velocity
    sprite->move(velocity * deltaTime);

    hitbox.setPosition(sprite->getPosition());
}

void Player::move(sf::Vector2f direction, float deltaTime) {
    // 1. Rotation: We only change the rotation target if there is input
    if (direction.x != 0 || direction.y != 0) {
        targetRotation = (std::atan2(direction.y, direction.x) * 180.f / 3.141592f) + 90.f;
        
        // 2. Acceleration: Instead of using 'direction' (input), 
        // we use the current angle of the sprite to push the ship.
        float angleRad = (sprite->getRotation().asDegrees() - 90.f) * 3.141592f / 180.f;
        sf::Vector2f thrustDir(std::cos(angleRad), std::sin(angleRad));

        velocity += thrustDir * acceleration * deltaTime;
    }
}