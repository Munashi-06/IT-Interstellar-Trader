#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

static sf::Color selectedColor = sf::Color(100, 100, 100);
// lista de colores para los botones, puedes elegir el que más te guste
/*
sf::Color(200, 200, 200); // Gris claro
sf::Color(255, 255, 255); // Blanco
sf::Color(180, 180, 255); // Azul claro
sf::Color(255, 180, 180); // Rojo claro
sf::Color(180, 255, 180); // Verde claro
*/
static sf::Color unselectedColor = sf::Color::White;

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
        shape.setOutlineColor(selectedColor);
        shape.setOutlineThickness(2);

        text.setFillColor(unselectedColor);
        
        // Centrar texto en la caja
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f + 12.f});
        text.setPosition({pos.x + size.x / 2.f, pos.y + size.y / 2.f});
    }

    void draw(sf::RenderWindow& window) {
        if (selected) {
            window.draw(shape); // Solo dibujamos la caja si está seleccionado
            text.setFillColor(selectedColor);
        }
        else {
            text.setFillColor(unselectedColor);
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
    sf::Text title;
    std::vector<Button> options;
};