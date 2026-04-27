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
    std::vector<Planet> paraMostrar;
    
    // 1. Obtenemos los planetas del sistema solar
    for (const auto& p : solarSystem) {
        paraMostrar.push_back(p);
    }

    // 2. Ordenamos el vector completo usando tu función cmp
    // Esto garantiza un orden lineal perfecto para la UI
    std::sort(paraMostrar.begin(), paraMostrar.end(), cmp);

    // 3. Pasamos este vector ordenado al Radar
    // Nota: El radar ahora recibirá un vector ordenado del 0 al N, 
    // así que ajusta el bucle del RadarUI para empezar en i=0
    radar->setHeapArray(paraMostrar);
}