#include "Systems/SaveSystem.hpp"
#include <iostream>

unsigned int SaveSystem::setGameSeed(const std::string& seedText) {
    unsigned int finalSeed;

    if (seedText.empty()) {
        finalSeed = static_cast<unsigned int>(std::time(nullptr));
        std::cout << "[SEMILLA] Generada aleatoriamente: " << finalSeed << std::endl;
    }
    else {
        std::hash<std::string> hasher;
        finalSeed = static_cast<unsigned int>(hasher(seedText));
        std::cout << "[SEMILLA] Texto '" << seedText << "' convertido a la semilla: " << finalSeed << std::endl;
    }

    std::srand(finalSeed);

    return finalSeed;
}