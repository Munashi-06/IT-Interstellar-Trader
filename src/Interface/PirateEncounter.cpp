#include "Interface/PirateEncounter.hpp"
#include <iostream>
#include <cmath>

namespace Interface {
    PirateEncounter::PirateEncounter() 
        : active(false), displayTimer(0.f), baseScale(1.0f), 
          showButtons(false), currentMenu(PirateMenu::Main), selectedButton(0) 
    {
        std::random_device rd;
        rng.seed(rd());
        mainOptions = {"Defenderse", "Sobornar", "Rendirse"};
        briberyOptions = {"Dar el 80% (Seguro)", "Dar el 40% (Arriesgado)", "Volver"};
    }

    bool PirateEncounter::loadAssets() {
        if (!pirateTex.loadFromFile("assets/anim03.png")) {
            std::cerr << "[ERROR] No se pudo cargar assets/anim03.png" << std::endl;
            return false;
        }
        
        pirateSprite = std::make_unique<sf::Sprite>(pirateTex);
        sf::Vector2u texSize = pirateTex.getSize();
        pirateSprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
        pirateSprite->setPosition({640.f, 300.f}); 
        
        float scale = 1.0f;
        if (texSize.x > 600) scale = 600.0f / texSize.x;
        if (texSize.y > 400) scale = std::min(scale, 400.0f / texSize.y);
        
        baseScale = scale; 
        pirateSprite->setScale({baseScale, baseScale});
        return true;
    }

    bool PirateEncounter::rollForEncounter(float chance) {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        if (dist(rng) < chance) {
            active = true; // ACTIVAMOS el objeto para que update/draw funcionen
            reset();       // Preparamos menús
            return true;
        }
        return false;
    }

    void PirateEncounter::update(float dt) {
        if (!active) return;
        displayTimer += dt;
        // Animación de pulso
        float pulse = 1.0f + 0.05f * std::sin(displayTimer * 5.0f);
        if (pirateSprite) pirateSprite->setScale({baseScale * pulse, baseScale * pulse});
    }

    void PirateEncounter::handleInput(sf::Keyboard::Key key) {
        // Navegación exclusiva con FLECHAS
        if (key == sf::Keyboard::Key::Up) {
            selectedButton = (selectedButton - 1 + 3) % 3;
        } 
        else if (key == sf::Keyboard::Key::Down) {
            selectedButton = (selectedButton + 1) % 3;
        }
    }

    void PirateEncounter::draw(sf::RenderWindow& window, sf::Font& font) {
        if (!active || !pirateSprite) return;

        window.draw(*pirateSprite);

        if (showButtons) {
            const auto& currentOpts = (currentMenu == PirateMenu::Main) ? mainOptions : briberyOptions;

            for (int i = 0; i < (int)currentOpts.size(); ++i) {
                sf::RectangleShape button({380.f, 45.f});
                button.setOrigin({190.f, 22.5f});
                button.setPosition({640.f, 450.f + (i * 65.f)});

                if (i == selectedButton) {
                    button.setFillColor(sf::Color(150, 0, 0, 230)); // Rojo selección
                    button.setOutlineColor(sf::Color::Red);
                } else {
                    button.setFillColor(sf::Color(20, 20, 20, 230));
                    button.setOutlineColor(sf::Color::White);
                }
                button.setOutlineThickness(2);
                
                sf::Text btnText(font, currentOpts[i]);
                btnText.setCharacterSize(18);
                sf::FloatRect b = btnText.getLocalBounds();
                
                // Corrección para SFML 3.0 (usando b.position.y)
                btnText.setOrigin({b.size.x / 2.f, b.size.y / 2.f + b.position.y / 2.f});
                btnText.setPosition(button.getPosition());

                window.draw(button);
                window.draw(btnText);
            }
        }
    }

    void PirateEncounter::reset() {
        showButtons = false;
        currentMenu = PirateMenu::Main;
        selectedButton = 0;
        displayTimer = 0.f;
    }

    void PirateEncounter::stop() {
        active = false; // Apaga update y draw
    }

    void PirateEncounter::setShowButtons(bool show) { showButtons = show; }
    bool PirateEncounter::isShowingButtons() const { return showButtons; }
}