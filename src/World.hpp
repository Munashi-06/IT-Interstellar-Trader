#pragma once
#include "Planet.hpp"
#include "PlanetManager.hpp"
#include "WorldStateManager.hpp"
#include "ItemFactory.hpp"

class World {
private:
    std::vector<Planet> solarSystem;
    WorldStateManager stateManager;
    int currentTurn;
    float deltaTime; // Tiempo entre turnos, podría influir en la duración de eventos y otras mecánicas temporales
    std::unordered_map<std::string, std::unique_ptr<Item>> globalCatalog; // Catálogo global de items para referencia en eventos y mercados

public:
    World(float deltaT) : currentTurn(0), deltaTime(deltaT) {
        // Usamos tu Factory para inicializar el universo
        solarSystem = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets_db.txt");
        // Cargar el catálogo global de items
        globalCatalog = ItemFactory::loadDatabase("assets/data/items.txt");
    }
    // Se llama cada vez que el jugador viaja o descansa
    void update();

    void setDeltaTime(float dt) { deltaTime = dt; }
    float getDeltaTime() const { return deltaTime; }
    const std::vector<Planet>& getPlanets() const { return solarSystem; }
    const std::unordered_map<std::string, std::unique_ptr<Item>>& getGlobalCatalog() const { return globalCatalog; }
};