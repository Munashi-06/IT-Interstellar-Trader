#pragma once
#include "Planet.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <random>

template <typename T, typename Cmp = std::less<>>
inline void insertionSort(std::vector<T> &a, Cmp&& cmp) {
    for(int i = 1; i < a.size(); i++) {
        T key = std::move(a[i]); 
        int j = i - 1;

        while(j >= 0 && cmp(key, a[j])) {
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