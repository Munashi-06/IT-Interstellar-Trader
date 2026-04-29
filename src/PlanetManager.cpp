#include "PlanetManager.hpp"

/*
¿Por qué usar un std::map<int, std::vector<Planet>>?
    Agrupación Automática: El mapa organiza los planetas por su órbita inmediatamente al leer el archivo.

    Garantía de Unicidad: Al iterar sobre las llaves del mapa, te aseguras de que solo eliges un planeta por cada número de órbita.

    Flexibilidad: Si en tu archivo tienes 50 planetas, pero 10 están en la órbita 1, el código elegirá solo uno de esos 10 al azar y pasará a buscar en la órbita 2.

Cómo usarlo en  main.cpp o en una futura clase Game:

        // Generar el sistema solar para la partida
        std::vector<Planet> sistemaSolar = PlanetManager::loadUniqueOrbitPlanets("planets_db.txt");

        // Ahora puedes usarlos en tu lógica de SFML
        for (const auto& p : sistemaSolar) {
            std::cout << "Planeta: " << p.getName() << " | Orbita: " << p.getOrbit() << std::endl;
        }

*/

std::vector<Planet> PlanetManager::loadUniqueOrbitPlanets(const std::string& filename) {
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

    std::vector<Planet> selectedPlanets;

    for (int orbit = 1; orbit <= 10; ++orbit) {
        if (orbitPool.count(orbit) && !orbitPool[orbit].empty()) {
            std::uniform_int_distribution<int> dist(0, orbitPool[orbit].size() - 1);
            int randomIndex = dist(g);
        
            selectedPlanets.push_back(orbitPool[orbit][randomIndex]);
        } else {
            std::cerr << "Aviso: No hay planetas definidos para la orbita " << orbit << std::endl;
        }
    }
    std::vector<int> availableOrbits;

    insertionSort(selectedPlanets);
    return selectedPlanets;
}