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
    float eventTimer = 0.0f; 
    float timeBetweenEvents = 10.0f; // Tiempo mínimo para que intente generar un nuevo evento
    int maxActiveEvents = 3; // Máximo número de eventos activos al mismo tiempo

public:
    bool update(float deltaTime, std::vector<Planet>& planets);

    bool triggerRandomEvent(std::vector<Planet>& planets);
};