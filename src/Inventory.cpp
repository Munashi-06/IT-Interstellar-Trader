#include "Inventory.hpp"

bool Inventory::addItem(std::unique_ptr<Item> newItem, int qty) {
    if (slots.count(newItem->getName())) {
        slots[newItem->getName()].quantity += qty;
        return true;
    }
    else {
        if (slots.size() < capacity) {
            slots[newItem->getName()] = { std::move(newItem), qty };
            return true;
        }
        else {
            std::cout << "¡Inventario lleno!" << std::endl;
            return false;
        }
    }
}

// Elimina cantidad. Si llega a 0, borra la entrada de la tabla.
void Inventory::removeItem(const std::string& name, int qty) {
    if (slots.count(name)) {
        slots[name].quantity -= qty;
        if (slots[name].quantity <= 0) {
            slots.erase(name);
        }
    }
}

void Inventory::display() const {
    std::cout << "\n--- INVENTARIO DE LA NAVE ---" << std::endl;
    for (auto const& [name, slot] : slots) {
        std::cout << "- " << name << " [x" << slot.quantity << "] | Precio: " 
                    << slot.item->getPrice() << " cdt." << std::endl;
    }
}