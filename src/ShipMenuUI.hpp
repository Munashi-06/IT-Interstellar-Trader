#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.hpp"
#include "Item.hpp"
#include <vector>
#include <string>

// Enum para saber por qué columna estamos ordenando
enum class SortColumn { Name, Category, Quality, Price, Quantity };

class ShipMenuUI {
private:
    sf::RectangleShape background;
    sf::RectangleShape tableBackground;
    sf::Sprite shipPreview;
    sf::Text titleText;
    sf::Text escText;
    
    // Botón de mejora
    sf::RectangleShape upgradeBtn;
    sf::Text upgradeText;

    // Textos de las cabeceras de la tabla
    sf::Text headerName, headerCategory, headerQuality, headerPrice;
    
    // --- SCROLL ---
    int startIndex = 0; // Índice del primer ítem a dibujar
    int maxVisibleItems = 10; // Cuántos ítems caben en la pantalla a la vez
    sf::RectangleShape scrollTrack; // El fondo de la barra de scroll
    sf::RectangleShape scrollThumb; // La barrita que se mueve
    // ------------------------

    SortColumn currentSort = SortColumn::Name;
    bool sortAscending = true;
    
    const sf::Font& font; // Referencia a la fuente global

public:
    ShipMenuUI(const sf::Font& f, const sf::Texture& shipTex);
    
    // Recibe el inventario y el catálogo para armar la tabla
    void draw(sf::RenderWindow& window, const Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Maneja clics (para ordenar cabeceras o botón de mejorar)
    void handleInput(const sf::Event& event, const sf::Vector2f& mousePos, int totalItems, Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    
    // Para hover de botones
    void update(const sf::Vector2f& mousePos);
};