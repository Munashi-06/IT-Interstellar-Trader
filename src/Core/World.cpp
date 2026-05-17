#include "Core/World.hpp"

bool World::update(Player& player, bool& outRestocked) {
    bool hasChanged = stateManager.update(deltaTime, solarSystem, player);
    outRestocked = false; // 1. Default to false every frame
    
    // RESTOCK TIMER: Restock markets every 300 seconds (5 minutes)
    static float restockTimer = 0.0f;
    restockTimer += deltaTime;
    
    if (restockTimer >= 300.0f) {
        for (auto& planet : solarSystem) {
            planet.restockMarket(globalCatalog, player.getShipLevel());
        }
        restockTimer = 0.0f;
        hasChanged = true;
        outRestocked = true; // 2. Signal the Engine that a restock just happened!
    }
    
    if (hasChanged) {
        forceRadarUpdate();
    }
    return hasChanged;
}

void World::updateRadar(Planet& plnt) noexcept {
    // Updates the radar heap after modifying a planet
    radar->remove(radar->getHeapArray(), cmp);
    radar->insert(std::move(plnt), radar->getHeapArray(), cmp);
}

void World::forceRadarUpdate() {
    std::vector<Planet> display;
    
    // 1. We obtain the planets of the solar system
    for (const auto& p : solarSystem) {
        display.push_back(p);
    }

    // 2. We order the entire vector
    // This guarantees perfect linear ordering for the UI
    StockSorter::hybridSort(display.begin(), display.end(), cmp);

    // 3. We pass this sorted vector to the Radar
    // Note: The radar will now receive a sorted vector from 0 to N,
    // so adjust the RadarUI loop to start at i=0
    radar->setHeapArray(display);
}