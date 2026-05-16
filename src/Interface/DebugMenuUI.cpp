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
    toggleBtnText(font, ""),
    isUnlocked(false) // Starts completely hidden
    {
    // --- TOGGLE BUTTON (Bottom right corner) ---
    toggleBtnBg.setSize({80.f, 30.f});
    toggleBtnBg.setFillColor(sf::Color(200, 0, 0, 180));
    toggleBtnBg.setPosition({1180.f, 680.f});
    
    toggleBtnText.setString("DEBUG");
    toggleBtnText.setCharacterSize(14);
    toggleBtnText.setFillColor(sf::Color::White);
    centerText(toggleBtnText, toggleBtnBg);

    // --- DROPDOWN PANEL ---
    panelBg.setSize({250.f, 200.f});
    panelBg.setFillColor(sf::Color(20, 20, 20, 240));
    panelBg.setOutlineThickness(2.f);
    panelBg.setOutlineColor(sf::Color::Red);
    panelBg.setPosition({1010.f, 470.f});

    // --- MONEY BUTTON ---
    moneyBtnBg.setSize({210.f, 30.f});
    moneyBtnBg.setFillColor(sf::Color(0, 150, 0));
    moneyBtnBg.setPosition({1030.f, 490.f});
    moneyBtnText.setFont(font); moneyBtnText.setString("+ 10,000 Bs"); moneyBtnText.setCharacterSize(14);
    centerText(moneyBtnText, moneyBtnBg);

    // --- VICTORY BUTTON ---
    winBtnBg.setSize({210.f, 30.f});
    winBtnBg.setFillColor(sf::Color(150, 0, 150));
    winBtnBg.setPosition({1030.f, 530.f});
    winBtnText.setFont(font); winBtnText.setString("TRIGGER ENDING"); winBtnText.setCharacterSize(14);
    centerText(winBtnText, winBtnBg);

    // --- ITEM SELECTOR ---
    float itemY = 580.f;
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
    currentItemDisplay.setPosition({1030.f, 620.f});
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
    
    // 1. LISTEN FOR SECRET CODE (If not yet unlocked)
    if (!isUnlocked) {
        if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
            // Filter only printable characters (ASCII code between 32 and 126)
            if (textEntered->unicode >= 32 && textEntered->unicode <= 126) {
                secretBuffer += static_cast<char>(textEntered->unicode);
                
                // If buffer gets too long, keep only the last 5 characters
                if (secretBuffer.length() > 5) {
                    secretBuffer.erase(0, secretBuffer.length() - 5);
                }
                
                // Check if they typed "admin"
                if (secretBuffer == "admin") {
                    isUnlocked = true;
                    std::cout << "[DEBUG] Menu unlocked via secret code!\n";
                }
            }
        }
        return; 
    }

    // 2. NORMAL CLICK LOGIC (Only works if isUnlocked is true)
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            // Open/Close menu
            if (toggleBtnBg.getGlobalBounds().contains(mousePos)) {
                isExpanded = !isExpanded;
                return;
            }

            if (isExpanded) {
                // Money
                if (moneyBtnBg.getGlobalBounds().contains(mousePos)) {
                    player.setMoney(player.getMoney() + 10000.f);
                    std::cout << "[DEBUG] +10,000 Bs added.\n";
                }
                // Victory
                else if (winBtnBg.getGlobalBounds().contains(mousePos)) {
                    std::cout << "[DEBUG] TRIGGERING WARP DRIVE VICTORY!\n";
                    
                    if (onTriggerVictory) {
                        onTriggerVictory(); 
                    }
                }
                // Navigate Items
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
                // Give Item
                else if (giveItemBtn.getGlobalBounds().contains(mousePos)) {
                    if (!itemIDs.empty()) {
                        std::string id = itemIDs[currentItemIndex];
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

void DebugMenuUI::draw(sf::RenderWindow& window) {
    // If not unlocked, draws ABSOLUTELY NOTHING
    if (!isUnlocked) return;

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

void DebugMenuUI::update(const sf::Vector2f& mousePos) {
    // Here you can add hover effects (change color when mouse is over) like we did in TradeMenuUI
}