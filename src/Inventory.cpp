#include "Inventory.hpp"
#include "PlanetManager.hpp"

// Usamos el catálogo para validar el item, pero solo guardamos el ID
bool Inventory::addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice) {
    // Intentar apilar en slots existentes
    for (auto& slot : slots) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity += qty;
            return true;
        }
    }

    // Si no se pudo apilar, buscar un slot vacío (nullopt)
    for (auto& slot : slots) {
        if (slot == std::nullopt) {
            slot = ItemStack{ itemID, qty, maxStackSize, buyPrice };
            return true;
        }
    }

    std::cout << "¡No hay espacio en la bodega!" << std::endl;
    return false;
}

void Inventory::removeItem(const std::string& itemID, int qty) {
    for (auto& slot : slots) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity -= qty;
            if (slot->quantity <= 0) {
                slot = std::nullopt; // Libera el slot
            }
            return;
        }
    }
}

void Inventory::display(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) const {
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i].has_value()) {
            // Buscamos la info real del item en el catálogo global
            const auto& itemInfo = catalog.at(slots[i]->itemID);
            std::cout << "Slot " << i << ": " << itemInfo->getName() 
                      << " x" << slots[i]->quantity << std::endl;
        } else {
            std::cout << "Slot " << i << ": [ VACÍO ]" << std::endl;
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