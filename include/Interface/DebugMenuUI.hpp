#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "Entities/Player.hpp"
#include "Systems/Inventory.hpp"
#include "Entities/Item.hpp"

class DebugMenuUI {
private:
    sf::Font font;
    bool isExpanded = false;
    std::string secretBuffer = "";
    bool isUnlocked = false;

    // Elementos visuales
    sf::RectangleShape toggleBtnBg;
    sf::Text toggleBtnText;

    sf::RectangleShape panelBg;
    
    // Botón Dinero
    sf::RectangleShape moneyBtnBg;
    sf::Text moneyBtnText;

    // Botón Ganar (Warp)
    sf::RectangleShape winBtnBg;
    sf::Text winBtnText;

    // Selector de Ítems
    sf::RectangleShape prevItemBtn;
    sf::Text prevItemText;
    sf::RectangleShape nextItemBtn;
    sf::Text nextItemText;
    
    sf::RectangleShape giveItemBtn;
    sf::Text giveItemText;
    
    sf::Text currentItemDisplay;

    // Lógica del catálogo
    std::vector<std::string> itemIDs;
    int currentItemIndex = 0;

    // Función auxiliar para centrar texto
    void centerText(sf::Text& text, const sf::RectangleShape& rect);

public:
    DebugMenuUI(const sf::Font& f);

    // Se llama una vez para cargar la lista de IDs del catálogo
    void initCatalog(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

    void handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Player& player, Inventory& playerInv);
    void update(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window);
};