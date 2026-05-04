#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <optional>

enum class ItemCategory { Resource, Technology, Luxury, Illegal, Food, Medical, Quest };
enum class Rarity { Common, Rare, Exotic, Legendary, Quest };

struct ItemStack {
    std::string itemID; // ID para buscar en el catálogo global (Tabla Hash del mundo)
    int quantity;
    int maxStackSize; // Ejemplo: Medicinas se apilan hasta 20, Motores solo 1
    float buyPrice; // Precio al que se compró, para calcular ganancias al vender
};

// Clase base para los items
class Item {
public:
    Item(std::string name, float basePrice, ItemCategory cat, Rarity rare, int maxStack);

    virtual ~Item() = default;

    // Getters
    std::string getName() const { return name; }
    ItemCategory getCategory() const { return category; }
    Rarity getRarity() const { return rarity; }
    float getBasePrice() const { return basePrice; }
    bool isIllegal() const { return category == ItemCategory::Illegal; }
    bool isQuestItem() const { return category == ItemCategory::Quest; }
    bool isLuxury() const { return category == ItemCategory::Luxury; }
    bool isResource() const { return category == ItemCategory::Resource; }
    bool isTechnology() const { return category == ItemCategory::Technology; }
    bool isFood() const { return category == ItemCategory::Food; }
    bool isMedical() const { return category == ItemCategory::Medical; }
    int getMaxStackSize() const { return maxStackSize; }
    std::string getCategoryString() const;
    std::string getRarityString() const;
    int getQuality() const;

    // El precio podría variar según el planeta, por eso es virtual
    virtual float getPrice() const { return basePrice; }

protected:
    std::string name;
    float basePrice;
    ItemCategory category;
    Rarity rarity;
    int maxStackSize; // Determina cuántas unidades de este item pueden apilarse
};

// Clases derivadas para cada categoría de item
#pragma region ClasesDerivadas
class Resource : public Item {
public:
    Resource(std::string n, float p, Rarity r);
};

class Technology : public Item {
public:
    Technology(std::string n, float p, Rarity r);
};

class Luxury : public Item {
public:
    Luxury(std::string n, float p, Rarity r);
};

class Illegal : public Item {
public:
    Illegal(std::string n, float p, Rarity r);
};

class Food : public Item {
public:
    Food(std::string n, float p, Rarity r);
};

class Medic : public Item {
public:
    Medic(std::string n, float p, Rarity r);
};

class QuestItem : public Item {
private:
    std::string questID;
public:
    QuestItem(std::string n, std::string id);

    float getPrice() const override;
    
    std::string getQuestID() const;
};
#pragma endregion