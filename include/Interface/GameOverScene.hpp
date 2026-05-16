#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include "../Entities/Player.hpp"

class Item;

namespace Game {
    
    //=============================================================================
    // Different reasons for game over
    //=============================================================================
    enum class DeathReason {
        NoMoneyNoItems,      // No money and no items in inventory
        NoMoneyOnly,         // No money but has items (can't sell? edge case)
        EnginesDestroyed,    // Pirates destroyed propulsion upgrades
        WarpDriveDestroyed,  // Warp drive was destroyed
        ShipDestroyed,       // Complete ship destruction
        CargoDestroyed       // All cargo was destroyed/looted
    };

    class GameOverScene {
    public:
        GameOverScene(const sf::Font& font);
        ~GameOverScene() = default;

        // Check if the player has lost the game
        static bool checkCondition(Player& player, 
            const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

        // Set the reason for game over (to display appropriate message)
        void setDeathReason(DeathReason reason);
        
        bool handleInput(const sf::Event& event);
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void reset();

        bool isActive() const { return active; }
        void setActive(bool a);

    private:
        std::unique_ptr<sf::Texture> anim09Texture;
        std::unique_ptr<sf::Sprite> anim09Sprite;
        sf::Text gameOverText;
        sf::Text subText;
        sf::Text continueText;
        sf::RectangleShape overlay;
        bool active;
        float fadeAlpha;
        float fadeSpeed;
        float baseScale;
        DeathReason deathReason;
    };
}