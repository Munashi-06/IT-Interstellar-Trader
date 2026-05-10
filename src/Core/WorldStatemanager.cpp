#include "Core/WorldStatemanager.hpp"
#include <string>

bool WorldStateManager::update(float deltaTime, std::vector<Planet>& planets) {
    eventTimer += deltaTime;
    bool eventTriggered = false;

    // 1. Handle the expiration of existing events
    // Let's assume that each "second" of gameplay decreases the duration
    static float oneSecondTimer = 0;
    oneSecondTimer += deltaTime;

    if (eventTimer >= timeBetweenEvents) {
        eventTriggered = triggerRandomEvent(planets);
        eventTimer = 0;
    }

    if (oneSecondTimer >= 1.0f) {
        for (auto& p : planets) {
            if (p.getEvent() != PlanetEvent::None) {
                p.decreaseEventDuration();
                if (p.getEvent() == PlanetEvent::None) {
                    eventTriggered = true; // An event has just ended
                    std::cout << "[EVENTO TERMINADO] En el planeta " << p.getName() << " ha terminado el evento: " 
                              << p.getEventName() << std::endl;
                }
            }
        }
        oneSecondTimer = 0;
    }
    return eventTriggered; // Returns whether an event was generated so the world knows whether to update the radar.
}

bool WorldStateManager::triggerRandomEvent(std::vector<Planet>& planets) {
    if (planets.empty()) {
        std::cout << "[ERROR] El sistema solar no tiene planetas cargados." << std::endl;
        return false;
    }
    // Count active events
    int activeEvents = 0;
    for (const auto& p : planets) {
        if (p.getEvent() != PlanetEvent::None) activeEvents++;
    }
    if (activeEvents >= maxActiveEvents) {
        return false; // A new event is not generated if there are already too many active events
    }
    // 1. Select a planet at random
    Planet& target = planets[rand() % planets.size()];
    // If you already have an event, we won't give you another one to avoid overlap.
    if (target.getEvent() != PlanetEvent::None) return false;

    int roll = rand() % 100;

    // 2. Attribute Weighting Logic
    // Piracy: More likely if securityLevel is low (< 4)
    if (target.getSecurityLevel() < 4 && roll < 40) {
        target.setEvent(PlanetEvent::Piracy);
        target.setEventDuration(40 + rand() % 40); // Minimum duration of 40 seconds, maximum 80 seconds
        return true;
    }
    // Famine: More likely if resource abundance is low (< 3)
    else if (target.getResourceAbundance() < 3 && roll < 30) {
        target.setEvent(PlanetEvent::Famine);
        target.setEventDuration(60 + rand() % 60); // Minimum duration of 60 seconds, maximum 120 seconds
        return true;
    }
    // Plague: More likely if medicalTech is low (< 4)
    else if (target.getMedicalTech() < 4 && roll < 25) {
        target.setEvent(PlanetEvent::Plague);
        target.setEventDuration(50 + rand() % 60); // Minimum duration of 50 seconds, maximum 110 seconds
        return true;
    }
    // Technological Boom: More likely if techLevel is high (> 7)
    else if (target.getTechLevel() > 7 && roll < 15) {
        target.setEvent(PlanetEvent::TechBoom);
        target.setEventDuration(30 + rand() % 60); // Minimum duration of 30 seconds, maximum 90 seconds
    
        return true;
    }

    return false;
}