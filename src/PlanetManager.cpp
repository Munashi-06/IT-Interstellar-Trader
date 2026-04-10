#include "PlanetManager.hpp"

static std::vector<Planet> loadUniqueOrbitPlanets(const std::string& filename) {
    std::map<int, std::vector<Planet>> orbitPool;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) return {};

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Ignora líneas vacías

        std::stringstream ss(line);
        std::string name, desc, temp;
        int orb, tech, sec, res, lux, moon, med;

        // Leemos strings con getline
        std::getline(ss, name, ',');
        std::getline(ss, desc, ',');

        // Para los números, es más seguro usar el operador >> y saltar la coma
        auto readNextInt = [&](int &val) {
            std::getline(ss, temp, ',');
            try {
                val = std::stoi(temp);
            } catch (...) {
                val = 0; // Valor por defecto si falla el parsing
            }
        };

        readNextInt(orb);
        readNextInt(tech);
        readNextInt(sec);
        readNextInt(res);
        readNextInt(lux);
        readNextInt(moon);
        readNextInt(med);

        orbitPool[orb].emplace_back(name, desc, orb, tech, sec, res, lux, moon, med);
    }

    // --- lógica de selección ---
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> distCount(5, 8);
    size_t targetCount = distCount(g);

    std::vector<Planet> selectedPlanets;
    std::vector<int> availableOrbits;
    for(auto const& [orbit, list] : orbitPool) {
        availableOrbits.push_back(orbit);
    }

    targetCount = std::min(targetCount, availableOrbits.size());
    std::shuffle(availableOrbits.begin(), availableOrbits.end(), g);

    for (size_t i = 0; i < targetCount; ++i) {
        int targetOrbit = availableOrbits[i];
        auto& candidates = orbitPool[targetOrbit];
        std::uniform_int_distribution<int> pDist(0, (int)candidates.size() - 1);
        selectedPlanets.push_back(candidates[pDist(g)]);
    }

    insertion_sort(selectedPlanets);
    return selectedPlanets;
}