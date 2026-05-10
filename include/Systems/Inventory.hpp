#pragma once
#include "Entities/Item.hpp"

class Inventory {
private:
    int capacity = 20; // Initial cargo hold capacity (20 slots)
    // The inventory is managed as a map of item name to its slot (item + quantity)
    std::vector<std::optional<ItemStack>> slots;

public:
    Inventory() noexcept : capacity(20) {
        slots.resize(capacity, std::nullopt); // Initialize with empty slots
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
    void sortByQuantity(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
};