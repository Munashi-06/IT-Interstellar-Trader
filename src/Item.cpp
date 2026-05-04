#include "Item.hpp"

Item::Item(std::string n, float p, ItemCategory cat, Rarity r, int maxStack)
    : name(n), basePrice(p), category(cat), rarity(r), maxStackSize(maxStack)
{}

std::string Item::getCategoryString() const {
    switch (category) {
        case ItemCategory::Illegal: return "Ilegal";
        case ItemCategory::Luxury: return "Lujo";
        case ItemCategory::Resource: return "Recurso";
        case ItemCategory::Technology: return "Tecnologia";
        case ItemCategory::Food: return "Comida";
        case ItemCategory::Medical: return "Medicina";
        case ItemCategory::Quest: return "Mision";
        default: return "Desconocida";
    }
}

std::string Item::getRarityString() const {
    switch (rarity) {
        case Rarity::Common: return "Comun";
        case Rarity::Rare: return "Raro";
        case Rarity::Exotic: return "Exotico";
        case Rarity::Legendary: return "Legendario";
        case Rarity::Quest: return "Mision";
        default: return "Desconocida";
    }
}

int Item::getQuality() const {
    // Asignamos un valor numérico a la calidad para facilitar comparaciones
    switch (rarity) {
        case Rarity::Common: return 1;
        case Rarity::Rare: return 2;
        case Rarity::Exotic: return 3;
        case Rarity::Legendary: return 4;
        case Rarity::Quest: return 5; // Los items de misión podrían considerarse de máxima calidad
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
    return 0.0f; // Los QuestItems no tienen precio, su valor es cumplir la misión
}

std::string QuestItem::getQuestID() const {
    return questID;
}