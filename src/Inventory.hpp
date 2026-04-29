#pragma once
#include "Item.hpp"

struct InventorySlot {
    std::unique_ptr<Item> item;
    int quantity;
};

class Inventory {
private:
    // El inventario se maneja como un mapa de nombre de item a su slot (item + cantidad)
    std::unordered_map<std::string, InventorySlot> slots;
    int capacity = 20;

public:
    bool addItem(std::unique_ptr<Item> item, int quantity);
    void removeItem(const std::string& itemName, int quantity);
    void display() const;
    void setCapacity(int a) noexcept;
};