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

void Player::setPosition(sf::Vector2f pos) {
    sprite->setPosition(pos);
    hitbox.setPosition(pos);
}

void Player::setRotation(float angle) {
    if (sprite) {
        sprite->setRotation(sf::degrees(angle));
    }
}

void Player::update(float deltaTime) {
    if (!sprite) return;
    hitbox.setPosition(sprite->getPosition());
}

void Player::travelTo(sf::Vector2f target, float dt, float travelSpeed) {
    sf::Vector2f currentPos = getPosition();
    sf::Vector2f direction = target - currentPos;
    float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (dist > 1.5f) {
        float angleRadians = std::atan2(direction.y, direction.x);
        float angleDegrees = angleRadians * 180.f / 3.14159265f;
        setRotation(angleDegrees + 90.f);

        direction /= dist; 
        float moveDistance = travelSpeed * dt;

        if (moveDistance > dist) {
            setPosition(target);
        }
        else {
            setPosition(currentPos + direction * moveDistance);
        }
    }
    else {
        setPosition(target);
    }
}

void Player::addInventoryCapacity(int extraSlots){
        inventory.upgradeStorage(extraSlots);
}

bool Player::upgradeShip(){
    if(shipLevel >= MAX_SHIP_LEVEL) {
        std::cout << "Nave al máximo nivel!" << std::endl;
        return false;
    }

    int cost = getUpgradeCost();
    if (money < cost){
        std::cout << "You don't have enough money for the upgrade" << std::endl;
        return false;
    }
    money -= cost;
    shipLevel++;
    std::cout << "Ship upgraded to level " << shipLevel << "!" << std::endl;
    return true;
}

int Player::getUpgradeCost() const {
    switch (shipLevel){
        case 1: return 500;
        case 2: return 2000;
        default: return 0;
    }
}

void Player::resetToDefaults() {
    money = 50.f;
    shipLevel = 1;
    
    minOrbit = 3;
    maxOrbit = 4;
    currentOrbit = 3;
    minOrbitReach = 3;
    maxOrbitReach = 5;

    hasWarpDrive = false;
    hasAdvancedRadar = false;
    hasEventFrequencyBonus = false;
    hasEventDurationBonus = false;
    isSmuggler = false;

    discount = 0.00f;
    corporateLicense = false;
    monopolyCharter = false;
    blackMarketContacts = false;
    syndicateBoss = false;

    inventory.resetCapacity();
}

void Player::clearInv() {
    inventory.clearAll();
}