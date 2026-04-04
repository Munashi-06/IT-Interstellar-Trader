#include "Item.hpp"

Item::Item(std::string n, double p, ItemCategory cat, Rarity r)
    : name(n), basePrice(p), category(cat), rarity(r)
{}

Resource::Resource(std::string n, double p, Rarity r)
    : Item(n, p, ItemCategory::Resource, r)
{}

Technology::Technology(std::string n, double p, Rarity r)
    : Item(n, p, ItemCategory::Technology, r)
{}

Luxury::Luxury(std::string n, double p, Rarity r)
    : Item(n, p, ItemCategory::Luxury, r)
{}

Illegal::Illegal(std::string n, double p, Rarity r)
    : Item(n, p, ItemCategory::Illegal, r)
{}

Food::Food(std::string n, double p, Rarity r)
    : Item(n, p, ItemCategory::Food, r)
{}

Medic::Medic(std::string n, double p, Rarity r)
    : Item(n, p, ItemCategory::Medical, r)
{}

QuestItem::QuestItem(std::string n, std::string id)
    : Item(n, 0.0, ItemCategory::Quest, Rarity::Quest), questID(id)
{}

double QuestItem::getPrice() const {
    return 0.0f; // Los QuestItems no tienen precio, su valor es cumplir la misión
}

std::string QuestItem::getQuestID() const {
    return questID;
}