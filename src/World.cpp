#include "World.hpp"

void World::update() {
    // se llama en cada frame del bucle principal para manejar contadores de tiempo (como la duración de los eventos).
    // Aquí podrías actualizar el estado del mundo, como avanzar eventos activos, manejar misiones, etc.
}

void World::updateRadar(Planet& plnt) noexcept {
    // Actualiza el heap de radar después de modificar un planeta (por ejemplo, después de que un evento termine)
    radar->remove(radar->getHeapArray(), cmp);
    radar->insert(std::move(plnt), radar->getHeapArray(), cmp);
}