#include "Core/WorldStatemanager.hpp"
#include <string>

bool WorldStateManager::update(float deltaTime, std::vector<Planet>& planets, Player& player) {
    eventTimer += deltaTime;
    bool eventTriggered = false;

    // 1. Manejar la expiración de eventos existentes
    // Supongamos que cada "segundo" de juego baja la duración
    static float oneSecondTimer = 0;
    oneSecondTimer += deltaTime;

    if (eventTimer >= timeBetweenEvents) {
        eventTriggered = triggerRandomEvent(planets, player);
        eventTimer = 0;
    }

    if (oneSecondTimer >= 1.0f) {
        for (auto& p : planets) {
            if (p.getEvent() != PlanetEvent::None) {
                p.decreaseEventDuration();
                if (p.getEvent() == PlanetEvent::None) {
                    eventTriggered = true; // Un evento acaba de terminar
                    std::cout << "[EVENTO TERMINADO] En el planeta " << p.getName() << " ha terminado el evento: " 
                              << p.getEventName() << std::endl;
                }
            }
        }
        oneSecondTimer = 0;
    }
    return eventTriggered; // Devuelve si se generó un evento para que el mundo sepa si debe actualizar el radar
}

bool WorldStateManager::triggerRandomEvent(std::vector<Planet>& planets, Player& player) {
    if (planets.empty()) {
        std::cout << "[ERROR] El sistema solar no tiene planetas cargados." << std::endl;
        return false;
    }

    // Contar eventos activos
    int activeEvents = 0;
    for (const auto& p : planets) {
        if (p.getEvent() != PlanetEvent::None) activeEvents++;
    }
    if (activeEvents >= maxActiveEvents) {
        return false; // No se genera un nuevo evento si ya hay demasiados activos
    }

// 1. Seleccionar un planeta al azar
    Planet& target = planets[rand() % planets.size()];
    // Si ya tiene un evento, no le damos otro para no solapar
    if (target.getEvent() != PlanetEvent::None) return false;

    float baseProbability = 15.f; // Base probability for an event to occur on a planet without considering attributes

    if (player.getEventFrequencyBonus()) {
        baseProbability = 32.5f; // Si el jugador tiene el bonus de frecuencia, casi duplica la probabilidad base
    }

    float roll = rand() % 100;

// 2. Lógica de Pesos por Atributos
    // Piratería: Más probable si securityLevel es bajo (< 4)
    if (target.getSecurityLevel() <= 4 && roll < baseProbability + 25.f) {
        target.setEvent(PlanetEvent::Piracy);
        target.setEventDuration(40 + rand() % 40 + (player.getEventFrequencyBonus() ? 30 : 0)); // duracion minima de 40segundos, max 80segundos
        return true;
    }
    // Hambruna: Más probable si resourceAbundance es bajo (< 3)
    else if (target.getResourceAbundance() <= 3 && roll < baseProbability + 15.f) {
        target.setEvent(PlanetEvent::Famine);
        target.setEventDuration(60 + rand() % 60 + (player.getEventFrequencyBonus() ? 30 : 0)); // duracion minima de 60 segundos, max 120 segundos
        return true;
    }
    // Plaga: Más probable si medicalTech es bajo (< 4)
    else if (target.getMedicalTech() <= 4 && roll < baseProbability + 10.f) {
        target.setEvent(PlanetEvent::Plague);
        target.setEventDuration(50 + rand() % 60 + (player.getEventFrequencyBonus() ? 30 : 0)); // duracion minima de 50 segundos, max 110 segundos
        return true;
    }
    // Auge Tecnológico: Más probable si techLevel es alto (> 7)
    else if (target.getTechLevel() >= 7 && roll < baseProbability) {
        target.setEvent(PlanetEvent::TechBoom);
        target.setEventDuration(30 + rand() % 60 + (player.getEventFrequencyBonus() ? 30 : 0)); // duracion minima de 30 segundos, max 90 segundos
        return true;
    }

    return false;
}