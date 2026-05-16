#pragma once
#include <SFML/Graphics.hpp>
#include "Systems/Inventory.hpp"
#include "Systems/UpgradeManager.hpp"
#include "Entities/Item.hpp"
#include "Entities/Player.hpp"
#include "Entities/Planet.hpp"
#include "Core/State.hpp"
#include <vector>
#include <string>

// Enum to know which column we are sorting by
enum class SortColumn { Name, Category, Quality, Price, Quantity };

class ShipMenuUI {
private:
    sf::RectangleShape background;
    sf::RectangleShape tableBackground;
    sf::Sprite shipPreview;
    sf::Text titleText, escText;
    
    // Upgrade button
    sf::RectangleShape upgradeBtn;
    sf::Text upgradeText;

    // Save & Exit Buttons
    sf::RectangleShape closeBtn;
    sf::Text closeText;
    sf::RectangleShape saveExitBtn;
    sf::Text saveExitText;

    // Table header texts
    sf::Text headerName, headerCategory, headerQuality, headerPrice;
    
    // --- SCROLL ---
    int startIndex = 0; // Index of the first item to draw
    int maxVisibleItems = 10; // How many items fit on screen at once
    sf::RectangleShape scrollTrack; // Scroll bar background
    sf::RectangleShape scrollThumb; // The moving bar
    // ------------------------

    // Ship Info
    sf::Text slotsText, money, minAndMaxOrbit, minAndMaxOrbitReach;

    SortColumn currentSort = SortColumn::Name;
    bool sortAscending = true;
    
    const sf::Font& font; // Reference to the global font

public:
    ShipMenuUI(const sf::Font& f, const sf::Texture& shipTex);
    
    // Receives the inventory and the catalog to build the table
    void draw(sf::RenderWindow& window, const Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Handles clicks (for sorting headers or upgrade button)
    void handleInput(const sf::Event& event, const sf::Vector2f& mousePos, int totalItems, Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog, State& currentState, Player& player, UpgradeManager& upgrades, const std::vector<Planet>& planets);
    
    // For button hover
    void update(const sf::Vector2f& mousePos);
};