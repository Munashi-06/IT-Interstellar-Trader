#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "World.hpp"
#include "Heap.hpp"

class RadarUI {
private:
    sf::Font font;
    std::unique_ptr<sf::Text> title;
    sf::RectangleShape background;
    // Vector para que guarde punteros únicos
    // Esto evita CUALQUIER intento de copia prohibida por SFML 3
    std::vector<std::unique_ptr<sf::Text>> planetEntries;

public:
    RadarUI(const sf::Font& sharedFont) : font(sharedFont) {
        background.setSize({280.f, 190.f});
        background.setFillColor(sf::Color(0, 0, 0, 180));
        background.setOutlineThickness(2);
        background.setOutlineColor(sf::Color::Cyan);
        background.setPosition({50.f, 50.f});

        // CORRECCIÓN: Pasamos los argumentos directamente a make_unique
        title = std::make_unique<sf::Text>(font, "RADAR DE PRIORIDAD", 20);
        title->setFillColor(sf::Color::Cyan);
        title->setPosition({60.f, 60.f});
    }

    void update(const std::vector<Planet>& heapArray) {
        planetEntries.clear();
        
        for (size_t i = 1; i < heapArray.size() && i <= 5; ++i) { 
            std::string info = std::to_string(i) + ". " + heapArray[i].getName();
            if (heapArray[i].getEvent() != PlanetEvent::None) {
                info += " (!)"; 
            }

            // Creamos el texto directamente como un puntero único dentro del vector
            auto entry = std::make_unique<sf::Text>(font, info, 16);
            
            entry->setPosition({65.f, 70.f + (i * 30.f)});
            
            if (heapArray[i].getEvent() != PlanetEvent::None) {
                entry->setFillColor(sf::Color::Yellow);
            } else {
                entry->setFillColor(sf::Color::White);
            }

            planetEntries.push_back(std::move(entry));
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        if (title) window.draw(*title);
        
        for (auto& entry : planetEntries) {
            if (entry) window.draw(*entry);
        }
    }
};