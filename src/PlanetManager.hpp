#pragma once
#include "Planet.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <random>

inline void insertionSort(std::vector<Planet> &a) {
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