#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

static sf::Color selectedColor = sf::Color(100, 100, 100);
// List of colors for the buttons, you can choose the one you like best
/*
sf::Color(200, 200, 200); // Light Gray
sf::Color(255, 255, 255); // White
sf::Color(180, 180, 255); // Light Blue
sf::Color(255, 180, 180); // Light Red
sf::Color(180, 255, 180); // Light Green
*/
static sf::Color unselectedColor = sf::Color::White;

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    bool selected = false; // New variable to control the "box"

    Button(const std::string& label, sf::Vector2f size, sf::Vector2f pos, sf::Font& font);

    //This is to align the options
    void setAlignmentLeft(float margin);

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
    std::string getClickedOption(const sf::Vector2f& mousePos) const;
private:
    int selectedItemIndex = 0;
    sf::Font font;
    sf::Text title;
    std::vector<Button> options;
};