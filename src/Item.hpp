#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

enum class ItemCategory { Resource, Technology, Luxury, Illegal, Food, Medical, Quest };
enum class Rarity { Common, Rare, Exotic, Legendary, Quest };

// Clase base para los items
class Item {
public:
    Item(std::string name, double basePrice, ItemCategory cat, Rarity rare);

    virtual ~Item() = default;

    // Getters
    std::string getName() const { return name; }
    ItemCategory getCategory() const { return category; }
    Rarity getRarity() const { return rarity; }
    bool isIllegal() const { return category == ItemCategory::Illegal; }
    bool isQuestItem() const { return category == ItemCategory::Quest; }
    bool isLuxury() const { return category == ItemCategory::Luxury; }
    bool isResource() const { return category == ItemCategory::Resource; }
    bool isTechnology() const { return category == ItemCategory::Technology; }
    bool isFood() const { return category == ItemCategory::Food; }
    bool isMedic() const { return category == ItemCategory::Medical; }
    
    // El precio podría variar según el planeta, por eso es virtual
    virtual double getPrice() const { return basePrice; }

protected:
    std::string name;
    double basePrice;
    ItemCategory category;
    Rarity rarity;
};

// Clases derivadas para cada categoría de item
class Resource : public Item {
public:
    Resource(std::string n, double p, Rarity r);
};

class Technology : public Item {
public:
    Technology(std::string n, double p, Rarity r);
};

class Luxury : public Item {
public:
    Luxury(std::string n, double p, Rarity r);
};

class Illegal : public Item {
public:
    Illegal(std::string n, double p, Rarity r);
};

class Food : public Item {
public:
    Food(std::string n, double p, Rarity r);
};

class Medic : public Item {
public:
    Medic(std::string n, double p, Rarity r);
};

class QuestItem : public Item {
private:
    std::string questID;
public:
    QuestItem(std::string n, std::string id);

    double getPrice() const override;
    
    std::string getQuestID() const;
};