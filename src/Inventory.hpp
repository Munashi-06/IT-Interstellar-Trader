#pragma once
#include "Item.hpp"

class Inventory {
private:
    int capacity = 20; // Capacidad inicial de la bodega (20 slots)
    // El inventario se maneja como un mapa de nombre de item a su slot (item + cantidad)
    std::vector<std::optional<ItemStack>> slots;

public:
    Inventory() noexcept : capacity(20) {
        slots.resize(capacity, std::nullopt); // Inicializamos con slots vacíos
    }
    bool addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice);
    void removeItem(const std::string& itemID, int qty);
    void display(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) const;
    void upgradeStorage(int extraSlots) noexcept;
    const std::vector<std::optional<ItemStack>>& getSlots() const noexcept { return slots; }
    int getCapacity() const noexcept { return capacity; }
    int getUsedSlots() const noexcept;
    void sortByName(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByCategory(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByQuality(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByPrice(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
};

/*
---- EJEMPLO DE USO ----

// 1. Inicializar inventario con 10 slots vacíos
// En el constructor de Inventory: slots.resize(10, std::nullopt);

// 2. Proceso de compra
std::string id = "plutonio_01";
if (nave.getInventory().addItem(id, 5, 20, 100.0f)) {
    std::cout << "Compra exitosa." << std::endl;
}

// 3. Mostrar inventario (necesita el catálogo para los nombres)
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
    
*/