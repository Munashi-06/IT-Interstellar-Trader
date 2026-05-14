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
    std::string itemID; // ID to search in the global catalog (World Hash Table)
    int quantity;
    int maxStackSize;
    float buyPrice; // Purchase price, used to calculate profit when selling
    std::string originPlanet; // The planet where the item was bought, used to prevent buy-sell exploits
};

// Base class for items
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

    // The price may vary depending on the planet, that's why it's virtual
    virtual float getPrice() const { return basePrice; }

protected:
    std::string name;
    float basePrice;
    ItemCategory category;
    Rarity rarity;
    int maxStackSize; // Determines how many units of this item can be stacked
};

// Derived classes for each item category
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