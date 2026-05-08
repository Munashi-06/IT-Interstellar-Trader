#pragma once
#include <SFML/Graphics.hpp>

class BackgroundStars {
private:
    sf::VertexArray starsFar;
    sf::VertexArray starsMid;
    sf::VertexArray starsNear;

public:
    BackgroundStars();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos);
};