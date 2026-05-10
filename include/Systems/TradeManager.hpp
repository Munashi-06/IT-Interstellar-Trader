#pragma once
#include "Entities/Player.hpp"
#include "Systems/Inventory.hpp"
#include "Entities/Planet.hpp"
#include "Entities/Item.hpp"

class TradeManager {
public:
    static bool buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    static bool sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

    // Helper functions to calculate final prices applying player's Trading Tree skills
    static float getFinalBuyPrice(const Item& item, float basePlanetPrice, const Player& player);
    static float getFinalSellPrice(const Item& item, float basePlanetPrice, const Player& player);
};