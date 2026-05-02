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

    Button(const std::string& label, sf::Vector2f size, sf::Vector2f pos, sf::Font& font);

    //esto es para alinear las opciones
    void setAlignmentLeft(float margin = 15.f);

    void draw(sf::RenderWindow& window);
};

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    bool updateHover(sf::Vector2f mousePos);
    std::string getSelectedOption() const;

private:
    int selectedItemIndex = 0;
    sf::Font font;
    sf::Text title;
    std::vector<Button> options;
};