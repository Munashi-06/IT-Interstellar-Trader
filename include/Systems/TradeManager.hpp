#pragma once
#include "Entities/Player.hpp"
#include "Systems/Inventory.hpp"
#include "Entities/Planet.hpp"
#include "Entities/Item.hpp"

class TradeManager {
public:
    // Retorna true si la compra fue exitosa, false si no hay dinero o espacio
    static bool buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Retorna true si la venta fue exitosa
    static bool sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
};