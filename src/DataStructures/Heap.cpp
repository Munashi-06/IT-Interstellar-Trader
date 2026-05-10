#include "DataStructures/Heap.hpp"

// Constructor
Heap::Heap(Planet&& p) noexcept : BinNode<Planet>(std::move(p)) {
    this->heap_array.push_back(std::move(p)); // Position 1 is the root
    this->n = 1;
}

void Heap::sift_up(std::vector<Planet>& arr, size_t idx, Cmp comparador) {
    while (idx > 0) { // The limit is now 0, not 1
        size_t padre = (idx - 1) / 2; // New parent formula
        if (comparador(arr[idx], arr[padre])) {
            std::swap(arr[idx], arr[padre]);
            idx = padre;
        } else break;
    }
}

void Heap::sift_down(std::vector<Planet>& arr, size_t idx, Cmp comparador) {
    size_t size = arr.size();
    while (true) {
        // New formula for children since the heap was made to start at position 0 of the array
        size_t l = 2 * idx + 1;
        size_t r = 2 * idx + 2;
        size_t c = idx;

        if (l < size && comparador(arr[l], arr[c])) c = l;
        if (r < size && comparador(arr[r], arr[c])) c = r;

        if (c != idx) {
            std::swap(arr[idx], arr[c]);
            idx = c;
        }
        else { break; }
    }
}

void Heap::insert(Planet&& p, std::vector<Planet>& arr, Cmp comparador) {
    arr.push_back(std::move(p));
    sift_up(arr, arr.size() - 1, comparador);
    this->n = arr.size(); 
}

void Heap::insertArray(std::vector<Planet>& arr, Cmp comparador) {
    for (size_t i = 1; i <= n; ++i) {
        sift_up(arr, i, comparador);
    }
}

void Heap::remove(std::vector<Planet>& arr, Cmp comparador) {
    if (arr.empty()) return;
    
    // We move the last one to the first
    arr[0] = std::move(arr.back());
    arr.pop_back();
    
    if (!arr.empty()) {
        sift_down(arr, 0, comparador);
    }
    this->n = arr.size();
}

void Heap::update(Planet& modified, std::vector<Planet>& arr, Cmp comparador) {
    // To update a modified planet, we first search for it in the heap
    // Then we apply sift-up or sift-down as appropriate
    for (size_t i = 1; i <= n; ++i) {
        if (arr[i].getName() == modified.getName()) {
            arr[i] = std::move(modified); // We update the planet
            sift_up(arr, i, comparador);
            sift_down(arr, i, comparador);
            break;
        }
    }
}