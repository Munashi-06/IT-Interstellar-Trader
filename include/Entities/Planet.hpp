#pragma once
#include "Entities/Item.hpp"
#include "DataStructures/StockSorter.hpp"
#include <SFML/Graphics.hpp>


static int MAX_STOCK = 100; // Cantidad máxima total de items que un planeta puede tener en su mercado

enum class PlanetEvent { None, War, Plague, TechBoom, Famine, Piracy };

class Planet {
private:
    std::string name;
    std::string description;
    PlanetEvent currentEvent = PlanetEvent::None;
    int EVENT_DURATION;
    std::vector<std::optional<ItemStack>> localStock; // Local stock of the planet, mapped by item ID

    int orbit; // Represents the planet's orbit, between 1 and 10 (1 is closest to the star, 10 is farthest)
    int techLevel; // Tech level of the planet, could influence the prices and types of items available
    int securityLevel; // Security level of the planet, could influence the presence of illegal items and their price
    int resourceAbundance; // Abundance of natural resources, could influence the supply of Resource category items
    int luxuryDemand; // Demand for luxury goods, could influence the supply and price of Luxury category items
    int moonCount; // Number of moons, could influence the presence of space stations and their item supply
    int medicalTech; // Level of medical technology, could influence the supply and price of Medical category items
    std::unique_ptr<sf::Texture> texture; // Textura del planeta
    std::unique_ptr<sf::Sprite> sprite; // Sprite del planeta
    float baseScale = 1.0f;
    bool highlighted = false;

public:
    Planet();
    Planet(std::string n, std::string d, int orb, int tech, int sec, int res, int lux, int moon, int med);
    Planet(const Planet& other); // Constructor for copy
    Planet& operator=(const Planet& other); // Assignment operator

    // Getters y Setters
    std::string getName() const;
    std::string getDescription() const;
    PlanetEvent getEvent() const;
    std::string getEventName() const;
    void setEvent(PlanetEvent e);
    int getEventDuration() const;
    void setEventDuration(int duration);
    void decreaseEventDuration();
    int getOrbit() const;
    int getTechLevel() const;
    int getSecurityLevel() const;
    int getResourceAbundance() const;
    int getLuxuryDemand() const;
    int getMoonCount() const;
    int getMedicalTech() const;
    float getBaseItemPrice(const std::string& itemID, const std::unordered_map<std::string, std::unique_ptr<Item>>& globalCatalog) const;
    float getLocalBasePrice(const std::string& itemID, const std::unordered_map<std::string, std::unique_ptr<Item>>& globalCatalog) const;
    const std::vector<std::optional<ItemStack>>& getLocalStock() const noexcept { return localStock; }
    sf::Sprite* getSprite() { return sprite.get(); }
    const sf::Sprite* getSprite() const { return sprite.get(); }
    void setHighlighted(bool h);

    void refreshMarket(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog); // Updates local stock based on planet specialization and current events
    void restockMarket(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog, int shipLevel); // Restocks items based on supply/demand and planet characteristics
    float getVisibilityPercent(Rarity rarity, int shipLevel) const; // Determines the percentage chance of an item of a certain rarity being visible in the market, based on planet characteristics and ship's scanner level
    bool canBuyItem(const Item& item) const; // Lógica de "necesito esto?"
    float getItemPrice(const std::string& itemID, const std::unordered_map<std::string, std::unique_ptr<Item>>& globalCatalog) const; // Calculates the price of an item based on supply/demand and current events
    bool addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice);
    void removeItem(const std::string& itemID, int qty);
    void sortByName(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByCategory(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByQuality(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByPrice(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByQuantity(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

    void loadTexture();
    bool hasSprite() const { return sprite != nullptr; }
    bool isHighlighted() const { return highlighted; }
    bool isPointNear(const sf::Vector2f& point, const sf::Vector2f& planetPos) const;
    void updateScale(float deltaTime);
};