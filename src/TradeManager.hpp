#pragma once
#include "Player.hpp"
#include "Inventory.hpp"
#include "Planet.hpp"
#include "Item.hpp"

class TradeManager {
public:
    // Retorna true si la compra fue exitosa, false si no hay dinero o espacio
    static bool buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Retorna true si la venta fue exitosa
    static bool sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
};