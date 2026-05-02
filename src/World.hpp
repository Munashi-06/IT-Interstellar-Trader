#pragma once
#include "Planet.hpp"
#include "PlanetManager.hpp"
#include "WorldStateManager.hpp"
#include "ItemFactory.hpp"
#include "Heap.hpp"
#include <SFML/Graphics.hpp>

class World {
private:
    sf::Clock worldClock;
    std::vector<Planet> solarSystem;
    WorldStateManager stateManager;
    int currentTurn;
    float deltaTime; // Tiempo entre turnos, podría influir en la duración de eventos y otras mecánicas temporales
    std::unordered_map<std::string, std::unique_ptr<Item>> globalCatalog; // Catálogo global de items para referencia en eventos y mercados
    std::unique_ptr<Heap> radar; // Heap para gestionar planetas por prioridad de eventos
public:
    World(float deltaT, std::unique_ptr<Heap> r, std::vector<Planet> planetas) 
        : currentTurn(0), 
          deltaTime(deltaT), 
          radar(std::move(r)), 
          solarSystem(std::move(planetas)) 
    {
        globalCatalog = ItemFactory::loadDatabase("assets/data/items.txt");
    }
    
    bool update();

    std::unique_ptr<Heap>& getRadar() { return radar; }
    void setDeltaTime(float dt) { deltaTime = dt; }
    float getDeltaTime() const { return deltaTime; }
    std::vector<Planet>& getPlanets() { return solarSystem; }
    const std::vector<Planet>& getPlanets() const { return solarSystem; }
    const std::unordered_map<std::string, std::unique_ptr<Item>>& getGlobalCatalog() const { return globalCatalog; }
    void updateRadar(Planet& plnt) noexcept; // Actualiza el heap de radar
    float getTime() const {return worldClock.getElapsedTime().asSeconds();}
    void forceRadarUpdate();
};