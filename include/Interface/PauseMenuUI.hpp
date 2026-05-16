#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseMenuUI {
public:
    PauseMenuUI(const sf::Font& font);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    std::string getSelectedOption() const;
    bool updateHover(const sf::Vector2f& mousePos);
    void handleMouseClick(const sf::Vector2f& mousePos);

private:
    std::vector<sf::Text> menuTexts;
    std::vector<sf::RectangleShape> buttonBgs;
    int selectedItemIndex;
    sf::RectangleShape overlay;
    sf::Text titleText;
};