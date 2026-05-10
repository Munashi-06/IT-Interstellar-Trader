#pragma once
#include "Entities/Player.hpp"
#include "Systems/Inventory.hpp"
#include "Entities/Planet.hpp"
#include "Entities/Item.hpp"

class TradeManager {
public:
    // Returns true if the purchase was successful, false if there is not enough money or space
    static bool buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Returns true if the sale was successful
    static bool sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
};