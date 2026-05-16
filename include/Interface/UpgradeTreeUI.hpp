#pragma once
#include <SFML/Graphics.hpp>
#include "Systems/UpgradeManager.hpp"
#include "Core/State.hpp"

class UpgradeTreeUI {
private:
    // Main background panel
    sf::RectangleShape background;
    sf::Text titleText;
    const sf::Font& font;

    // UI Elements for navigation tabs
    sf::RectangleShape tabLogistics, tabPropulsion, tabTrading;
    sf::Text textLogistics;
    sf::Text textPropulsion;
    sf::Text textTrading;

    // --- TOOLTIP VARIABLES ---
    sf::RectangleShape tooltipBg;
    sf::Text tooltipText;
    bool showTooltip = false;

    // --- NEW: BACK BUTTON & MONEY ---
    sf::RectangleShape backBtn;
    sf::Text backText;
    sf::Text moneyText;

    // Tracks which upgrade tree is currently visible (0 = Logistics, 1 = Propulsion, 2 = Trading)
    int currentTab = 0; 

    // Helper function to draw connecting lines between a parent upgrade and its children
    void drawConnection(sf::RenderWindow& window, sf::Vector2f parentPos, sf::Vector2f childPos, sf::Color color);
    
    // Recursive function to traverse the BinaryNode tree and draw each upgrade box
    void drawNode(sf::RenderWindow& window, std::shared_ptr<BinNode<Upgrade>> node, sf::Vector2f pos, float hSpacing, float vSpacing);

    // Helper to find which node the mouse is currently hovering over
    std::shared_ptr<BinNode<Upgrade>> getHoveredNode(std::shared_ptr<BinNode<Upgrade>> node, sf::Vector2f pos, float hSpacing, float vSpacing, const sf::Vector2f& mousePos);

    // Helper to wrap long text into multiple lines
    std::string wrapText(const std::string& text, unsigned maxCharsPerLine);
public:
    UpgradeTreeUI(const sf::Font& f);
    
    // Renders the background, tabs, and calls the recursive tree drawing
    void draw(sf::RenderWindow& window, UpgradeManager& manager, float playerMoney);    
    // Handles tab switching and upgrade purchasing clicks
    std::string handleInput(const sf::Event& event, const sf::Vector2f& mousePos, UpgradeManager& manager, float& playerMoney, State& currentState);    
    
    // Helper for detecting clicks on nodes and handling mutually exclusive siblings
    void handleNodeClick(std::shared_ptr<BinNode<Upgrade>> node, std::shared_ptr<BinNode<Upgrade>> sibling, sf::Vector2f pos, float hSpacing, float vSpacing, const sf::Vector2f& mousePos, UpgradeManager& manager, float& playerMoney, std::string& outMessage);
    
    // Handles hover effects for tabs and upgrade nodes
    void update(const sf::Vector2f& mousePos, UpgradeManager& manager);
};