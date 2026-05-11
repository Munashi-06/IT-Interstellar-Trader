#pragma once
#include "Systems/Inventory.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:
    Player() = default;
    Player(float x, float y, const std::string& texturePath);
    

    void draw(sf::RenderWindow& window);
    void update(float deltaTime);
    void move(sf::Vector2f direction, float deltaTime);

    void addInventoryCapacity(int extraSlots);

    bool upgradeShip();
    
    // Setters
    void setPosition(sf::Vector2f pos);
    void setRotation(float angle);
    void setMoney(float amount) { money = amount; }
    void setInventory(const Inventory& inv) { inventory = inv; }
    void setMaxOrbit(short orbit) { maxOrbit = orbit; }
    void setMinOrbit(short orbit) { minOrbit = orbit; }
    void setCurrentOrbit(short orbit) { currentOrbit = orbit; }
    void setMaxOrbitReach(short orbit) { maxOrbitReach = orbit; }
    void setMinOrbitReach(short orbit) { minOrbitReach = orbit; }
    void setHasWarpDrive(bool val) { hasWarpDrive = val; }
    void setHasAdvancedRadar(bool val) { hasAdvancedRadar = val; }
    void setEventFrequencyBonus(bool val) { hasEventFrequencyBonus = val; }
    void setEventDurationBonus(bool val) { hasEventDurationBonus = val; }
    void setIsSmuggler(bool val) { isSmuggler = val; }
    void setIsManipulator(bool val) { isManipulator = val; }
    void setDiscount(float val) { discount = val; }
    void setCorporateLicense(bool val) { corporateLicense = val; }
    void setMonopolyCharter(bool val) { monopolyCharter = val; }
    void setBlackMarketContacts(bool val) { blackMarketContacts = val; }
    void setSyndicateBoss(bool val) { syndicateBoss = val; }
    
    // Getters
    int getShipLevel() const { return shipLevel; }
    int getUpgradeCost() const;
    sf::Vector2f getPosition() const { return sprite->getPosition(); }
    float getMoney() const { return money; }
    float& getMoneyRef() { return money; }
    short getMaxOrbit() const { return maxOrbit; }
    short getMinOrbit() const { return minOrbit; }
    short getCurrentOrbit() const { return currentOrbit; }
    short getMaxOrbitReach() const { return maxOrbitReach; }
    short getMinOrbitReach() const { return minOrbitReach; }
    bool getHasWarpDrive() const { return hasWarpDrive; }
    bool getHasAdvancedRadar() const { return hasAdvancedRadar; }
    bool getEventFrequencyBonus() const { return hasEventFrequencyBonus; }
    bool getEventDurationBonus() const { return hasEventDurationBonus; }
    Inventory& getInventory() { return inventory; }
    bool gethasAdvancedRadar() const { return hasAdvancedRadar; }
    bool gethasEventFrequencyBonus() const { return hasEventFrequencyBonus; }
    bool gethasEventDurationBonus() const { return hasEventDurationBonus; }
    bool getisSmuggler() const { return isSmuggler; }
    bool getisManipulator() const { return isManipulator; }
    float getDiscount() const { return discount; }
    bool getCorporateLicense() const { return corporateLicense; }
    bool getMonopolyCharter() const { return monopolyCharter; }
    bool getBlackMarketContacts() const { return blackMarketContacts; }
    bool getSyndicateBoss() const { return syndicateBoss; }

    void levelUpShip() { 
        if (shipLevel < MAX_SHIP_LEVEL) {
            shipLevel++; 
        }
    }
        
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
    Inventory inventory; // Player's inventory

    short minOrbit = 3; // Minimum orbit the player can be in
    short maxOrbit = 4; // Maximun orbit the player can be in
    short currentOrbit = 3; // Current orbit of the player
    short minOrbitReach = 3; // The closest orbit the player can reach
    short maxOrbitReach = 5; // The farthest orbit the player can reach

    bool hasWarpDrive = false;
    bool hasAdvancedRadar = false;
    bool hasEventFrequencyBonus = false;
    bool hasEventDurationBonus = false;
    bool isSmuggler = false;
    bool isManipulator = false;

    float discount = 0.00f; // Percentage discount for trading (e.g., 0.05 for 5% discount)
    bool corporateLicense = false; // Whether the player has a corporate license for trading in high-security planets
    bool monopolyCharter = false; // Whether the player has a monopoly charter for exclusive trading rights on certain goods
    bool blackMarketContacts = false; // Whether the player has contacts in the black market for better deals on illegal goods
    bool syndicateBoss = false; // Whether the player is a syndicate boss with maximum influence in the black market
}; 