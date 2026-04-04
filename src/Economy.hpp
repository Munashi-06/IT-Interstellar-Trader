#pragma once
#include "Planet.hpp"
#include "Item.hpp"

class Economy {
public:
    Economy() = default;

    // Esta es la función principal: Calcula el precio de UN item en UN planeta
    static double calculatePrice(const Item& item, const Planet& planet);

    // Esta función genera la lista de items que un planeta tiene a la venta
    // Devuelve un vector de nombres de items
    std::vector<std::string> generateMarketStock(const Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
};