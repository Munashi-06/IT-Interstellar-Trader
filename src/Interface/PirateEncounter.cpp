#include "Interface/PirateEncounter.hpp"
#include <cmath>
#include <iostream>
#include <cstdlib>

namespace Interface {
    PirateEncounter::PirateEncounter() 
        : active(false), displayTimer(0.f), baseScale(1.0f), 
          showButtons(false), currentMenu(PirateMenu::Main), selectedButton(0) 
    {
        mainOptions = {"Defend", "Bribe", "Surrender"};
        briberyOptions = {"Give 80% (Safe)", "Give 40% (Risky)", "Back"};
    }

    bool PirateEncounter::loadAssets() {
        if (!pirateTex.loadFromFile("assets/anim03.png")) {
            std::cerr << "[PIRATE] Error loading pirate texture" << std::endl;
            return false;
        }
        pirateSprite = std::make_unique<sf::Sprite>(pirateTex);
        sf::Vector2u texSize = pirateTex.getSize();
        pirateSprite->setOrigin(sf::Vector2f(static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f));
        pirateSprite->setPosition(sf::Vector2f(640.f, 300.f)); 
        baseScale = (texSize.x > 600) ? 600.0f / static_cast<float>(texSize.x) : 1.0f;
        pirateSprite->setScale(sf::Vector2f(baseScale, baseScale));
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
            resText.setOrigin(sf::Vector2f(b.size.x / 2.f, b.size.y / 2.f));
            resText.setPosition(sf::Vector2f(640.f, 480.f));
            window.draw(resText);
            
            sf::Text sub(font, "Press ENTER to continue...");
            sub.setCharacterSize(14);
            sub.setFillColor(sf::Color::White);
            sf::FloatRect sb = sub.getLocalBounds();
            sub.setOrigin(sf::Vector2f(sb.size.x / 2.f, sb.size.y / 2.f));
            sub.setPosition(sf::Vector2f(640.f, 530.f));
            window.draw(sub);
        } else if (showButtons) {
            const auto& opts = (currentMenu == PirateMenu::Main) ? mainOptions : briberyOptions;
            for (int i = 0; i < static_cast<int>(opts.size()); ++i) {
                sf::RectangleShape btn(sf::Vector2f(380.f, 45.f));
                btn.setOrigin(sf::Vector2f(190.f, 22.5f));
                btn.setPosition(sf::Vector2f(640.f, 420.f + (i * 65.f)));
                btn.setFillColor(i == selectedButton ? sf::Color(150,0,0,230) : sf::Color(20,20,20,230));
                btn.setOutlineThickness(2);
                btn.setOutlineColor(i == selectedButton ? sf::Color::Red : sf::Color::White);
                
                sf::Text txt(font, opts[i]);
                txt.setCharacterSize(18);
                txt.setFillColor(sf::Color::White);
                sf::FloatRect tb = txt.getLocalBounds();
                txt.setOrigin(sf::Vector2f(tb.size.x/2.f, tb.size.y/2.f));
                txt.setPosition(btn.getPosition());
                window.draw(btn);
                window.draw(txt);
            }
        }
    }

    bool PirateEncounter::rollForEncounter(float chance) {
        float roll = static_cast<float>(rand() % 100) / 100.0f; 
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
        if (pirateSprite) {
            pirateSprite->setScale(sf::Vector2f(baseScale * pulse, baseScale * pulse));
        }
    }
    
    void PirateEncounter::handleInput(sf::Keyboard::Key k) {
        if (currentMenu == PirateMenu::Result) return;
        if (k == sf::Keyboard::Key::Up) selectedButton = (selectedButton - 1 + 3) % 3;
        if (k == sf::Keyboard::Key::Down) selectedButton = (selectedButton + 1) % 3;
    }
    
    void PirateEncounter::reset() { 
        showButtons = false; 
        currentMenu = PirateMenu::Main; 
        selectedButton = 0; 
        resultMessage = ""; 
    }
    
    void PirateEncounter::stop() { active = false; }
    void PirateEncounter::setShowButtons(bool s) { showButtons = s; }
    bool PirateEncounter::isShowingButtons() const { return showButtons; }
    void PirateEncounter::setMenu(PirateMenu menu) { currentMenu = menu; }
    PirateMenu PirateEncounter::getCurrentMenu() const { return currentMenu; }
    int PirateEncounter::getSelectedButton() const { return selectedButton; }
    bool PirateEncounter::isActive() const { return active; }

    void PirateEncounter::handleEncounterLogic(sf::Keyboard::Key key, Player& player, bool& gameOverTriggered) {
        if (!active) return;
        
        if (currentMenu == PirateMenu::Result) {
            if (key == sf::Keyboard::Key::Enter) {
                stop();
            }
        } 
        else if (showButtons) {
            handleInput(key);
            
            if (key == sf::Keyboard::Key::Enter) {
                int sel = selectedButton;
                int roll = rand() % 100;
                int playerLevel = player.getLevel(); 

                if (currentMenu == PirateMenu::Main) {
                    if (sel == 0) {  // Defend
                        int winChance = 40 + (playerLevel * 12);
                        if (roll < winChance) {
                            setResult("VICTORY: You defended your ship!");
                        } else {
                            // DERROTA - Solo perder dinero, NO el inventario
                            player.setMoney(0.0f);
                            setResult("DEFEAT: They looted all your credits!");
                        }
                    }
                    else if (sel == 1) {  // Bribe
                        currentMenu = PirateMenu::Bribery;
                        selectedButton = 0;
                        showButtons = true;
                    }
                    else if (sel == 2) {  // Surrender
                        player.getInventory().clearAll();
                        setResult("SURRENDER: They took all your cargo.");
                        if (player.getMoney() <= 0) {
                            gameOverTriggered = true;
                        }
                    }
                } 
                else if (currentMenu == PirateMenu::Bribery) {
                    float currentMoney = player.getMoney();
                    if (sel == 0) {  // 80% bribe
                        player.setMoney(currentMoney * 0.2f);
                        if (roll < 5) {
                            player.setMoney(0.0f);
                            setResult("BETRAYAL: They took all your money!");
                        } else {
                            setResult("PAID: You are free to go.");
                        }
                    }
                    else if (sel == 1) {  // 40% bribe
                        player.setMoney(currentMoney * 0.6f);
                        if (roll < 45) {
                            player.setMoney(0.0f);
                            setResult("REJECTED: They took all your money!");
                        } else {
                            setResult("LUCKY: They accepted the minimum deal.");
                        }
                    }
                    else {  // Back
                        currentMenu = PirateMenu::Main;
                        selectedButton = 0;
                        showButtons = true;
                    }
                }
            }
        } 
        else if (key == sf::Keyboard::Key::Enter) {
            showButtons = true;
            selectedButton = 0;
        }
    }
}