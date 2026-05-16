#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Entities/Player.hpp"
#include "Systems/UpgradeManager.hpp"
#include "Interface/GameOverScene.hpp" // Añadido para tener acceso a Game::DeathReason

namespace Interface {
    enum class PirateMenu { Main, Bribery, Result };
    
    class PirateEncounter {
    public:
        PirateEncounter();
        
        bool loadAssets();
        void draw(sf::RenderWindow& window, sf::Font& font);
        void update(float dt);
        void handleInput(sf::Keyboard::Key k);
        void handleMouseMove(const sf::Vector2f& mousePos);
        
        // Firmas limpias (sin parámetros pasados por referencia extraños)
        bool handleMouseClick(const sf::Vector2f& mousePos, Player& player, UpgradeManager& upgrades);
        void handleEncounterLogic(sf::Keyboard::Key key, Player& player, UpgradeManager& upgrades);
        
        bool rollForEncounter(float chance);
        void setResult(const std::string& message);
        void reset();
        void stop();
        void setShowButtons(bool s);
        bool isShowingButtons() const;
        void setMenu(PirateMenu menu);
        PirateMenu getCurrentMenu() const;
        int getSelectedButton() const;
        bool isActive() const;

        // Getters para que el Engine sepa por qué murió el jugador
        bool isGameOverPending() const { return pendingGameOver; }
        Game::DeathReason getPendingReason() const { return pendingReason; }
        
    private:
        bool active;
        float displayTimer;
        float baseScale;
        bool showButtons;
        PirateMenu currentMenu;
        int selectedButton;
        std::vector<std::string> mainOptions;
        std::vector<std::string> briberyOptions;
        std::string resultMessage;
        
        sf::Texture pirateTex;
        std::unique_ptr<sf::Sprite> pirateSprite;

        // Variables de estado interno para el Game Over
        bool pendingGameOver = false;
        Game::DeathReason pendingReason = Game::DeathReason::NoMoneyNoItems;

        void executeSelection(Player& player, UpgradeManager& upgrades);
    };
}