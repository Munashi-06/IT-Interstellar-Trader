#include "Core/World.hpp"

bool World::update() {
    // This is called in each frame of the main loop to handle time counters (such as event duration).
    // Here you could update the world state, such as advancing active events, managing missions, etc.
    // We only update the radar if something actually changed in the events.
    // This avoids recalculating the heap every frame, which improves performance.
    bool hasChanged = stateManager.update(deltaTime, solarSystem);
    
    if (hasChanged) {
        forceRadarUpdate();
    }
    return hasChanged; // Returns whether there were changes to the main method so that the interface can be updated if necessary
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

    // 2. We order the entire vector using your cmp function
    // This guarantees perfect linear ordering for the UI
    std::sort(display.begin(), display.end(), cmp);

    // 3. We pass this sorted vector to the Radar
    // Note: The radar will now receive a sorted vector from 0 to N,
    // so adjust the RadarUI loop to start at i=0
    radar->setHeapArray(display);
}