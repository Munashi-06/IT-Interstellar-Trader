#include "WorldStatemanager.hpp"
#include <string>

// Helper function to convert PlanetEvent enum to string
std::string planetEventToString(PlanetEvent event) {
    switch (event) {
        case PlanetEvent::None: return "Ninguno";
        case PlanetEvent::Piracy: return "Piratería";
        case PlanetEvent::Famine: return "Hambruna";
        case PlanetEvent::Plague: return "Plaga";
        case PlanetEvent::TechBoom: return "Auge Tecnológico";
        default: return "Desconocido";
    }
}

bool WorldStateManager::update(float deltaTime, std::vector<Planet>& planets) {
    eventTimer += deltaTime;
    bool eventTriggered = false;
    if (eventTimer >= timeBetweenEvents) {
        eventTriggered = triggerRandomEvent(planets);
        eventTimer = 0;
    }
    return eventTriggered; // Devuelve si se generó un evento para que el mundo sepa si debe actualizar el radar
}

bool WorldStateManager::triggerRandomEvent(std::vector<Planet>& planets) {
    if (planets.empty()) {
        std::cout << "[ERROR] El sistema solar no tiene planetas cargados." << std::endl;
        return false;
    }

    // 1. Seleccionar un planeta al azar
    Planet& target = planets[rand() % planets.size()];
    // Si ya tiene un evento, no le damos otro para no solapar
    if (target.getEvent() != PlanetEvent::None) return false;

    int roll = rand() % 100;

    // 2. Lógica de Pesos por Atributos
    // Piratería: Más probable si securityLevel es bajo (< 4)
    if (target.getSecurityLevel() < 4 && roll < 40) {
        target.setEvent(PlanetEvent::Piracy);
        target.setEventDuration(5 + rand() % 5); 
        std::cout << "[EVENTO] En el planeta " << target.getName() << " ha ocurrido un evento: " 
                 << planetEventToString(target.getEvent()) << " con duracion de " << target.getEventDuration() << " seg." << std::endl;
        return true;
    }
    // Hambruna: Más probable si resourceAbundance es bajo (< 3)
    else if (target.getResourceAbundance() < 3 && roll < 30) {
        target.setEvent(PlanetEvent::Famine);
        target.setEventDuration(7 + rand() % 4);
        std::cout << "[EVENTO] En el planeta " << target.getName() << " ha ocurrido un evento: " 
              << planetEventToString(target.getEvent()) << " con duración de " << target.getEventDuration() << " seg." << std::endl;
        return true;
    }
    // Plaga: Más probable si medicalTech es bajo (< 4)
    else if (target.getMedicalTech() < 4 && roll < 25) {
        target.setEvent(PlanetEvent::Plague);
        target.setEventDuration(4 + rand() % 6);
        std::cout << "[EVENTO] En el planeta " << target.getName() << " ha ocurrido un evento: " 
              << planetEventToString(target.getEvent()) << " con duración de " << target.getEventDuration() << " seg." << std::endl;
        return true;
    }
    // Auge Tecnológico: Más probable si techLevel es alto (> 7)
    else if (target.getTechLevel() > 7 && roll < 15) {
        target.setEvent(PlanetEvent::TechBoom);
        target.setEventDuration(3 + rand() % 3);
        std::cout << "[EVENTO] En el planeta " << target.getName() << " ha ocurrido un evento: " 
              << planetEventToString(target.getEvent()) << " con duración de " << target.getEventDuration() << " seg." << std::endl;
    
        return true;
    }

    return false;
}