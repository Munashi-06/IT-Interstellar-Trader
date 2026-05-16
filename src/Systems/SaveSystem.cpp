#include "Systems/SaveSystem.hpp"
#include "Entities/Planet.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string SaveSystem::currentSeedText = "";

unsigned int SaveSystem::setGameSeed(const std::string& seedText) {
    currentSeedText = seedText;
    unsigned int finalSeed;
    if (seedText.empty()) {
        finalSeed = static_cast<unsigned int>(std::time(nullptr));
        currentSeedText = std::to_string(finalSeed); // If it's a random number, we store it as text
    } else {
        std::hash<std::string> hasher;
        finalSeed = static_cast<unsigned int>(hasher(seedText));
    }
    std::srand(finalSeed);
    std::cout << "[SEED] Universe initialized with: " << currentSeedText << "\n";
    return finalSeed;
}

std::string SaveSystem::getCurrentSeedText() {
    return currentSeedText;
}

bool SaveSystem::saveExists() {
    std::ifstream file("save.dat");
    return file.good();
}

bool SaveSystem::saveGame(Player& player, const UpgradeManager& upgrades, const std::vector<Planet>& planets) {
    std::ofstream file("save.dat");
    if (!file.is_open()) {
        std::cerr << "[SAVE ERROR] Unable to open save.dat for writing.\n";
        return false;
    }

    file << "SEED:" << currentSeedText << "\n";
    file << "MONEY:" << player.getMoney() << "\n";
    file << "ORBIT:" << player.getCurrentOrbit() << "\n";

    // Format inventory as ID=Qty,ID=Qty
    file << "INVENTORY:";
    const auto& slots = player.getInventory().getSlots();
    bool first = true;
    for (const auto& slot : slots) {
        if (slot.has_value()) {
            if (!first) file << ",";
            file << slot->itemID << "=" << slot->quantity;
            first = false;
        }
    }
    file << "\n";

    file << "UPGRADES:";
    auto purchased = upgrades.getPurchasedUpgrades();
    bool firstUpg = true;
    for (const auto& id : purchased) {
        if (!firstUpg) file << ",";
        file << id;
        firstUpg = false;
    }
    file << "\n";

    file << "PLANETS:";
    for (size_t i = 0; i < planets.size(); ++i) {
        if (i > 0) file << ",";
        file << planets[i].getName();
    }
    file << "\n";

    std::cout << "[SAVE] Save file saved successfully.\n";
    return true;
}

bool SaveSystem::saveGame(Player& player, const UpgradeManager& upgrades) {
    std::vector<Planet> empty;
    return saveGame(player, upgrades, empty);
}

bool SaveSystem::loadGame(SaveData& outData) {
    std::ifstream file("save.dat");
    if (!file.is_open()) {
    std::cout << "ERROR" << std::endl;
       return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        std::getline(ss, key, ':');

        if (key == "SEED") {
            std::getline(ss, outData.seedText);
        } 
        else if (key == "MONEY") {
            std::string val;
            std::getline(ss, val);
            outData.money = std::stof(val);
        } 
        else if (key == "ORBIT") {
            std::string val;
            std::getline(ss, val);
            outData.currentOrbit = static_cast<short>(std::stoi(val));
        }
        else if (key == "INVENTORY") {
            std::string itemsStr;
            std::getline(ss, itemsStr);
            std::stringstream itemStream(itemsStr);
            std::string pairStr;
            
            // Split the string “Titanium=5,Rations=10”
            while (std::getline(itemStream, pairStr, ',')) {
                size_t eqPos = pairStr.find('=');
                if (eqPos != std::string::npos) {
                    std::string id = pairStr.substr(0, eqPos);
                    int qty = std::stoi(pairStr.substr(eqPos + 1));
                    outData.inventory.push_back({id, qty});
                }
            }
        }
        else if (key == "UPGRADES") {
            std::string upgStr;
            std::getline(ss, upgStr);
            std::stringstream upgStream(upgStr);
            std::string id;
            while (std::getline(upgStream, id, ',')) {
                if (!id.empty() && id.back() == '\r') {
                    id.pop_back(); // Borra el '\r' invisible
                }
                if (!id.empty()) outData.purchasedUpgrades.push_back(id);
            }
        }
        else if (key == "PLANETS") {
            std::string planetsStr;
            std::getline(ss, planetsStr);
            std::stringstream planetStream(planetsStr);
            std::string name;
            while (std::getline(planetStream, name, ',')) {
                if (!name.empty()) outData.planetNames.push_back(name);
            }
        }
    }
    std::cout << "[SAVE] Save file loaded successfully.\n";
    return true;
}