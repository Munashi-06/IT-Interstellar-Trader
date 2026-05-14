#include "Systems/TradeManager.hpp"

bool TradeManager::buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    const auto& itemData = catalog.at(itemID);
    
    // 1. Obtenemos el precio del planeta y le aplicamos los descuentos del jugador
    float basePlanetPrice = planet.getItemPrice(itemID, catalog);
    float finalPrice = getFinalBuyPrice(*itemData, basePlanetPrice, player);

    // 2. Verificar si el jugador tiene suficiente dinero usando el precio FINAL
    if (player.getMoney() < finalPrice) {
        std::cout << "[COMERCIO] Fondos insuficientes para comprar " << itemData->getName() << ".\n";
        return false;
    }

    // 3. Intentar añadir al inventario del jugador
    if (!playerInv.addItem(itemID, 1, itemData->getMaxStackSize(), finalPrice, planet.getName())) {
        std::cout << "[TRADE] No space in the ship's cargo hold.\n";
        return false;
    }

    // 4. Efectuamos la transacción con el precio descontado:
    player.setMoney(player.getMoney() - finalPrice); // Restar dinero con descuento
    planet.removeItem(itemID, 1);               // Quitar 1 unidad del mercado

    std::cout << "[TRADE] You bought 1x " << itemData->getName() << " for Bs. " << finalPrice << "\n";
    return true;
}

bool TradeManager::sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    const auto& itemData = catalog.at(itemID);
    
    // 1. We get de base price in the planet and apply the player's sell bonus
    float basePlanetPrice = planet.getItemPrice(itemID, catalog); 
    float originalBuyPrice = basePlanetPrice;
    std::string originPlanet = planet.getName(); // Por defecto

    for (const auto& slot : playerInv.getSlots()) {
        if (slot.has_value() && slot->itemID == itemID) {
            originalBuyPrice = slot->buyPrice;
            originPlanet = slot->originPlanet;
            break;
        }
    }
    float finalPrice = getFinalSellPrice(*itemData, basePlanetPrice, player, originalBuyPrice, originPlanet, planet.getName());

    // 2. Remove 1 unit from the player's inventory
    playerInv.removeItem(itemID, 1);

    // 3. Add money to the player
    player.setMoney(player.getMoney() + finalPrice);

    // 4. Add the item to the planet's market so others can buy it (or the player can rebuy it)
    planet.addItem(itemID, 1, itemData->getMaxStackSize(), itemData->getBasePrice());

    std::cout << "[TRADE] You sold 1x " << itemData->getName() << " for Bs. " << finalPrice << "\n";
    return true;
}

float TradeManager::getFinalBuyPrice(const Item& item, float basePlanetPrice, const Player& player) {
    // 1. Iniciamos con el descuento base (ej. 0.05 del Universal Translator)
    // Nota: en tu código base decía player.getDiscount(), si lo cambiaste a getFlatDiscount(), usa ese.
    float totalDiscount = player.getDiscount(); 
    
    // 2. Sumamos descuentos según las habilidades
    if (player.getCorporateLicense() && (item.isTechnology() || item.isMedical())) {
        totalDiscount += 0.15f; // Sumamos 15%
    }
    
    if (player.getBlackMarketContacts() && item.isIllegal()) {
        totalDiscount += 0.25f; // Sumamos 25%
    }
    
    // Opcional: Poner un límite para que el descuento nunca sea del 100% y te salgan gratis
    if (totalDiscount > 0.90f) {
        totalDiscount = 0.90f; // Máximo 90% de descuento
    }
    
    // 3. Aplicamos el descuento total sumado al precio base
    return basePlanetPrice * (1.0f - totalDiscount);
}

float TradeManager::getFinalSellPrice(const Item& item, float basePlanetPrice, const Player& player, float originalBuyPrice, const std::string& originPlanet, const std::string& currentPlanet) {
    float totalBonus = 0.0f; // Iniciamos en 0% de bono extra
    
    // 1. Sumamos los bonos de venta según las habilidades
    if (player.getMonopolyCharter() && !item.isIllegal()) {
        totalBonus += 0.20f; // Sumamos 20% de ganancia
    }
    
    if (player.getSyndicateBoss() && item.isIllegal()) {
        totalBonus += 0.50f; // Sumamos 50% de ganancia (¡Aquí está el Syndicate Boss!)
    }

    float bonusPrice = basePlanetPrice * (1.0f + totalBonus);
    //LIMIT: The selling price must never be higher than the buying price on the same planet. This prevents the exploit of buying and selling in the same location.
    if (originPlanet == currentPlanet) {
        bonusPrice = std::min(bonusPrice, originalBuyPrice);
    }
    return bonusPrice;
}