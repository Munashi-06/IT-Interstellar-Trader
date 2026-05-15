#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include "../Entities/Player.hpp"

class Item;

namespace Game {
    class GameOverScene {
    public:
        GameOverScene(const sf::Font& font);
        ~GameOverScene() = default;

        static bool checkCondition(Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

        bool handleInput(const sf::Event& event);
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void reset();

        bool isActive() const { return active; }
        void setActive(bool a);

    private:
        std::unique_ptr<sf::Texture> anim09Texture;  // Cambiado a unique_ptr
        std::unique_ptr<sf::Sprite> anim09Sprite;
        sf::Text gameOverText;
        sf::Text subText;
        sf::Text continueText;
        sf::RectangleShape overlay;
        bool active;
        float fadeAlpha;
        float fadeSpeed;
        float baseScale;
    };
}