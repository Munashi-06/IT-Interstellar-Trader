#include "ItemFactory.hpp"

std::unordered_map<std::string, std::unique_ptr<Item>> ItemFactory::loadDatabase(const std::string& path) {
    std::unordered_map<std::string, std::unique_ptr<Item>> db;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, priceS, catS, rareS;

        std::getline(ss, name, ';');
        std::getline(ss, priceS, ';');
        std::getline(ss, catS, ';');
        std::getline(ss, rareS, ';');

        double price = std::stod(priceS);
        ItemCategory cat = static_cast<ItemCategory>(std::stoi(catS));
        Rarity rare = static_cast<Rarity>(std::stoi(rareS));

        // Creamos el objeto según la categoría
        if (cat == ItemCategory::Resource) 
            db[name] = std::make_unique<Resource>(name, price, rare);
        else if (cat == ItemCategory::Food)
            db[name] = std::make_unique<Food>(name, price, rare);
        // ... (repetir para Medical, Technology, etc.)
    }
    return db;
}