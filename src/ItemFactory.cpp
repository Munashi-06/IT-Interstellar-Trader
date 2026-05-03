#include "ItemFactory.hpp"

std::unordered_map<std::string, std::unique_ptr<Item>> ItemFactory::loadDatabase(const std::string& path) {
    std::unordered_map<std::string, std::unique_ptr<Item>> db;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        // Ignorar líneas vacías por seguridad
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name, priceS, catS, rareS;

        std::getline(ss, name, ';');
        std::getline(ss, priceS, ';');
        std::getline(ss, catS, ';');
        std::getline(ss, rareS, ';');

        // Manejo básico de errores por si hay una línea mal formateada
        try {
            float price = std::stof(priceS); // Cambiado a stof porque tus items usan float
            ItemCategory category = static_cast<ItemCategory>(std::stoi(catS));
            Rarity rare = static_cast<Rarity>(std::stoi(rareS));

            // Creamos el objeto según la categoría
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
                    // Los quest items necesitan un ID, por ahora podemos usar el mismo nombre
                    db[name] = std::make_unique<QuestItem>(name, name);
                    break;
                default:
                    std::cerr << "Categoría desconocida para el item: " << name << std::endl;
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parseando la línea: " << line << " - " << e.what() << std::endl;
        }
    }
    return db;
}