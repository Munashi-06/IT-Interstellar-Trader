#include "Systems/ItemFactory.hpp"

std::unordered_map<std::string, std::unique_ptr<Item>> ItemFactory::loadDatabase(const std::string& path) {
    std::unordered_map<std::string,
    std::unique_ptr<Item>> db;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        // Ignore empty lines for safety
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name, priceS, catS, rareS;

        std::getline(ss, name, ';');
        std::getline(ss, priceS, ';');
        std::getline(ss, catS, ';');
        std::getline(ss, rareS, ';');

        // Basic error handling in case of a malformed line
        try {
            float price = std::stof(priceS); // Changed to stof because your items use float
            ItemCategory category = static_cast<ItemCategory>(std::stoi(catS));
            Rarity rare = static_cast<Rarity>(std::stoi(rareS));

            // Create the object according to the category
            switch (category) {
                case ItemCategory::Resource:
                    db[name] = std::make_unique<Resource>(name, price, rare);
                    break;
                case ItemCategory::Technology:
                    db[name] = std::make_unique<Technology>(name, price, rare);
                    break;
                case ItemCategory::Luxury:
                    db[name] = std::make_unique<Luxury>(name, price, rare);
                    break;
                case ItemCategory::Illegal:
                    db[name] = std::make_unique<Illegal>(name, price, rare);
                    break;
                case ItemCategory::Food:
                    db[name] = std::make_unique<Food>(name, price, rare);
                    break;
                case ItemCategory::Medical:
                    db[name] = std::make_unique<Medic>(name, price, rare);
                    break;
                case ItemCategory::Quest:
                    // Quest items need an ID, for now we can use the same name
                    db[name] = std::make_unique<QuestItem>(name, name);
                    break;
                default:
                    std::cerr << "Unknown category for item: " << name << std::endl;
                    break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
        }
    }
    return db;
}