#pragma once
#include "BinaryNode.hpp"
#include "Planet.hpp"
#include <vector>

typedef bool (*Cmp)(const Planet&, const Planet&);

inline bool cmp(const Planet& a, const Planet& b) // Función de comparación para ordenar los planetas en el heap
{
/*
    Aun sigo pensando como se podrian hacer las comparaciones pero se podrian comparar por
    el evento que esta ocurriendo en el planeta, la duracion y el nivel tecnologico,
    medico, de seguridad... etc. dependiendo del evento se compararia por diferentes atributos
    para ver cual es el que tiene mayor descuento o el que es mas urgente visitar.

    Comparar por atributos tambien ayuda a mantener el Heap ordenado a pesar de no tener eventos,
    ya que el jugador puede decidir visitar un planeta sin eventos pero con un nivel tecnologico alto
    para comprar tecnologia, o un planeta con alta abundancia de recursos para comprar comida, etc.
*/
    float scoreA = 0.0f;
    float scoreB = 0.0f;

    // Sumamos modificadores (puedes usar los atributos que ya tienes)
    if(a.getEvent() == PlanetEvent::War) { // Guerra
        scoreA = 50.0f;
        scoreA += a.getSecurityLevel() * 2.0f; // Más inseguro = más prioridad
    }
    else if(a.getEvent() == PlanetEvent::Famine) { // Hambruna
        scoreA = 45.0f;
        scoreA += a.getResourceAbundance() * 1.75f; // Menos recursos = más prioridad
    }
    else if(a.getEvent() == PlanetEvent::Plague) { // Peste
        scoreA = 40.0f;
        scoreA += a.getMedicalTech() * 1.5f; // Mejor tecnología médica = más prioridad
    }
    else if(a.getEvent() == PlanetEvent::TechBoom) { // Auge Tecnológico
        scoreA = 30.0f;
        scoreA += a.getTechLevel() * 1.25f; // Más tecnológico = más prioridad
    }

    if(b.getEvent() == PlanetEvent::War) { // Guerra
        scoreB = 50.0f;
        scoreB += b.getSecurityLevel() * 2.0f; // Más inseguro = más prioridad
    }
    else if(b.getEvent() == PlanetEvent::Famine) { // Hambruna
        scoreB = 45.0f;
        scoreB += b.getResourceAbundance() * 1.75f; // Menos recursos = más prioridad
    }
    else if(b.getEvent() == PlanetEvent::Plague) { // Peste
        scoreB = 40.0f;
        scoreB += b.getMedicalTech() * 1.5f; // Mejor tecnología médica = más prioridad
    }
    else if(b.getEvent() == PlanetEvent::TechBoom) { // Auge Tecnológico
        scoreB = 30.0f;
        scoreB += b.getTechLevel() * 1.25f; // Más tecnológico = más prioridad
    }

    return scoreA > scoreB; // Max-Heap: el mayor puntaje arriba
}

// Max-Heap de Planetas basado en eventos y atributos
class Heap : public BinNode<Planet>
{
private:
    std::vector<Planet> heap_array; // Almacena los planetas en un arreglo para facilitar las operaciones de sift-up y sift-down
    size_t n = 0; // Tamaño actual del heap
    // Funciones auxiliares para mantener la propiedad del heap reciben como comparador la funcion cmp declarada arriba
    void sift_up(std::vector<Planet>& arr, size_t idx, bool (*cmp)(const Planet&, const Planet&));
    void sift_down(std::vector<Planet>& arr, size_t idx, bool (*cmp)(const Planet&, const Planet&));
public:
    Heap(Planet&& p) noexcept;

    std::vector<Planet>& getHeapArray() noexcept { return heap_array; }

    void insert(Planet&& p, std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&));
    void remove(std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&));

    // Actualizar el heap después de modificar un planeta (por ejemplo, después de que un evento termine)
    void update(Planet& modified, std::vector<Planet>& arr, bool (*cmp)(const Planet&, const Planet&));
};