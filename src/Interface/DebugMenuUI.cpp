#include "Interface/DebugMenuUI.hpp"
#include <iostream>

DebugMenuUI::DebugMenuUI(const sf::Font& f) :
    font(f),
    winBtnText(font, ""),
    giveItemText(font, ""),
    moneyBtnText(font, ""),
    nextItemText(font, ""),
    prevItemText(font, ""),
    currentItemDisplay(font, ""),
    toggleBtnText(font, "")
    {
    // --- BOTÓN TOGGLE (Esquina superior derecha) ---
    toggleBtnBg.setSize({80.f, 30.f});
    toggleBtnBg.setFillColor(sf::Color(200, 0, 0, 180)); // Rojo alerta
    toggleBtnBg.setPosition({1180.f, 10.f});
    
    toggleBtnText.setString("DEBUG");
    toggleBtnText.setCharacterSize(14);
    toggleBtnText.setFillColor(sf::Color::White);
    centerText(toggleBtnText, toggleBtnBg);

    // --- PANEL DESPLEGABLE ---
    panelBg.setSize({250.f, 200.f});
    panelBg.setFillColor(sf::Color(20, 20, 20, 240));
    panelBg.setOutlineThickness(2.f);
    panelBg.setOutlineColor(sf::Color::Red);
    panelBg.setPosition({1010.f, 50.f});

    // --- BOTÓN DINERO ---
    moneyBtnBg.setSize({210.f, 30.f});
    moneyBtnBg.setFillColor(sf::Color(0, 150, 0));
    moneyBtnBg.setPosition({1030.f, 70.f});
    moneyBtnText.setFont(font); moneyBtnText.setString("+ 10,000 Bs"); moneyBtnText.setCharacterSize(14);
    centerText(moneyBtnText, moneyBtnBg);

    // --- BOTÓN VICTORIA ---
    winBtnBg.setSize({210.f, 30.f});
    winBtnBg.setFillColor(sf::Color(150, 0, 150));
    winBtnBg.setPosition({1030.f, 110.f});
    winBtnText.setFont(font); winBtnText.setString("TRIGGER ENDING"); winBtnText.setCharacterSize(14);
    centerText(winBtnText, winBtnBg);

    // --- SELECTOR DE ITEMS ---
    float itemY = 160.f;
    prevItemBtn.setSize({30.f, 30.f});
    prevItemBtn.setFillColor(sf::Color(80, 80, 80));
    prevItemBtn.setPosition({1030.f, itemY});
    prevItemText.setString("<");
    prevItemText.setCharacterSize(14);
    centerText(prevItemText, prevItemBtn);

    nextItemBtn.setSize({30.f, 30.f});
    nextItemBtn.setFillColor(sf::Color(80, 80, 80));
    nextItemBtn.setPosition({1210.f, itemY});
    nextItemText.setString(">");
    nextItemText.setCharacterSize(14);
    centerText(nextItemText, nextItemBtn);

    giveItemBtn.setSize({130.f, 30.f});
    giveItemBtn.setFillColor(sf::Color(0, 100, 200));
    giveItemBtn.setPosition({1070.f, itemY});
    giveItemText.setString("GIVE ITEM");
    giveItemText.setCharacterSize(14);
    centerText(giveItemText, giveItemBtn);

    currentItemDisplay.setString("Loading...");
    currentItemDisplay.setCharacterSize(12);
    currentItemDisplay.setFillColor(sf::Color::Yellow);
    currentItemDisplay.setPosition({1030.f, 200.f});
}

void DebugMenuUI::initCatalog(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    itemIDs.clear();
    for (const auto& pair : catalog) {
        itemIDs.push_back(pair.first);
    }
    if (!itemIDs.empty()) {
        currentItemDisplay.setString("ID: " + itemIDs[0]);
    }
}

void DebugMenuUI::centerText(sf::Text& text, const sf::RectangleShape& rect) {
    sf::FloatRect bounds = text.getLocalBounds();
    // text.setOrigin({bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f});
    text.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    text.setPosition({rect.getPosition().x + rect.getSize().x / 2.0f, rect.getPosition().y + rect.getSize().y / 2.0f});
}

void DebugMenuUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Player& player, Inventory& playerInv) {
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            // Abrir/Cerrar menú
            if (toggleBtnBg.getGlobalBounds().contains(mousePos)) {
                isExpanded = !isExpanded;
                return;
            }

            if (isExpanded) {
                // Dinero
                if (moneyBtnBg.getGlobalBounds().contains(mousePos)) {
                    player.setMoney(player.getMoney() + 10000.f);
                    std::cout << "[DEBUG] +10,000 Bs added.\n";
                }
                // Victoria
                else if (winBtnBg.getGlobalBounds().contains(mousePos)) {
                    std::cout << "[DEBUG] TRIGGERING WARP DRIVE VICTORY!\n";
                    // Aquí puedes cambiar el estado a State::VictoryScene o llamar a tu lógica de fin de juego
                }
                // Navegar Ítems
                else if (prevItemBtn.getGlobalBounds().contains(mousePos)) {
                    if (!itemIDs.empty()) {
                        currentItemIndex = (currentItemIndex - 1 + itemIDs.size()) % itemIDs.size();
                        currentItemDisplay.setString("ID: " + itemIDs[currentItemIndex]);
                    }
                }
                else if (nextItemBtn.getGlobalBounds().contains(mousePos)) {
                    if (!itemIDs.empty()) {
                        currentItemIndex = (currentItemIndex + 1) % itemIDs.size();
                        currentItemDisplay.setString("ID: " + itemIDs[currentItemIndex]);
                    }
                }
                // Dar Ítem
                else if (giveItemBtn.getGlobalBounds().contains(mousePos)) {
                    if (!itemIDs.empty()) {
                        std::string id = itemIDs[currentItemIndex];
                        // Asumimos maxStack 99 y precio 0 para el debug
                        if(playerInv.addItem(id, 10, 99, 0.0f)) {
                            std::cout << "[DEBUG] 10x " << id << " added to inventory.\n";
                        } else {
                            std::cout << "[DEBUG] Inventory full!\n";
                        }
                    }
                }
            }
        }
    }
}

void DebugMenuUI::update(const sf::Vector2f& mousePos) {
    // Aquí puedes añadir efectos de hover (cambiar color si el mouse está encima) como hicimos en TradeMenuUI
}

void DebugMenuUI::draw(sf::RenderWindow& window) {
    window.draw(toggleBtnBg);
    window.draw(toggleBtnText);

    if (isExpanded) {
        window.draw(panelBg);
        window.draw(moneyBtnBg);  window.draw(moneyBtnText);
        window.draw(winBtnBg);    window.draw(winBtnText);
        
        window.draw(prevItemBtn); window.draw(prevItemText);
        window.draw(nextItemBtn); window.draw(nextItemText);
        window.draw(giveItemBtn); window.draw(giveItemText);
        
        window.draw(currentItemDisplay);
    }
}