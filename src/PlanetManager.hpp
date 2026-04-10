#include "Planet.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <random>
#include <algorithm>

void insertion_sort(std::vector<Planet> &a) {
    for(int i = 1; i < a.size(); i++) {
        Planet key = std::move(a[i]); 
        int j = i - 1;

        while(j >= 0 && a[j].getOrbit() > key.getOrbit()) {
            a[j + 1] = std::move(a[j]);
            j--;
        }

        a[j + 1] = std::move(key);
    }
}

class PlanetManager {
public:
    static std::vector<Planet> loadUniqueOrbitPlanets(const std::string& filename);
};

/*
¿Por qué usar un std::map<int, std::vector<Planet>>?
    Agrupación Automática: El mapa organiza los planetas por su órbita inmediatamente al leer el archivo.

    Garantía de Unicidad: Al iterar sobre las llaves del mapa, te aseguras de que solo eliges un planeta por cada número de órbita.

    Flexibilidad: Si en tu archivo tienes 50 planetas, pero 10 están en la órbita 1, el código elegirá solo uno de esos 10 al azar y pasará a buscar en la órbita 2.

Cómo usarlo en tu main.cpp o en tu clase Game:

        // Generar el sistema solar para la partida
        std::vector<Planet> sistemaSolar = PlanetManager::loadUniqueOrbitPlanets("planets_db.txt");

        // Ahora puedes usarlos en tu lógica de SFML
        for (const auto& p : sistemaSolar) {
            std::cout << "Planeta: " << p.getName() << " | Orbita: " << p.getOrbit() << std::endl;
        }

*/