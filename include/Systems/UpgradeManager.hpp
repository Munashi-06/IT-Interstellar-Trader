#pragma once
#include "DataStructures/BinaryNode.hpp"
#include "Entities/Upgrade.hpp"
#include "Entities/Player.hpp"
#include <memory>
#include <iostream>

class UpgradeManager {
private:
    // Pointers to the root nodes of our different upgrade tabs
    std::shared_ptr<BinNode<Upgrade>> logisticsTree;
    std::shared_ptr<BinNode<Upgrade>> propulsionTree;
    std::shared_ptr<BinNode<Upgrade>> tradingTree;

public:
    UpgradeManager();
    
    // Initializes all nodes, costs, and callbacks for each tree
    void initTrees(Player& player);
    
    // Attempts to purchase the upgrade pointed to by "node".
    // If the purchase is successful, funds are deducted, the upgrade effect is applied,
    // children are unlocked, and the sibling is blocked if mutually exclusive.
    bool purchaseUpgrade(std::shared_ptr<BinNode<Upgrade>> node, std::shared_ptr<BinNode<Upgrade>> siblingNode, float& playerMoney);

    // Getters so the UI can traverse and draw the upgrade trees
    std::shared_ptr<BinNode<Upgrade>> getLogisticsRoot() const { return logisticsTree; }
    std::shared_ptr<BinNode<Upgrade>> getPropulsionRoot() const { return propulsionTree; }
    std::shared_ptr<BinNode<Upgrade>> getTradingRoot() const { return tradingTree; }
};