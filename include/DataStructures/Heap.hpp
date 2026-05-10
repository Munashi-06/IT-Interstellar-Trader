#pragma once
#include "DataStructures/BinaryNode.hpp"
#include "Entities/Planet.hpp"
#include <vector>

typedef bool (*Cmp)(const Planet&, const Planet&);

inline bool cmp(const Planet& a, const Planet& b) // Comparison function for sorting planets in the heap
{
    if (a.getEvent() == PlanetEvent::None && b.getEvent() != PlanetEvent::None) {
        return false; // b has an event, a does not, so b is more urgent
    }
    if (a.getEvent() != PlanetEvent::None && b.getEvent() == PlanetEvent::None) {
        return true; // a has an event, b does not, so a is more urgent
    }
    float scoreA = 0.0f;    
    float scoreB = 0.0f;

    // Add modifiers (you can use the attributes you already have)

    // -- PLANET A --
    if(a.getEvent() == PlanetEvent::War) { // War
        scoreA = 50.0f;
        scoreA += (10.0f - a.getSecurityLevel()) * 2.0f; // More insecure = higher priority
    }
    else if(a.getEvent() == PlanetEvent::Famine) { // Famine
        scoreA = 45.0f;
        scoreA += (10.0f - a.getResourceAbundance()) * 1.75f; // Fewer resources = higher priority
    }
    else if(a.getEvent() == PlanetEvent::Plague) { // Plague
        scoreA = 40.0f;
        scoreA += (10.0f - a.getMedicalTech()) * 1.5f; // Lower medical technology = higher priority
    }
    else if(a.getEvent() == PlanetEvent::TechBoom) { // Tech Boom
        scoreA = 30.0f;
        scoreA += a.getTechLevel() * 1.25f; // More technological = higher priority
    }


    // -- PLANET B --
    if(b.getEvent() == PlanetEvent::War) { // War
        scoreB = 50.0f;
        scoreB += (10.0f - b.getSecurityLevel()) * 2.0f; // More insecure = higher priority
    }
    else if(b.getEvent() == PlanetEvent::Famine) { // Famine
        scoreB = 45.0f;
        scoreB += (10.0f - b.getResourceAbundance()) * 1.75f; // Fewer resources = higher priority
    }
    else if(b.getEvent() == PlanetEvent::Plague) { // Plague
        scoreB = 40.0f;
        scoreB += (10.0f - b.getMedicalTech()) * 1.5f; // Lower medical technology = higher priority
    }
    else if(b.getEvent() == PlanetEvent::TechBoom) { // Tech Boom
        scoreB = 30.0f;
        scoreB += b.getTechLevel() * 1.25f; // More technological = higher priority
    }

    return scoreA > scoreB; // Max-Heap: highest score on top
}

// Max-Heap of Planets based on events and attributes
class Heap : public BinNode<Planet>
{
private:
    std::vector<Planet> heap_array; // Stores planets in an array to facilitate sift-up and sift-down operations
    size_t n = 0; // Current size of the heap
    // Helper functions to maintain the heap property, using the cmp function declared above as comparator
    void sift_up(std::vector<Planet>& arr, size_t idx, bool (*cmp)(const Planet&, const Planet&));
    void sift_down(std::vector<Planet>& arr, size_t idx, bool (*cmp)(const Planet&, const Planet&));
public:
    Heap(Planet&& p) noexcept;

    std::vector<Planet>& getHeapArray() noexcept { return heap_array; }

    void setHeapArray(std::vector<Planet>& newArr) noexcept { heap_array = std::move(newArr); n = heap_array.size() - 1; }
    void setN(size_t newN) noexcept { n = newN; }

    void insert(Planet&& p, std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&));
    void insertArray(std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&)); // Inserts an entire vector into the heap (useful for full radar updates)
    void remove(std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&));

    // Update the heap after modifying a planet (for example, after an event ends)
    void update(Planet& modified, std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&));
};