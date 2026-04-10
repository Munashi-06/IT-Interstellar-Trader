#pragma once
#include "Planet.hpp"
#include "PlanetManager.hpp"
#include "WorldStateManager.hpp"
#include "ItemFactory.hpp"
#include "Heap.hpp"

class World {
private:
    std::vector<Planet> solarSystem;
    WorldStateManager stateManager;
    int currentTurn;
    float deltaTime; // Tiempo entre turnos, podría influir en la duración de eventos y otras mecánicas temporales
    std::unordered_map<std::string, std::unique_ptr<Item>> globalCatalog; // Catálogo global de items para referencia en eventos y mercados
    std::unique_ptr<Heap> radar; // Heap para gestionar planetas por prioridad de eventos
public:
    World(float deltaT, std::unique_ptr<Heap> r) : currentTurn(0), deltaTime(deltaT), radar(std::move(r)) {
        // Usamos tu Factory para inicializar el universo
        solarSystem = PlanetManager::loadUniqueOrbitPlanets("assets/data/planets_db.txt");
        // Cargar el catálogo global de items
        globalCatalog = ItemFactory::loadDatabase("assets/data/items.txt");
        // Inicializar el heap de radar
        for (const auto& plnt : solarSystem) {
            radar->insert(Planet(plnt), radar->getHeapArray(), cmp); 
        }
    }
    // Se llama cada vez que el jugador viaja o descansa
    void update();

    void setDeltaTime(float dt) { deltaTime = dt; }
    float getDeltaTime() const { return deltaTime; }
    const std::vector<Planet>& getPlanets() const { return solarSystem; }
    const std::unordered_map<std::string, std::unique_ptr<Item>>& getGlobalCatalog() const { return globalCatalog; }
    void updateRadar(Planet& plnt) noexcept; // Actualiza el heap de radar
};