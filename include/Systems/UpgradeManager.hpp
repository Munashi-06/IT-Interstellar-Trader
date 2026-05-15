#pragma once
#include "DataStructures/BinaryNode.hpp"
#include "Entities/Upgrade.hpp"
#include "Entities/Player.hpp"
#include <memory>
#include <iostream>
#include <algorithm>

class UpgradeManager {
private:
    // Pointers to the root nodes of our different upgrade tabs
    std::shared_ptr<BinNode<Upgrade>> logisticsTree;
    std::shared_ptr<BinNode<Upgrade>> propulsionTree;
    std::shared_ptr<BinNode<Upgrade>> tradingTree;

    private:
    void collectPurchasedWithDepth(std::shared_ptr<BinNode<Upgrade>> node, int depth, std::vector<std::pair<std::string, int>>& output);
    void collectPurchased(std::shared_ptr<BinNode<Upgrade>> node, std::vector<std::string>& list) const;
    bool findNodeAndSibling(std::shared_ptr<BinNode<Upgrade>> current, std::shared_ptr<BinNode<Upgrade>> sibling,
        const std::string& targetID, std::shared_ptr<BinNode<Upgrade>>& outNode, std::shared_ptr<BinNode<Upgrade>>& outSibling) const;
    void resetNodeStatus(std::shared_ptr<BinNode<Upgrade>> node, bool isRoot);

    // Find the improvement at the end of the branch
    std::string findDeepestPurchased(std::shared_ptr<BinNode<Upgrade>> node) const;
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

    std::vector<std::string> getPurchasedUpgrades() const;
    void loadPurchasedUpgrades(const std::vector<std::string>& purchasedIDs);
    void resetTrees();

    // treeType: 1 = Propulsion, 2 = Logistics, 3 = Trade
    // Returns ‘true’ if it successfully disabled something.
    // branch (Optional): 0 = Any, 1 = Left branch, 2 = Right branch
    bool deactivateDeepestUpgrade(Player& player, int treeType, int branch);
};