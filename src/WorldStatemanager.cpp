#include "WorldStatemanager.hpp"

void WorldStateManager::update(float deltaTime, std::vector<Planet>& planets) {
    eventTimer += deltaTime;
    if (eventTimer >= timeBetweenEvents) {
        triggerRandomEvent(planets);
        eventTimer = 0;
    }
}

void WorldStateManager::triggerRandomEvent(std::vector<Planet>& planets) {
    // 1. Contar cuántos eventos activos hay
    int activeEvents = 0;
    for (const auto& planet : planets) {
        if (planet.getEvent() != PlanetEvent::None) activeEvents++;
    }

    // 2. Si es menor a maxActiveEvents, elegir un planeta al azar
    if (activeEvents < maxActiveEvents) {
        int randomIndex = rand() % planets.size();
        Planet& chosenPlanet = planets[randomIndex];

        // 3. Cambiar su PlanetEvent basándose en sus atributos
        // (Aquí podrías implementar una lógica más compleja que considere los atributos del planeta)
        int eventChance = rand() % 100;
        if (eventChance < 20) {
            chosenPlanet.setEvent(PlanetEvent::War);
        } else if (eventChance < 40) {
            chosenPlanet.setEvent(PlanetEvent::Plague);
        } else if (eventChance < 60) {
            chosenPlanet.setEvent(PlanetEvent::TechBoom);
        } else if (eventChance < 80) {
            chosenPlanet.setEvent(PlanetEvent::Famine);
        } else {
            chosenPlanet.setEvent(PlanetEvent::None); // Evento aleatorio que no afecta al planeta
        }
    }
}

void WorldStateManager::triggerRandomEvent(std::vector<Planet>& planets) {
    Planet& target = planets[rand() % planets.size()];
    
    int roll = rand() % 100;

    // Si la seguridad es baja, hay mucha chance de Piratería
    if (target.getSecurityLevel() < 4 && roll < 60) {
        target.setEvent(PlanetEvent::Piracy);
        target.setEventDuration(getRandomEventDuration(PlanetEvent::Piracy));
    } 
    // Si la abundancia es baja, hay chance de Hambruna
    else if (target.getResourceAbundance() < 3 && roll < 50) {
        target.setEvent(PlanetEvent::Famine);
        target.setEventDuration(getRandomEventDuration(PlanetEvent::Famine));
    }
    // Si la tecnología médica es baja, hay chance de Plaga
    else if (target.getMedicalTech() < 4 && roll < 40) {
        target.setEvent(PlanetEvent::Plague);
        target.setEventDuration(getRandomEventDuration(PlanetEvent::Plague));
    }
    // Si el nivel tecnológico es alto, hay chance de Boom Tecnológico
    else if (target.getTechLevel() > 7 && roll < 30) {
        target.setEvent(PlanetEvent::TechBoom);
        target.setEventDuration(getRandomEventDuration(PlanetEvent::TechBoom));
    }
    // Si el nivel de seguridad es alto, hay chance de Guerra (conflictos entre facciones por recursos)
    else if (target.getSecurityLevel() > 7 && roll < 20) {
        target.setEvent(PlanetEvent::War);
        target.setEventDuration(getRandomEventDuration(PlanetEvent::War));
    }
    else {
        target.setEvent(PlanetEvent::None); // Evento aleatorio que no afecta al planeta
        target.setEventDuration(getRandomEventDuration(PlanetEvent::None));
    }
}