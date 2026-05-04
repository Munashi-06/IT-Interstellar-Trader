#pragma once
#include <SFML/Graphics.hpp>
#include "TradeManager.hpp"
#include "ShipMenuUI.hpp" // Para usar SortColumn

class TradeMenuUI {
private:
// --- VARIABLES GENERALES ---
    sf::Text titleText;
    int maxVisibleRows = 10; // Límite de ítems a mostrar en las tablas
    
// --- LADO DEL JUGADOR (Izquierda) ---
    sf::RectangleShape playerTableBg;
    sf::Text playerSectionTitle;
    sf::Text playerMoneyText;
    sf::Text headerPlayerCategory;
    sf::Text headerPlayerQuality;   
    
    // Cabeceras de la tabla del jugador
    sf::Text headerPlayerName;
    sf::Text headerPlayerQty;
    sf::Text headerPlayerSellPrice;
    
    int playerStartIndex = 0; // Para el scroll de la tabla del jugador
    SortColumn playerCurrentSort = SortColumn::Name;
    bool playerSortAscending = true;

// --- LADO DEL PLANETA (Derecha) ---
    sf::RectangleShape planetTableBg;
    sf::Text planetNameText;
    sf::Text headerPlanetCategory;
    sf::Text headerPlanetQuality;
    
    // Cabeceras de la tabla del planeta
    sf::Text headerPlanetName;
    sf::Text headerPlanetQty;
    sf::Text headerPlanetBuyPrice;
    
    int planetStartIndex = 0; // Para el scroll de la tabla del planeta
    SortColumn planetCurrentSort = SortColumn::Name; // NUEVO
    bool planetSortAscending = true;


// --- VARIABLES DE SELECCIÓN Y BOTONES ---
    sf::RectangleShape selectionHighlight; // El rectángulo que dibuja el "resaltado" sobre la fila seleccionada

    // Botones de acción principales
    sf::RectangleShape playerActionBtnBg;
    sf::Text playerActionBtnText; // "VENDER"
    
    sf::RectangleShape planetActionBtnBg;
    sf::Text planetActionBtnText; // "COMPRAR"
// --- MENU CONTEXTUAL (Clic Derecho) ---
    bool showContextMenu = false;
    sf::Vector2f contextMenuPos;
    std::string selectedItemID = "";
    bool isPlayerItem = true; // true = el clic fue en el inventario del jugador (Vender), false = en el mercado (Comprar)

    sf::RectangleShape contextMenuBg;
    sf::Text ctxBtnInfo;
    sf::Text ctxBtnAction; // Dirá "Vender" o "Comprar"
    sf::Text ctxBtnCancel;

// --- POPUP DE INFORMACIÓN ---
    bool showInfoPopup = false;
    sf::RectangleShape infoPopupBg;
    sf::Text infoPopupText;
    sf::Text infoPopupEscText;

    sf::Text infoInputText; // Para decirle al jugador que teclas se pueden usar en el mercado

    const sf::Font& font;

public:
    TradeMenuUI(const sf::Font& f);
    
    // El draw necesita conocer el inventario del jugador, el planeta actual y el catálogo global
    void draw(sf::RenderWindow& window, const Inventory& playerInv, const Planet& currentPlanet, float playerMoney, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Aquí es donde procesaremos el clic para "Vender" o "Comprar"
    void handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Inventory& playerInv, Planet& currentPlanet, Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

    void update(const sf::Vector2f& mousePos);

    bool isInfoPopupOpen() const { return showInfoPopup; }
    void closeInfoPopup() { showInfoPopup = false; }
};