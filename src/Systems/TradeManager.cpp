#include "Systems/TradeManager.hpp"

bool TradeManager::buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    const auto& itemData = catalog.at(itemID);
    float price = planet.getItemPrice(itemID, catalog);

    // 1. Check if the player has enough money
    // (Assuming you have getMoney() and setMoney() methods in your Player class)
    if (player.getMoney() < price) {
        std::cout << "[TRADE] Insufficient funds to buy " << itemData->getName() << ".\n";
        return false;
    }

    // 2. Attempt to add to the player's inventory (this also checks for available space)
    if (!playerInv.addItem(itemID, 1, itemData->getMaxStackSize(), itemData->getBasePrice())) {
        std::cout << "[TRADE] No space in the ship's cargo hold.\n";
        return false;
    }

    // 3. If we reached this point, there is money and space. We execute the transaction:
    player.setMoney(player.getMoney() - price); // Subtract money
    planet.removeItem(itemID, 1);               // Remove 1 unit from the planet's market

    std::cout << "[TRADE] You bought 1x " << itemData->getName() << " for Bs. " << price << "\n";
    return true;
}

bool TradeManager::sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    const auto& itemData = catalog.at(itemID);
    
    // We use the same planet market price for the sale
    float price = planet.getItemPrice(itemID, catalog); 

    // 1. Remove 1 unit from the player's inventory
    playerInv.removeItem(itemID, 1);

    // 2. Add money to the player
    player.setMoney(player.getMoney() + price);

    // 3. Add the item to the planet's market so others can buy it (or the player can rebuy it)
    planet.addItem(itemID, 1, itemData->getMaxStackSize(), itemData->getBasePrice());

    std::cout << "[TRADE] You sold 1x " << itemData->getName() << " for Bs. " << price << "\n";
    return true;
}