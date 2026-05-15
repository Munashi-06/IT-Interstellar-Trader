#include "Interface/GameOverScene.hpp"
#include "Entities/Item.hpp"
#include <iostream>
#include <cmath>
#include <cstdint>

namespace Game {

    GameOverScene::GameOverScene(const sf::Font& font)
        : anim09Texture(nullptr),
          anim09Sprite(nullptr),
          gameOverText(font, ""),
          subText(font, ""),
          continueText(font, ""),
          active(false),
          fadeAlpha(0.f),
          fadeSpeed(200.f),
          baseScale(1.0f)
    {
        std::cout << "[GAMEOVER] Constructor llamado" << std::endl;
        
        // Crear textura
        anim09Texture = std::make_unique<sf::Texture>();
        
        // Cargar textura
        if (!anim09Texture->loadFromFile("assets/anim06.png")) {
            std::cerr << "[GAMEOVER] Error: No se pudo cargar assets/anim06.png" << std::endl;
            if (!anim09Texture->loadFromFile("assets/anim03.png")) {
                std::cerr << "[GAMEOVER] Error: Tampoco se pudo cargar anim03.png" << std::endl;
            } else {
                std::cout << "[GAMEOVER] Usando anim03.png como fallback" << std::endl;
            }
        } else {
            std::cout << "[GAMEOVER] anim09.png cargado correctamente" << std::endl;
        }
        
        // Crear sprite con la textura
        anim09Sprite = std::make_unique<sf::Sprite>(*anim09Texture);
        
        // Configurar origen y posición
        sf::Vector2u texSize = anim09Texture->getSize();
        if (texSize.x > 0 && texSize.y > 0) {
            anim09Sprite->setOrigin(sf::Vector2f(static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f));
            anim09Sprite->setPosition(sf::Vector2f(640.f, 360.f));
            
            baseScale = (texSize.x > 600) ? 600.0f / static_cast<float>(texSize.x) : 1.0f;
            anim09Sprite->setScale(sf::Vector2f(baseScale, baseScale));
        }

        // Overlay oscuro
        overlay.setSize(sf::Vector2f(1280.f, 720.f));
        overlay.setFillColor(sf::Color(0, 0, 0, 0));

        // Texto GAME OVER
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(80);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineColor(sf::Color::Black);
        gameOverText.setOutlineThickness(3.f);
        
        sf::FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(sf::Vector2f(textBounds.size.x / 2.f, textBounds.size.y / 2.f));
        gameOverText.setPosition(sf::Vector2f(640.f, 200.f));

        // Texto de estado
        subText.setString("You have no money and no tradable items left.");
        subText.setCharacterSize(24);
        subText.setFillColor(sf::Color::White);
        
        sf::FloatRect subBounds = subText.getLocalBounds();
        subText.setOrigin(sf::Vector2f(subBounds.size.x / 2.f, subBounds.size.y / 2.f));
        subText.setPosition(sf::Vector2f(640.f, 520.f));

        // Texto de continuar
        continueText.setString("Press ENTER to return to main menu");
        continueText.setCharacterSize(18);
        continueText.setFillColor(sf::Color::Yellow);
        
        sf::FloatRect contBounds = continueText.getLocalBounds();
        continueText.setOrigin(sf::Vector2f(contBounds.size.x / 2.f, contBounds.size.y / 2.f));
        continueText.setPosition(sf::Vector2f(640.f, 600.f));
        
        std::cout << "[GAMEOVER] Constructor completado" << std::endl;
    }

    bool GameOverScene::checkCondition(Player& player,
        const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog)
    {
        if (player.getMoney() > 0) return false;
        if (player.getInventory().getUsedSlots() > 0) return false;
        return true;
    }

    bool GameOverScene::handleInput(const sf::Event& event) {
        if (!active) return false;
        
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Enter) {
                active = false;
                std::cout << "[GAMEOVER] Enter presionado, desactivando" << std::endl;
                return true;
            }
        }
        return false;
    }

    void GameOverScene::update(float dt) {
        if (!active) return;
        
        static float timer = 0.f;
        timer += dt;
        float pulse = 1.0f + 0.03f * std::sin(timer * 3.0f);
        if (anim09Sprite) {
            anim09Sprite->setScale(sf::Vector2f(baseScale * pulse, baseScale * pulse));
        }
        
        if (fadeAlpha < 180.f) {
            fadeAlpha += fadeSpeed * dt;
            if (fadeAlpha > 180.f) fadeAlpha = 180.f;
            overlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
        }
        
        float alpha = 160 + std::sin(timer * 4.f) * 95;
        continueText.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
    }

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

    void GameOverScene::reset() {
        fadeAlpha = 0.f;
        overlay.setFillColor(sf::Color(0, 0, 0, 0));
        if (anim09Sprite) {
            anim09Sprite->setScale(sf::Vector2f(baseScale, baseScale));
        }
    }

    void GameOverScene::setActive(bool a) {
        active = a;
        if (active) {
            reset();
            std::cout << "[GAMEOVER] Activado" << std::endl;
        }
    }
}