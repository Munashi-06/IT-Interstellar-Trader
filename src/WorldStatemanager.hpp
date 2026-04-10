#pragma once
#include "Planet.hpp"

inline int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

inline int getRandomEventDuration(PlanetEvent event) {
    switch (event) {
        case PlanetEvent::War: return randomInt(100, 250);
        case PlanetEvent::Plague: return randomInt(50, 300);
        case PlanetEvent::TechBoom: return randomInt(60, 160);
        case PlanetEvent::Famine: return randomInt(90, 200);
        case PlanetEvent::Piracy: return randomInt(80, 180);
        default: return 0;
    }
}

class WorldStateManager {
private:
    float eventTimer;
    float timeBetweenEvents;
    int maxActiveEvents;

public:
    void update(float deltaTime, std::vector<Planet>& planets);

    void triggerRandomEvent(std::vector<Planet>& planets);
};