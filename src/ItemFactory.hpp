#pragma once
#include "Item.hpp"
#include "StockSorter.hpp"

class ItemFactory {
public:
    // Carga todos los items posibles del juego en un "catálogo"
    static std::unordered_map<std::string, std::unique_ptr<Item>> loadDatabase(const std::string& path);
};