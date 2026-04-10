#include "Heap.hpp"

Heap::Heap(Planet&& p) noexcept
    : BinNode<Planet>(std::move(p))
{
    // Agrega el planeta inicial al heap_array en la posicion 1 (null, planeta)
    this->heap_array.push_back(Planet());
    this->heap_array[1] = std::move(p);
    this->n = 1;
}

void Heap::sift_up(std::vector<Planet>& arr, size_t idx, Cmp cmp)
{
    // Mientras no sea la raíz y el hijo sea "mejor" que el padre
    while (idx > 1) {
        size_t padre = idx / 2;
        if (cmp(arr[idx], arr[padre])) {
            std::swap(arr[idx], arr[padre]);
            idx = padre;
        } else {
            break;
        }
    }
}


void Heap::sift_down(std::vector<Planet>& arr, size_t idx, Cmp cmp)
{
        size_t l, r, c;
    while (true) {
        l = 2 * idx;
        r = 2 * idx + 1;
        c = idx;
        
        if (l <= n && cmp(arr[l], arr[c])) c = l;
        if (r <= n && cmp(arr[r], arr[c])) c = r;

        if (c != idx) {
            std::swap(arr[idx], arr[c]);
            idx = c;
        }
        else {
            break;
        }
    }
}

void Heap::insert(Planet&& p, std::vector<Planet>& arr, Cmp cmp = cmp)
{
    // Agrega el nuevo planeta al final del arreglo
    arr.push_back(std::move(p));
    n++;

    // Mantiene la propiedad del heap
    sift_up(arr, n, cmp);
}

void Heap::remove(std::vector<Planet>& arr, Cmp cmp = cmp)
{
    if (n == 0) return; // Heap vacío

    // Reemplaza la raíz con el último elemento
    arr[1] = std::move(arr[n]);
    arr.pop_back();
    n--;

    // Mantiene la propiedad del heap
    sift_down(arr, 1, cmp);
}

void Heap::update(Planet& modified, std::vector<Planet>& arr, Cmp cmp = cmp)
{
    // Recorre el heap para encontrar el planeta modificado
    // como solo se van a tener unos pocos planetas en el heap, esto no debería ser un gran problema de rendimiento
    for (size_t i = 1; i <= n; ++i) {
        if (arr[i].getName() == modified.getName()) { // Asumiendo que el nombre es único
            sift_up(arr, i, cmp);
            if (arr[i].getName() == modified.getName()) { // Si no se movió hacia arriba, intenta hacia abajo
                sift_down(arr, i, cmp);
            }
            break;
        }
    }
}