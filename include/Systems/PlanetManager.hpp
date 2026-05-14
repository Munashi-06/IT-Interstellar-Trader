#pragma once
#include "Entities/Planet.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <random>
#include "DataStructures/StockSorter.hpp"

class PlanetManager {
public:
    static std::vector<Planet> loadUniqueOrbitPlanets(const std::string& filename);
};