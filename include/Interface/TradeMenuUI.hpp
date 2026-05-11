#pragma once
#include <SFML/Graphics.hpp>
#include "Systems/TradeManager.hpp"
#include "Interface/ShipMenuUI.hpp" // To use SortColumn

class TradeMenuUI {
private:
// --- GENERAL VARIABLES ---
    sf::Text titleText;
    int maxVisibleRows = 10; // Limit of items to display in the tables
    
// --- PLAYER SIDE (Left) ---
    sf::RectangleShape playerTableBg;
    sf::Text playerSectionTitle;
    sf::Text playerMoneyText;
    sf::Text headerPlayerCategory;
    sf::Text headerPlayerQuality;   
    
    // Player table headers
    sf::Text headerPlayerName;
    sf::Text headerPlayerQty;
    sf::Text headerPlayerSellPrice;
    
    int playerStartIndex = 0; // For scrolling the player table
    SortColumn playerCurrentSort = SortColumn::Name;
    bool playerSortAscending = true;

// --- PLANET SIDE (Right) ---
    sf::RectangleShape planetTableBg;
    sf::Text planetNameText;
    sf::Text headerPlanetCategory;
    sf::Text headerPlanetQuality;
    
    // Planet table headers
    sf::Text headerPlanetName;
    sf::Text headerPlanetQty;
    sf::Text headerPlanetBuyPrice;
    
    int planetStartIndex = 0; // For scrolling the planet table
    SortColumn planetCurrentSort = SortColumn::Name; // NEW
    bool planetSortAscending = true;


// --- SELECTION AND BUTTON VARIABLES ---
    sf::RectangleShape selectionHighlight; // The rectangle that draws the "highlight" over the selected row

    // Main action buttons
    sf::RectangleShape playerActionBtnBg;
    sf::Text playerActionBtnText; // "SELL"
    
    sf::RectangleShape planetActionBtnBg;
    sf::Text planetActionBtnText; // "BUY"
// --- CONTEXT MENU (Right Click) ---
    bool showContextMenu = false;
    sf::Vector2f contextMenuPos;
    std::string selectedItemID = "";
    bool isPlayerItem = true; // true = click was on player's inventory (Sell), false = on market (Buy)

    sf::RectangleShape contextMenuBg;
    sf::Text ctxBtnInfo;
    sf::Text ctxBtnAction; // Will say "Sell" or "Buy"
    sf::Text ctxBtnCancel;

// --- INFO POPUP ---
    bool showInfoPopup = false;
    sf::RectangleShape infoPopupBg;
    sf::Text infoPopupText;
    sf::Text infoPopupEscText;

    sf::Text infoInputText; // To tell the player which keys can be used in the market

    const sf::Font& font;

    // --- Helper function to calculate the visibility percetage
    float getVisibilityPercent(Rarity rarity, int shipLevel) const;


public:
    TradeMenuUI(const sf::Font& f);
    
    void draw(sf::RenderWindow& window, const Inventory& playerInv, const Planet& currentPlanet, float playerMoney, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog, int playerShipLevel = 1);
    
    // This is where we will process clicks for "Sell" or "Buy"
    void handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Inventory& playerInv, Planet& currentPlanet, Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

    void update(const sf::Vector2f& mousePos);

    bool isInfoPopupOpen() const { return showInfoPopup; }
    void closeInfoPopup() { showInfoPopup = false; }
};