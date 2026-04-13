#include "Heap.hpp"

// Constructor
Heap::Heap(Planet&& p) noexcept : BinNode<Planet>(std::move(p)) {
    this->heap_array.push_back(Planet()); // Posición 0 vacía
    this->heap_array.push_back(std::move(p)); // Posición 1 es la raíz
    this->n = 1;
}

void Heap::sift_up(std::vector<Planet>& arr, size_t idx, Cmp comparador) {
    while (idx > 1) {
        size_t padre = idx / 2;
        if (comparador(arr[idx], arr[padre])) {
            std::swap(arr[idx], arr[padre]);
            idx = padre;
        } else break;
    }
}

void Heap::sift_down(std::vector<Planet>& arr, size_t idx, Cmp comparador) {
    while (true) {
        size_t l = 2 * idx;
        size_t r = 2 * idx + 1;
        size_t c = idx;

        if (l <= n && comparador(arr[l], arr[c])) c = l;
        if (r <= n && comparador(arr[r], arr[c])) c = r;

        if (c != idx) {
            std::swap(arr[idx], arr[c]);
            idx = c;
        } else break;
    }
}

void Heap::insert(Planet&& p, std::vector<Planet>& arr, Cmp comparador) {
    arr.push_back(std::move(p));
    n++;
    sift_up(arr, n, comparador);
}

void Heap::remove(std::vector<Planet>& arr, Cmp comparador) {
    if (n == 0) return;
    arr[1] = std::move(arr[n]);
    arr.pop_back();
    n--;
    if (n > 0) sift_down(arr, 1, comparador);
}

void Heap::update(Planet& modified, std::vector<Planet>& arr, Cmp comparador) {
    // Para actualizar un planeta modificado, primero lo buscamos en el heap
    // Luego aplicamos sift-up o sift-down según corresponda
    for (size_t i = 1; i <= n; ++i) {
        if (arr[i].getName() == modified.getName()) { // Asumiendo que el nombre es único
            arr[i] = std::move(modified); // Actualizamos el planeta
            sift_up(arr, i, comparador);
            sift_down(arr, i, comparador);
            break;
        }
    }
}