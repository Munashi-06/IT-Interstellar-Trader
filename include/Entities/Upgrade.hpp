#pragma once
#include <string>
#include <functional>

// Represents the current state of an upgrade in the Tree
enum class UpgradeStatus { 
    LOCKED,                  // Cannot be purchased yet (parent not purchased)
    AVAILABLE,               // Ready to be purchased
    PURCHASED,               // Already purchased and applied
    BLOCKED_BY_CHOICE        // Locked forever because the player chose the mutually exclusive sibling
};

struct Upgrade {
    std::string id;
    std::string name;
    std::string description;
    float cost;
    UpgradeStatus status;
    
    // Determines if buying this upgrade permanently locks the sibling node in the binary tree
    bool isMutuallyExclusive; 

    // Contains the specific logic of what this upgrade does.
    std::function<void()> applyEffect;

    Upgrade(std::string id, std::string name, std::string desc, float cost, bool exclusive, std::function<void()> effect)
        : id(id), name(name), description(desc), cost(cost), 
          status(UpgradeStatus::LOCKED), isMutuallyExclusive(exclusive), applyEffect(effect) {}
};