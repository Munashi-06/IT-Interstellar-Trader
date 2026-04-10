#include "WorldStatemanager.hpp"

void WorldStateManager::update(float deltaTime, std::vector<Planet>& planets) {
    eventTimer += deltaTime;
    if (eventTimer >= timeBetweenEvents) {
        triggerRandomEvent(planets);
        eventTimer = 0;
    }
}

void WorldStateManager::triggerRandomEvent(std::vector<Planet>& planets) {
    if (planets.empty()) return;

    // 1. Seleccionar un planeta al azar
    Planet& target = planets[rand() % planets.size()];
    
    // Si ya tiene un evento, no le damos otro para no solapar
    if (target.getEvent() != PlanetEvent::None) return;

    int roll = rand() % 100;

    // 2. Lógica de Pesos por Atributos
    // Piratería: Más probable si securityLevel es bajo (< 4)
    if (target.getSecurityLevel() < 4 && roll < 40) {
        target.setEvent(PlanetEvent::Piracy);
        target.setEventDuration(5 + rand() % 5); 
    }
    // Hambruna: Más probable si resourceAbundance es bajo (< 3)
    else if (target.getResourceAbundance() < 3 && roll < 30) {
        target.setEvent(PlanetEvent::Famine);
        target.setEventDuration(7 + rand() % 4);
    }
    // Plaga: Más probable si medicalTech es bajo (< 4)
    else if (target.getMedicalTech() < 4 && roll < 25) {
        target.setEvent(PlanetEvent::Plague);
        target.setEventDuration(4 + rand() % 6);
    }
    // Auge Tecnológico: Más probable si techLevel es alto (> 7)
    else if (target.getTechLevel() > 7 && roll < 15) {
        target.setEvent(PlanetEvent::TechBoom);
        target.setEventDuration(3 + rand() % 3);
    }
}