#include "World.hpp"

bool World::update() {
    // se llama en cada frame del bucle principal para manejar contadores de tiempo (como la duración de los eventos).
    // Aquí podrías actualizar el estado del mundo, como avanzar eventos activos, manejar misiones, etc.
    
    // Solo si algo cambió realmente en los eventos, actualizamos el radar
    // Esto evita recalcular el heap cada frame si no es necesario, lo que mejora el rendimiento
    bool huboCambio = stateManager.update(deltaTime, solarSystem);
    
    if (huboCambio) {
        forceRadarUpdate();
    }
    return huboCambio; // Devuelve si hubo cambios al main para que pueda actualizar la interfaz si es necesario
}

void World::updateRadar(Planet& plnt) noexcept {
    // Actualiza el heap de radar después de modificar un planeta (por ejemplo, después de que un evento termine)
    radar->remove(radar->getHeapArray(), cmp);
    radar->insert(std::move(plnt), radar->getHeapArray(), cmp);
}

void World::forceRadarUpdate() {
    // 1. En lugar de limpiar el original, crea uno temporal
    std::vector<Planet> nuevoHeap;
    nuevoHeap.push_back(Planet()); // Índice 0
    
    // IMPORTANTE: Reseteamos el contador interno del heap 
    // para que coincida con el nuevo vector que vamos a llenar
    radar->setN(0);

    for (const auto& p : solarSystem) {
        // Usa una función de inserción que reciba el vector por parámetro
        // o asegúrate de que insert() no asuma que el vector ya tiene espacio
        radar->insert(Planet(p), nuevoHeap, cmp); 
    }

    // 2. Solo al final, asigna el nuevo heap al radar
    radar->setHeapArray(nuevoHeap); 
}