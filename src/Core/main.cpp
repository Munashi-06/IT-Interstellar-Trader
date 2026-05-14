#include "Core/Engine.hpp"
#include "Systems/AssetManager.hpp"

int main() {
    if (!AssetManager::loadAll()) return -1;

    Game::Engine engine;
    
    if (engine.init()) {
        engine.run();
    }
    return 0;
}