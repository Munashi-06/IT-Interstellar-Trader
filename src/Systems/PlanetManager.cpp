#include "Systems/PlanetManager.hpp"

/*
Why use a std::map<int, std::vector<Planet>>?
    Automatic Grouping: The map organizes planets by their orbit immediately upon reading the file.

    Uniqueness Guarantee: By iterating over the map keys, you ensure that you only choose one planet for each orbit number.

    Flexibility: If you have 50 planets in your file, but 10 are in orbit 1, the code will pick only one of those 10 at random and move on to search in orbit 2.

How to use it in main.cpp or a future Game class:

        // Generate the solar system for the match
        std::vector<Planet> solarSystem = PlanetManager::loadUniqueOrbitPlanets("planets_db.txt");

        // Now you can use them in your SFML logic
        for (const auto& p : solarSystem) {
            std::cout << "Planet: " << p.getName() << " | Orbit: " << p.getOrbit() << std::endl;
        }

*/

std::vector<Planet> PlanetManager::loadUniqueOrbitPlanets(const std::string& filename) {
    std::map<int, std::vector<Planet>> orbitPool;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) return {};

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Ignore empty lines

        std::stringstream ss(line);
        std::string name, desc, temp;
        int orb, tech, sec, res, lux, moon, med;

        // Read strings with getline
        std::getline(ss, name, ',');
        std::getline(ss, desc, ',');

        // For numbers, it's safer to use the >> operator and skip the comma
        auto readNextInt = [&](int &val) {
            std::getline(ss, temp, ',');
            try {
                val = std::stoi(temp);
            }
            catch (...) {
                val = 0; // Default value if parsing fails
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

    // --- selection logic ---
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Planet> selectedPlanets;

    for (int orbit = 1; orbit <= 10; ++orbit) {
        if (orbitPool.count(orbit) && !orbitPool[orbit].empty()) {
            std::uniform_int_distribution<int> dist(0, orbitPool[orbit].size() - 1);
            int randomIndex = dist(gen);

            selectedPlanets.push_back(orbitPool[orbit][randomIndex]);
        }
        else {
            std::cerr << "Warning: No planets defined for orbit " << orbit << std::endl;
        }
    }
    std::vector<int> availableOrbits;

    StockSorter::hybridSort(selectedPlanets.begin(), selectedPlanets.end(), [](const Planet& a, const Planet& b) {
        return a.getOrbit() < b.getOrbit();
    });
    return selectedPlanets;
}

std::vector<Planet> PlanetManager::loadPlanetsByName(const std::string& filename, const std::vector<std::string>& names) {
    std::unordered_map<std::string, Planet> allPlanets;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, desc, temp;
        int orb, tech, sec, res, lux, moon, med;

        std::getline(ss, name, ',');
        std::getline(ss, desc, ',');

        auto readNextInt = [&](int &val) {
            std::getline(ss, temp, ',');
            try { val = std::stoi(temp); } catch (...) { val = 0; }
        };

        readNextInt(orb);
        readNextInt(tech);
        readNextInt(sec);
        readNextInt(res);
        readNextInt(lux);
        readNextInt(moon);
        readNextInt(med);

        allPlanets[name] = Planet(name, desc, orb, tech, sec, res, lux, moon, med);
    }

    std::vector<Planet> result;
    for (const auto& name : names) {
        if (allPlanets.count(name)) {
            result.push_back(allPlanets[name]);
        }
    }

    return result;
}