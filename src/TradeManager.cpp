#include "TradeManager.hpp"

bool TradeManager::buyItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    const auto& itemData = catalog.at(itemID);
    float price = planet.getItemPrice(itemID, catalog);

    // 1. Verificar si el jugador tiene suficiente dinero
    // (Asumo que tienes métodos getMoney() y setMoney() en tu clase Player)
    if (player.getMoney() < price) {
        std::cout << "[COMERCIO] Fondos insuficientes para comprar " << itemData->getName() << ".\n";
        return false;
    }

    // 2. Intentar añadir al inventario del jugador (esto también verifica si hay espacio)
    if (!playerInv.addItem(itemID, 1, itemData->getMaxStackSize(), itemData->getBasePrice())) {
        std::cout << "[COMERCIO] No hay espacio en la bodega de la nave.\n";
        return false;
    }

    // 3. Si llegamos aquí, hay dinero y espacio. Efectuamos la transacción:
    player.setMoney(player.getMoney() - price); // Restar dinero
    planet.removeItem(itemID, 1);               // Quitar 1 unidad del mercado del planeta

    std::cout << "[COMERCIO] Compraste 1x " << itemData->getName() << " por Bs. " << price << "\n";
    return true;
}

bool TradeManager::sellItem(const std::string& itemID, Player& player, Inventory& playerInv, Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    const auto& itemData = catalog.at(itemID);
    
    // Usamos el mismo precio de mercado del planeta para la venta
    float price = planet.getItemPrice(itemID, catalog); 

    // 1. Quitamos 1 unidad del inventario del jugador
    playerInv.removeItem(itemID, 1);

    // 2. Sumamos el dinero al jugador
    player.setMoney(player.getMoney() + price);

    // 3. Añadimos el ítem al mercado del planeta para que otros puedan comprarlo (o el jugador recomprarlo)
    planet.addItem(itemID, 1, itemData->getMaxStackSize(), itemData->getBasePrice());

    std::cout << "[COMERCIO] Vendiste 1x " << itemData->getName() << " por Bs. " << price << "\n";
    return true;
}