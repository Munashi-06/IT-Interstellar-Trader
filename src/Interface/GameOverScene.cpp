#include "Interface/GameOverScene.hpp"
#include "Entities/Item.hpp"
#include <iostream>
#include <cmath>
#include <cstdint>

namespace Game {

    //=============================================================================
    // Constructor
    //=============================================================================
    GameOverScene::GameOverScene(const sf::Font& font)
        : anim09Texture(nullptr),
          anim09Sprite(nullptr),
          gameOverText(font, ""),
          subText(font, ""),
          continueText(font, ""),
          active(false),
          fadeAlpha(0.f),
          fadeSpeed(200.f),
          baseScale(1.0f),
          deathReason(DeathReason::NoMoneyNoItems)  // Default reason
    {
        std::cout << "[GAMEOVER] Constructor called" << std::endl;
        
        // Create texture
        anim09Texture = std::make_unique<sf::Texture>();
        
        // Load texture
        if (!anim09Texture->loadFromFile("assets/anim06.png")) {
            std::cerr << "[GAMEOVER] Error: Could not load assets/anim06.png" << std::endl;
            if (!anim09Texture->loadFromFile("assets/anim03.png")) {
                std::cerr << "[GAMEOVER] Error: Could not load anim03.png either" << std::endl;
            } else {
                std::cout << "[GAMEOVER] Using anim03.png as fallback" << std::endl;
            }
        } else {
            std::cout << "[GAMEOVER] anim06.png loaded successfully" << std::endl;
        }
        
        // Create sprite with the texture
        anim09Sprite = std::make_unique<sf::Sprite>(*anim09Texture);
        
        // Configure origin and position
        sf::Vector2u texSize = anim09Texture->getSize();
        if (texSize.x > 0 && texSize.y > 0) {
            anim09Sprite->setOrigin(sf::Vector2f(static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f));
            anim09Sprite->setPosition(sf::Vector2f(640.f, 360.f));
            
            baseScale = (texSize.x > 600) ? 600.0f / static_cast<float>(texSize.x) : 1.0f;
            anim09Sprite->setScale(sf::Vector2f(baseScale, baseScale));
        }

        // Dark overlay
        overlay.setSize(sf::Vector2f(1280.f, 720.f));
        overlay.setFillColor(sf::Color(0, 0, 0, 0));

        // GAME OVER text
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(80);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineColor(sf::Color::Black);
        gameOverText.setOutlineThickness(3.f);
        
        sf::FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(sf::Vector2f(textBounds.size.x / 2.f, textBounds.size.y / 2.f));
        gameOverText.setPosition(sf::Vector2f(640.f, 180.f));

        // Status text (will be updated based on death reason)
        subText.setCharacterSize(24);
        subText.setFillColor(sf::Color::White);
        
        sf::FloatRect subBounds = subText.getLocalBounds();
        subText.setOrigin(sf::Vector2f(subBounds.size.x / 2.f, subBounds.size.y / 2.f));
        subText.setPosition(sf::Vector2f(640.f, 500.f));

        // Continue text
        continueText.setString("Press ENTER to return to main menu");
        continueText.setCharacterSize(18);
        continueText.setFillColor(sf::Color::Yellow);
        
        sf::FloatRect contBounds = continueText.getLocalBounds();
        continueText.setOrigin(sf::Vector2f(contBounds.size.x / 2.f, contBounds.size.y / 2.f));
        continueText.setPosition(sf::Vector2f(640.f, 620.f));
        
        std::cout << "[GAMEOVER] Constructor completed" << std::endl;
    }

    //=============================================================================
    // Check if the player has lost the game
    //=============================================================================
    bool GameOverScene::checkCondition(Player& player,
        const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog)
    {
        // Check if player has money
        bool hasMoney = (player.getMoney() > 0);
        
        // Check if player has any items in inventory
        bool hasItems = (player.getInventory().getUsedSlots() > 0);
        
        // Check if player has any propulsion upgrades (treeType = 1)
        // This would require access to UpgradeManager
        // For now, we'll use a simple flag that can be set externally
        // bool hasPropulsionUpgrades = player.hasPropulsionUpgrades();
        
        if (!hasMoney && !hasItems) {
            // Death by poverty - no money and no items to sell
            return true;
        }
        
        return false;
    }

    //=============================================================================
    // Set the death reason to display appropriate message
    //=============================================================================
    void GameOverScene::setDeathReason(DeathReason reason) {
        deathReason = reason;
        
        // Update subtitle text based on death reason
        switch (deathReason) {
            case DeathReason::NoMoneyNoItems:
                subText.setString("You have run out of money and have no items left to sell.\nYour journey ends here...");
                break;
            case DeathReason::NoMoneyOnly:
                subText.setString("You have no money left and cannot afford basic supplies.\nYour journey ends here...");
                break;
            case DeathReason::EnginesDestroyed:
                subText.setString("The pirates have destroyed your engines!\nWithout propulsion, you are stranded forever...");
                break;
            case DeathReason::WarpDriveDestroyed:
                subText.setString("The pirates have destroyed your Warp Drive!\nYou can no longer return home...");
                break;
            case DeathReason::ShipDestroyed:
                subText.setString("Your ship has been completely destroyed.\nThere is no way to recover...");
                break;
            case DeathReason::CargoDestroyed:
                subText.setString("The pirates have destroyed all your cargo.\nWith nothing left to trade, your journey ends...");
                break;
            default:
                subText.setString("You have no money and no tradable items left.\nYour journey ends here...");
                break;
        }
        
        // Recenter the text after changing it
        sf::FloatRect subBounds = subText.getLocalBounds();
        subText.setOrigin(sf::Vector2f(subBounds.size.x / 2.f, subBounds.size.y / 2.f));
        subText.setPosition(sf::Vector2f(640.f, 500.f));
    }

    //=============================================================================
    // Handle input (ENTER to return to menu)
    //=============================================================================
    bool GameOverScene::handleInput(const sf::Event& event) {
        if (!active) return false;
        
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Enter) {
                active = false;
                std::cout << "[GAMEOVER] Enter pressed, deactivating" << std::endl;
                return true;
            }
        }
        return false;
    }

    //=============================================================================
    // Update animation (fade in and pulsing effect)
    //=============================================================================
    void GameOverScene::update(float dt) {
        if (!active) return;
        
        static float timer = 0.f;
        timer += dt;
        
        // Pulsing effect for the background sprite
        float pulse = 1.0f + 0.03f * std::sin(timer * 3.0f);
        if (anim09Sprite) {
            anim09Sprite->setScale(sf::Vector2f(baseScale * pulse, baseScale * pulse));
        }
        
        // Fade in effect for the overlay
        if (fadeAlpha < 200.f) {
            fadeAlpha += fadeSpeed * dt;
            if (fadeAlpha > 200.f) fadeAlpha = 200.f;
            overlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
        }
        
        // Blinking effect for continue text
        float alpha = 160 + std::sin(timer * 4.f) * 95;
        continueText.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
    }

    //=============================================================================
    // Draw all elements
    //=============================================================================
    void GameOverScene::draw(sf::RenderWindow& window) {
        if (!active) return;
        
        if (anim09Sprite) {
            window.draw(*anim09Sprite);
        }
        window.draw(overlay);
        window.draw(gameOverText);
        window.draw(subText);
        window.draw(continueText);
    }

    //=============================================================================
    // Reset the scene to initial state
    //=============================================================================
    void GameOverScene::reset() {
        fadeAlpha = 0.f;
        overlay.setFillColor(sf::Color(0, 0, 0, 0));
        if (anim09Sprite) {
            anim09Sprite->setScale(sf::Vector2f(baseScale, baseScale));
        }
    }

    //=============================================================================
    // Activate or deactivate the Game Over screen
    //=============================================================================
    void GameOverScene::setActive(bool a) {
        active = a;
        if (active) {
            reset();
            std::cout << "[GAMEOVER] Activated" << std::endl;
        }
    }

}