#include "Entities/Item.hpp"

Item::Item(std::string n, float p, ItemCategory cat, Rarity r, int maxStack)
    : name(n), basePrice(p), category(cat), rarity(r), maxStackSize(maxStack)
{}

std::string Item::getCategoryString() const {
    switch (category) {
        case ItemCategory::Illegal: return "Illegal";
        case ItemCategory::Luxury: return "Luxury";
        case ItemCategory::Resource: return "Resource";
        case ItemCategory::Technology: return "Technology";
        case ItemCategory::Food: return "Food";
        case ItemCategory::Medical: return "Medical";
        case ItemCategory::Quest: return "Quest";
        default: return "Unknown";
    }
}

std::string Item::getRarityString() const {
    switch (rarity) {
        case Rarity::Common: return "Common";
        case Rarity::Rare: return "Rare";
        case Rarity::Exotic: return "Exotic";
        case Rarity::Legendary: return "Legendary";
        case Rarity::Quest: return "Quest";
        default: return "Unknown";
    }
}

int Item::getQuality() const {
    // We assign a numerical value to quality to facilitate comparisons
    switch (rarity) {
        case Rarity::Common: return 1;
        case Rarity::Rare: return 2;
        case Rarity::Exotic: return 3;
        case Rarity::Legendary: return 4;
        case Rarity::Quest: return 5; // Quest items could be considered of maximum quality
        default: return 0;
    }
}

Resource::Resource(std::string n, float p, Rarity r)
    : Item(n, p, ItemCategory::Resource, r, 100)
{}

Technology::Technology(std::string n, float p, Rarity r)
    : Item(n, p, ItemCategory::Technology, r, 70)
{}

Luxury::Luxury(std::string n, float p, Rarity r)
    : Item(n, p, ItemCategory::Luxury, r, 50)
{}

Illegal::Illegal(std::string n, float p, Rarity r)
    : Item(n, p, ItemCategory::Illegal, r, 20)
{}

Food::Food(std::string n, float p, Rarity r)
    : Item(n, p, ItemCategory::Food, r, 100)
{}

Medic::Medic(std::string n, float p, Rarity r)
    : Item(n, p, ItemCategory::Medical, r, 40)
{}

QuestItem::QuestItem(std::string n, std::string id)
    : Item(n, 0.0, ItemCategory::Quest, Rarity::Quest, 1), questID(id)
{}

float QuestItem::getPrice() const {
    return 0.0f; // QuestItems do not have a price; their value is fulfilling the quest
}

std::string QuestItem::getQuestID() const {
    return questID;
}