#include "Interface/PirateEncounter.hpp"
#include <cmath>

namespace Interface {
    PirateEncounter::PirateEncounter() 
        : active(false), displayTimer(0.f), baseScale(1.0f), 
          showButtons(false), currentMenu(PirateMenu::Main), selectedButton(0) 
    {
        mainOptions = {"Defenderse", "Sobornar", "Rendirse"};
        briberyOptions = {"Dar el 80% (Seguro)", "Dar el 40% (Arriesgado)", "Volver"};
    }

    bool PirateEncounter::loadAssets() {
        if (!pirateTex.loadFromFile("assets/anim03.png")) return false;
        pirateSprite = std::make_unique<sf::Sprite>(pirateTex);
        sf::Vector2u texSize = pirateTex.getSize();
        pirateSprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
        pirateSprite->setPosition({640.f, 300.f}); 
        baseScale = (texSize.x > 600) ? 600.0f / texSize.x : 1.0f;
        pirateSprite->setScale({baseScale, baseScale});
        return true;
    }

    void PirateEncounter::setResult(const std::string& message) {
        resultMessage = message;
        currentMenu = PirateMenu::Result;
        showButtons = false; 
    }

    void PirateEncounter::draw(sf::RenderWindow& window, sf::Font& font) {
        if (!active || !pirateSprite) return;
        window.draw(*pirateSprite);

        if (currentMenu == PirateMenu::Result) {
            sf::Text resText(font, resultMessage);
            resText.setCharacterSize(22);
            resText.setFillColor(sf::Color::Yellow);
            resText.setOutlineColor(sf::Color::Black);
            resText.setOutlineThickness(2);
            sf::FloatRect b = resText.getLocalBounds();
            resText.setOrigin({b.size.x / 2.f, b.size.y / 2.f + b.position.y / 2.f});
            resText.setPosition({640.f, 480.f});
            window.draw(resText);
            
            sf::Text sub(font, "Presiona ENTER para continuar...");
            sub.setCharacterSize(14);
            sub.setPosition({640.f, 530.f});
            sf::FloatRect sb = sub.getLocalBounds();
            sub.setOrigin({sb.size.x / 2.f, sb.size.y / 2.f + sb.position.y / 2.f});
            window.draw(sub);
        } else if (showButtons) {
            const auto& opts = (currentMenu == PirateMenu::Main) ? mainOptions : briberyOptions;
            for (int i = 0; i < (int)opts.size(); ++i) {
                sf::RectangleShape btn({380.f, 45.f});
                btn.setOrigin({190.f, 22.5f});
                btn.setPosition({640.f, 450.f + (i * 65.f)});
                btn.setFillColor(i == selectedButton ? sf::Color(150,0,0,230) : sf::Color(20,20,20,230));
                btn.setOutlineThickness(2);
                btn.setOutlineColor(i == selectedButton ? sf::Color::Red : sf::Color::White);
                
                sf::Text txt(font, opts[i]);
                txt.setCharacterSize(18);
                sf::FloatRect tb = txt.getLocalBounds();
                txt.setOrigin({tb.size.x/2.f, tb.size.y/2.f + tb.position.y/2.f});
                txt.setPosition(btn.getPosition());
                window.draw(btn);
                window.draw(txt);
            }
        }
    }

    bool PirateEncounter::rollForEncounter(float chance) {
    // Convertimos el 'chance' (ej. 0.3f) a un porcentaje (30)
        float roll = (rand() % 100) / 100.0f; 
        if (roll < chance) { 
            active = true; 
            reset(); 
            return true; 
        }
        return false;
    }
    void PirateEncounter::update(float dt) {
        if (!active) return;
        displayTimer += dt;
        float pulse = 1.0f + 0.05f * std::sin(displayTimer * 5.0f);
        pirateSprite->setScale({baseScale * pulse, baseScale * pulse});
    }
    void PirateEncounter::handleInput(sf::Keyboard::Key k) {
        if (currentMenu == PirateMenu::Result) return;
        if (k == sf::Keyboard::Key::Up) selectedButton = (selectedButton - 1 + 3) % 3;
        if (k == sf::Keyboard::Key::Down) selectedButton = (selectedButton + 1) % 3;
    }
    void PirateEncounter::reset() { showButtons = false; currentMenu = PirateMenu::Main; selectedButton = 0; resultMessage = ""; }
    void PirateEncounter::stop() { active = false; }
    void PirateEncounter::setShowButtons(bool s) { showButtons = s; }
    bool PirateEncounter::isShowingButtons() const { return showButtons; }
}