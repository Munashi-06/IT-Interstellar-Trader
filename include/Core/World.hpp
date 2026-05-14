#pragma once
#include "Core/WorldStatemanager.hpp"
#include "Entities/Planet.hpp"
#include "Systems/PlanetManager.hpp"
#include "Systems/ItemFactory.hpp"
#include "DataStructures/Heap.hpp"
#include "DataStructures/StockSorter.hpp"
#include <SFML/Graphics.hpp>

class World {
private:
    sf::Clock worldClock;
    std::vector<Planet> solarSystem;
    WorldStateManager stateManager;
    int currentTurn;
    float deltaTime; // Time between turns could influence the duration of events and other time-based mechanics
    std::unordered_map<std::string, std::unique_ptr<Item>> globalCatalog; // Global catalog of items for reference in events and markets
    std::unique_ptr<Heap> radar; // Heap for managing planets by event priority
public:
    World(float deltaT, std::unique_ptr<Heap> r, std::vector<Planet> planetas) 
        : currentTurn(0), 
          deltaTime(deltaT), 
          radar(std::move(r)), 
          solarSystem(std::move(planetas)) 
    {
        globalCatalog = ItemFactory::loadDatabase("assets/data/items.txt");
    }
    
    bool update(Player& player); // Devuelve si hubo cambios en el estado del mundo que requieran actualizar la interfaz

    std::unique_ptr<Heap>& getRadar() { return radar; }
    void setDeltaTime(float dt) { deltaTime = dt; }
    float getDeltaTime() const { return deltaTime; }
    std::vector<Planet>& getPlanets() { return solarSystem; }
    const std::vector<Planet>& getPlanets() const { return solarSystem; }
    const std::unordered_map<std::string, std::unique_ptr<Item>>& getGlobalCatalog() const { return globalCatalog; }
    void updateRadar(Planet& plnt) noexcept;// Update the radar heap
    float getTime() const {return worldClock.getElapsedTime().asSeconds();}
    void forceRadarUpdate();
    std::unordered_map<std::string, std::unique_ptr<Item>>& getCatalog() { return globalCatalog; }
};
