#include "Interface/ShipMenuUI.hpp"
#include "Entities/Player.hpp"

ShipMenuUI::ShipMenuUI(const sf::Font& f, const sf::Texture& shipTex) 
    : font(f),                          // Initialize font reference
      shipPreview(shipTex),              // Sprite NEEDS the texture now
      titleText(f, ""),                  // Text NEEDS font and initial string
      escText(f, ""),
      upgradeText(f, ""),
      headerName(f, "NAME"),             // Direct text initialization
      headerCategory(f, "CATEGORY"),
      headerQuality(f, "QUALITY"),
      headerPrice(f, "PRICE") 
{
    float tableStartX = 450.f;
    float headerY = 200.f;
    int fontSize = 18;

    // 1. Background (1024x576) and table background
    background.setSize({1024.f, 576.f});
    background.setFillColor(sf::Color(15, 15, 25, 220)); // Slightly darker
    background.setOutlineThickness(3);
    background.setOutlineColor(sf::Color::Cyan);
    background.setOrigin({512.f, 288.f});
    background.setPosition({640.f, 360.f}); // Screen center

    // Specific background for the table to make it stand out
    tableBackground.setSize({600.f, 400.f});
    tableBackground.setFillColor(sf::Color(20, 20, 30, 200));
    tableBackground.setPosition({tableStartX - 12.f, headerY - 5.f}); 
    tableBackground.setOutlineThickness(1);
    tableBackground.setOutlineColor(sf::Color::White);

    // 2. Ship preview on the left side
    shipPreview.setTexture(shipTex);
    shipPreview.setScale({0.5f, 0.5f}); // Smaller
    shipPreview.setPosition({150.f, 90.f});

    // --- UPGRADE BUTTON CONFIGURATION ---
    upgradeBtn.setSize({200.f, 40.f});
    upgradeBtn.setFillColor(sf::Color(0, 150, 0));
    upgradeBtn.setPosition({170.f, 450.f}); // Located under the ship

    upgradeText.setString("UPGRADE");
    upgradeText.setCharacterSize(18);
    upgradeText.setFillColor(sf::Color::White);
    
    // Center the text mathematically within the button
    sf::FloatRect upRect = upgradeText.getLocalBounds();
    upgradeText.setOrigin({upRect.size.x / 2.f, upRect.size.y / 2.f});
    upgradeText.setPosition({(tableStartX - 280.f) + 100.f, 450.f + 15.f}); 

    // 3. General Title
    titleText.setFont(font);
    titleText.setString("SHIP STATUS AND INVENTORY");
    titleText.setCharacterSize(25);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition({400.f, 100.f}); 

    // 4. Table Headers (Right side)
    headerName.setFont(font);
    headerName.setString("NAME");
    headerName.setPosition({tableStartX, headerY});
    headerName.setCharacterSize(fontSize);

    headerCategory.setFont(font);
    headerCategory.setString("CATEGORY");
    headerCategory.setPosition({tableStartX + 250.f, headerY});
    headerCategory.setCharacterSize(fontSize);

    headerQuality.setFont(font);
    headerQuality.setString("QUALITY");
    headerQuality.setPosition({tableStartX + 370.f, headerY});
    headerQuality.setCharacterSize(fontSize);

    headerPrice.setFont(font);
    headerPrice.setString("PRICE");
    headerPrice.setPosition({tableStartX + 490.f, headerY});
    headerPrice.setCharacterSize(fontSize);
    
    // 5. Setup visual scrollbar (Right of the table)
    scrollTrack.setSize({10.f, 400.f}); 
    scrollTrack.setFillColor(sf::Color(50, 50, 50, 150));
    scrollTrack.setPosition({tableStartX + 600.f, headerY - 5.f}); 

    scrollThumb.setSize({10.f, 50.f}); // Dynamic adjustment later
    scrollThumb.setFillColor(sf::Color::Cyan);
    scrollThumb.setPosition({tableStartX + 600.f, headerY - 5.f}); 
}

void ShipMenuUI::draw(sf::RenderWindow& window, const Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    // Draw backgrounds, ship, buttons, and headers
    window.draw(background);
    window.draw(shipPreview);
    window.draw(titleText);
    window.draw(tableBackground);
    window.draw(upgradeBtn);
    window.draw(upgradeText);
    
    window.draw(headerName);
    window.draw(headerCategory);
    window.draw(headerQuality);
    window.draw(headerPrice);

    // --- DRAW TABLE WITH SCROLL ---
    const auto& slots = inventory.getSlots(); 
    
    // Count real items to adjust scroll
    int totalItems = 0;
    for (const auto& slot : slots) {
        if (slot.has_value()) totalItems++;
    }

    // Draw items based on startIndex
    float startY = 240.f;
    float rowHeight = 35.f;
    int itemsDrawn = 0;
    int currentItemIndex = 0;

    for (const auto& slot : slots) {
        if (slot.has_value()) {
            // Only draw if within visible scroll range
            if (currentItemIndex >= startIndex && itemsDrawn < maxVisibleItems) {
                const auto& itemData = catalog.at(slot->itemID);

                sf::Text rowText(font, itemData->getName() + " x " + std::to_string(slot->quantity));
                sf::Text categoryText(font, itemData->getCategoryString());
                sf::Text qualityText(font, itemData->getRarityString());
                
                // Price formatting (truncating to 2 decimals)
                std::string price = std::to_string(itemData->getPrice());
                price = price.substr(0, price.find(".") + 3); 
                price = "Bs. " + price; 
                sf::Text priceText(font, price);

                sf::Color rarityColor;
                switch (itemData->getRarity()) {
                    case Rarity::Common: rarityColor = sf::Color::White; break;
                    case Rarity::Rare: rarityColor = sf::Color::Green; break;
                    case Rarity::Exotic: rarityColor = sf::Color::Blue; break;
                    case Rarity::Legendary: rarityColor = sf::Color(128, 0, 128); break; // Purple
                    case Rarity::Quest: rarityColor = sf::Color(255, 215, 0); break; // Gold
                }
                
                rowText.setCharacterSize(14);
                rowText.setFillColor(sf::Color::White);
                categoryText.setCharacterSize(14);
                categoryText.setFillColor(sf::Color::White);
                qualityText.setCharacterSize(14);
                qualityText.setFillColor(rarityColor);
                priceText.setCharacterSize(14);
                priceText.setFillColor(sf::Color::White);

                float yPos = startY + (itemsDrawn * rowHeight);

                rowText.setPosition({450.f, yPos});
                window.draw(rowText);

                categoryText.setPosition({480.f + 220.f, yPos});
                window.draw(categoryText);

                qualityText.setPosition({450.f + 370.f, yPos});
                window.draw(qualityText);

                priceText.setPosition({450.f + 490.f, yPos});
                window.draw(priceText);

                itemsDrawn++;
            }
            currentItemIndex++;
        }
    }

    // Update scrollbar (Thumb) position
    if (totalItems > maxVisibleItems) {
        float scrollPercent = (float)startIndex / (totalItems - maxVisibleItems);
        float thumbY = 195.f + (scrollPercent * (400.f - scrollThumb.getSize().y));
        scrollThumb.setPosition({450.f + 600.f, thumbY});
        
        window.draw(scrollTrack);
        window.draw(scrollThumb);
    }
}

void ShipMenuUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, int totalItems, Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog, State& currentState, Player& player) {
    // Detect mouse scroll
    if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
            if (mouseWheel->delta > 0) {
                startIndex--;
            } else if (mouseWheel->delta < 0) {
                startIndex++;
            }

            // Bound scroll range
            if (startIndex < 0) startIndex = 0;
            int maxStart = std::max(0, totalItems - maxVisibleItems);
            if (startIndex > maxStart) startIndex = maxStart;
        }
    }

    // Detect mouse clicks
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            // click on Header NAME
            if (headerName.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Name) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Name; sortAscending = true; }
                inventory.sortByName(sortAscending, catalog);
            }
            
            // Click on Header CATEGORY
            else if (headerCategory.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Category) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Category; sortAscending = true; }
                inventory.sortByCategory(sortAscending, catalog);
            }
            
            // Click on Header QUALITY
            else if (headerQuality.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Quality) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Quality; sortAscending = true; }
                inventory.sortByQuality(sortAscending, catalog);
            }
            
            // Click on Header PRICE
            else if (headerPrice.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Price) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Price; sortAscending = true; }
                inventory.sortByPrice(sortAscending, catalog);
            }

            // Click in UPGRADE
            if (upgradeBtn.getGlobalBounds().contains(mousePos)) {
                // Use This for adding more upgrades (More item Stock)
                // if (player.upgradeShip()) {
                //     std::cout << "Ship Level: " << player.getShipLevel() << std::endl;
                // }
                currentState = State::UpgradeTree;
                return;
            }
        }
    }
}

void ShipMenuUI::update(const sf::Vector2f& mousePos) {
    // --- HOVER FOR UPGRADE BUTTON ---
    if (upgradeBtn.getGlobalBounds().contains(mousePos)) {
        upgradeBtn.setFillColor(sf::Color(0, 200, 0));
    } else {
        upgradeBtn.setFillColor(sf::Color(0, 150, 0));
    }

    // --- HOVER FOR HEADERS ---
    sf::Color normalColor = sf::Color::White;
    sf::Color hoverColor = sf::Color::Yellow;

    if (headerName.getGlobalBounds().contains(mousePos)) headerName.setFillColor(hoverColor);
    else headerName.setFillColor(normalColor);

    if (headerCategory.getGlobalBounds().contains(mousePos)) headerCategory.setFillColor(hoverColor);
    else headerCategory.setFillColor(normalColor);

    if (headerQuality.getGlobalBounds().contains(mousePos)) headerQuality.setFillColor(hoverColor);
    else headerQuality.setFillColor(normalColor);

    if (headerPrice.getGlobalBounds().contains(mousePos)) headerPrice.setFillColor(hoverColor);
    else headerPrice.setFillColor(normalColor);
}