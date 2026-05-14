#include "Systems/Inventory.hpp"
#include "Systems/PlanetManager.hpp"

// We use the catalog to validate the item, but we only store the ID
bool Inventory::addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice) {
    // Try to stack in existing slots
    for (auto& slot : slots) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity += qty;
            return true;
        }
    }

    // If stacking was not possible, look for an empty slot (nullopt)
    for (auto& slot : slots) {
        if (slot == std::nullopt) {
            slot = ItemStack{ itemID, qty, maxStackSize, buyPrice };
            return true;
        }
    }

    std::cout << "No space in the cargo hold!" << std::endl;
    return false;
}

void Inventory::clearAll() noexcept {
    for (auto& slot : slots) {
        slot = std::nullopt;
    }
}

void Inventory::removeItem(const std::string& itemID, int qty) {
    for (auto& slot : slots) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity -= qty;
            if (slot->quantity <= 0) {
                slot = std::nullopt; // Frees the slot
            }
            return;
        }
    }
}

void Inventory::display(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) const {
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i].has_value()) {
            // We look for the actual item info in the global catalog
            const auto& itemInfo = catalog.at(slots[i]->itemID);
            std::cout << "Slot " << i << ": " << itemInfo->getName() 
                      << " x" << slots[i]->quantity << std::endl;
        } else {
            std::cout << "Slot " << i << ": [ EMPTY ]" << std::endl;
        }
    }
}

void Inventory::upgradeStorage(int extraSlots) noexcept {
    slots.resize(slots.size() + extraSlots, std::nullopt);
}

int Inventory::getUsedSlots() const noexcept {
    int used = 0;
    for (const auto& slot : slots) {
        if (slot.has_value()) used++;
    }
    return used;
}

void Inventory::resetCapacity() {
    clearAll();
    capacity = 20;
}

void Inventory::sortByName(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByName(slots, ascending, catalog);
}

void Inventory::sortByCategory(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByCategory(slots, ascending, catalog);
}

void Inventory::sortByQuality(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByQuality(slots, ascending, catalog);
}

void Inventory::sortByPrice(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByPrice(slots, ascending, catalog);
}

void Inventory::sortByQuantity(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByQuantity(slots, ascending, catalog);
}