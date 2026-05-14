#pragma once
#include <string>
#include <vector>
#include <functional>
#include <ctime>
#include <cstdlib>
#include "Entities/Player.hpp"
#include "Systems/UpgradeManager.hpp"

// Structure that transfers data loaded from the file to the game
struct SaveData {
    std::string seedText;
    float money;
    short currentOrbit;
    std::vector<std::pair<std::string, int>> inventory;
    std::vector<std::string> purchasedUpgrades;
};

class SaveSystem {
private:
    static std::string currentSeedText; // Almacena la semilla actual en memoria

public:
    // Seed Management
    static unsigned int setGameSeed(const std::string& seedText);
    static std::string getCurrentSeedText();

    // Handling the Save File
    static bool saveExists();
    static bool saveGame(Player& player, const UpgradeManager& upgrades);
    static bool loadGame(SaveData& outData);

    static void handleImput();
};