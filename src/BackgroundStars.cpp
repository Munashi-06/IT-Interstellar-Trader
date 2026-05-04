#include "BackgroundStars.hpp"
#include <cstdlib>

BackgroundStars::BackgroundStars() {
    starsFar = sf::VertexArray(sf::PrimitiveType::Points, 800);
    for (int i = 0; i < 800; i++) {
        starsFar[i].position = {(float)(rand()%2000), (float)(rand()%2000)};
        int c = 60 + (rand()% 40);
        starsFar[i].color = sf::Color(c, c, c);
    }

    starsMid = sf::VertexArray(sf::PrimitiveType::Points, 400);
    for (int i = 0; i < 400; i++) {
        starsMid[i].position = {(float)(rand()%2000), (float)(rand()%2000)};
        int c = 120 + (rand()% 60);
        starsMid[i].color = sf::Color(c, c, c);
    }

    starsNear = sf::VertexArray(sf::PrimitiveType::Points, 200);
    for (int i = 0; i < 200; i++) {
        starsNear[i].position = {(float)(rand()%2000), (float)(rand()%2000)};
        int c = 200 + (rand()% 55);
        starsNear[i].color = sf::Color(c, c, c);
    }
}

void BackgroundStars::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
    sf::RenderStates states;

    sf::Transform tFar; tFar.translate(-playerPos * 0.2f);
    states.transform = tFar; 
    window.draw(starsFar, states);

    sf::Transform tMid; tMid.translate(-playerPos * 0.4f);
    states.transform = tMid; 
    window.draw(starsMid, states);

    sf::Transform tNear; tNear.translate(-playerPos * 0.7f);
    states.transform = tNear; 
    window.draw(starsNear, states);
}