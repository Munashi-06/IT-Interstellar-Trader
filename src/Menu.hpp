#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    bool selected = false; // Nueva variable para controlar la "cajita"

    Button(const std::string& label, sf::Vector2f size, sf::Vector2f pos, sf::Font& font) 
        : text(font, label, 35) // Corregimos C2512 aquí
    {
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::Transparent); // Invisible por defecto
        shape.setOutlineColor(sf::Color::Cyan);
        shape.setOutlineThickness(2);

        text.setFillColor(sf::Color::White);
        
        // Centrar texto en la caja
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f + 12.f});
        text.setPosition({pos.x + size.x / 2.f, pos.y + size.y / 2.f});
    }

    void draw(sf::RenderWindow& window) {
        if (selected) {
            window.draw(shape); // Solo dibujamos la caja si está seleccionado
            text.setFillColor(sf::Color::Cyan);
        } else {
            text.setFillColor(sf::Color::White);
        }
        window.draw(text);
    }
};

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    void updateHover(sf::Vector2f mousePos);
    std::string getSelectedOption() const;

private:
    int selectedItemIndex = 0;
    sf::Font font;
    std::vector<Button> options;
};