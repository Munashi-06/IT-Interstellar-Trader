#include "Interface/TradeMenuUI.hpp"
#include <iomanip> // For formatting decimals (std::fixed, std::setprecision)
#include <sstream>

/*
Alternative colors for the menu:
    1. Background: 
    (10, 10, 20, 235) - Very dark
    (20, 20, 40, 220) - Dark with a hint of blue
    (20, 20, 40, 200) - Dark but more transparent
    transparent black: (0, 0, 0, 180)
    2. Text:
    Cyan: (0, 255, 255)
    Yellow: (255, 255, 0)
    3. Borders:
    Cyan: (0, 255, 255)
    yellow: (255, 255, 0)
*/

TradeMenuUI::TradeMenuUI(const sf::Font& f) 
    : font(f),
      titleText(f, ""),
      playerSectionTitle(f, ""),
      playerMoneyText(f, ""),
      headerPlayerName(f, ""),
      headerPlayerQty(f, ""),
      headerPlayerSellPrice(f, ""),
      planetNameText(f, ""),
      headerPlanetName(f, ""),
      headerPlanetQty(f, ""),
      headerPlanetBuyPrice(f, ""),
      ctxBtnInfo(f, "Info."),
      ctxBtnAction(f, ""),
      ctxBtnCancel(f, "Cancel"),
      infoPopupText(f, ""),
      infoPopupEscText(f, "Press Esc. to close"),
      infoInputText(f, "Use Mouse Scroll to navigate through objects, RMB for more information, LMB to select, Esc. to exit."),
      playerActionBtnText(f, "SELL"),
      planetActionBtnText(f, "BUY"),
      headerPlayerCategory(f, "CATEGORY"),
      headerPlayerQuality(f, "QUALITY"),
      headerPlanetCategory(f, "CATEGORY"),
      headerPlanetQuality(f, "QUALITY"),
      togglePriceBtnText(f, "")
      
{
// --- LAYOUT CONSTANTS (Visual adjustments) ---
    sf::Vector2f panelSize(560.f, 550.f);
    sf::Color panelFill(0, 0, 0, 180); // Very dark, almost opaque for readability
    sf::Color cyanOutline(sf::Color::Cyan);
    int headerSize = 20;

    // General Title
    titleText.setFont(font);
    titleText.setString("TRADE MENU");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Cyan);
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
    titleText.setPosition({ 640.f, 50.f });

// --- LEFT SECTION: PLAYER ---
    float leftPanelX = 50.f;
    float topY = 100.f;
    float headerY = topY + 60.f;

    playerTableBg.setSize(panelSize);
    playerTableBg.setFillColor(panelFill);
    playerTableBg.setOutlineThickness(3);
    playerTableBg.setOutlineColor(cyanOutline);
    playerTableBg.setPosition({ leftPanelX, topY });

    playerSectionTitle.setFont(font);
    playerSectionTitle.setString("YOUR INVENTORY");
    playerSectionTitle.setCharacterSize(25);
    playerSectionTitle.setFillColor(sf::Color::Cyan);
    playerSectionTitle.setPosition({ leftPanelX + 20.f, topY + 15.f });

    playerMoneyText.setFont(font);
    playerMoneyText.setCharacterSize(18);
    playerMoneyText.setFillColor(sf::Color::Yellow);
    playerMoneyText.setPosition({ leftPanelX + 380.f, topY + 22.f });

    // Adjust X positions relative to leftPanelX to accommodate more columns
    headerPlayerName.setFont(font);
    headerPlayerName.setString("ITEM");
    headerPlayerName.setCharacterSize(16);
    headerPlayerName.setFillColor(cyanOutline);
    headerPlayerName.setPosition({ leftPanelX + 10.f, headerY });

    headerPlayerCategory.setFont(font);
    headerPlayerCategory.setString("CAT.");
    headerPlayerCategory.setCharacterSize(16);
    headerPlayerCategory.setFillColor(cyanOutline);
    headerPlayerCategory.setPosition({ leftPanelX + 225.f, headerY });

    headerPlayerQuality.setFont(font);
    headerPlayerQuality.setString("QUAL.");
    headerPlayerQuality.setCharacterSize(16);
    headerPlayerQuality.setFillColor(cyanOutline);
    headerPlayerQuality.setPosition({ leftPanelX + 330.f, headerY });

    headerPlayerQty.setFont(font);
    headerPlayerQty.setString("QTY.");
    headerPlayerQty.setCharacterSize(16);
    headerPlayerQty.setFillColor(cyanOutline);
    headerPlayerQty.setPosition({ leftPanelX + 400.f, headerY });

    headerPlayerSellPrice.setFont(font);
    headerPlayerSellPrice.setString("SELL");
    headerPlayerSellPrice.setCharacterSize(16);
    headerPlayerSellPrice.setFillColor(cyanOutline);
    headerPlayerSellPrice.setPosition({ leftPanelX + 470.f, headerY });

// --- RIGHT SECTION: PLANET ---
    float rightPanelX = 670.f;

    planetTableBg.setSize(panelSize);
    planetTableBg.setFillColor(panelFill);
    planetTableBg.setOutlineThickness(3);
    planetTableBg.setOutlineColor(cyanOutline);
    planetTableBg.setPosition({ rightPanelX, topY });

    planetNameText.setFont(font);
    // Planet name is dynamically updated in draw()
    planetNameText.setCharacterSize(25);
    planetNameText.setFillColor(sf::Color::Cyan);
    planetNameText.setPosition({ rightPanelX + 20.f, topY + 15.f });

    // Adjust X positions relative to rightPanelX
    headerPlanetName.setFont(font);
    headerPlanetName.setString("ITEM");
    headerPlanetName.setCharacterSize(16);
    headerPlanetName.setFillColor(cyanOutline);
    headerPlanetName.setPosition({ rightPanelX + 10.f, headerY });

    headerPlanetCategory.setFont(font);
    headerPlanetCategory.setString("CAT.");
    headerPlanetCategory.setCharacterSize(16);
    headerPlanetCategory.setFillColor(cyanOutline);
    headerPlanetCategory.setPosition({ rightPanelX + 225.f, headerY });

    headerPlanetQuality.setFont(font);
    headerPlanetQuality.setString("QUAL.");
    headerPlanetQuality.setCharacterSize(16);
    headerPlanetQuality.setFillColor(cyanOutline);
    headerPlanetQuality.setPosition({ rightPanelX + 330.f, headerY });

    headerPlanetQty.setFont(font);
    headerPlanetQty.setString("QTY.");
    headerPlanetQty.setCharacterSize(16);
    headerPlanetQty.setFillColor(cyanOutline);
    headerPlanetQty.setPosition({ rightPanelX + 400.f, headerY });

    headerPlanetBuyPrice.setFont(font);
    headerPlanetBuyPrice.setString("BUY");
    headerPlanetBuyPrice.setCharacterSize(16);
    headerPlanetBuyPrice.setFillColor(cyanOutline);
    headerPlanetBuyPrice.setPosition({ rightPanelX + 470.f, headerY });
    
// --- CONTEXT MENU CONFIGURATION ---
    contextMenuBg.setSize({ 150.f, 100.f });
    contextMenuBg.setFillColor(sf::Color(20, 20, 30, 240)); // Very opaque
    contextMenuBg.setOutlineThickness(2);
    contextMenuBg.setOutlineColor(sf::Color::Yellow); // Highlighted so it is noticeable

    int ctxFontSize = 16;
    ctxBtnInfo.setCharacterSize(ctxFontSize);   ctxBtnInfo.setFillColor(sf::Color::White);
    ctxBtnAction.setCharacterSize(ctxFontSize); ctxBtnAction.setFillColor(sf::Color::White);
    ctxBtnCancel.setCharacterSize(ctxFontSize); ctxBtnCancel.setFillColor(sf::Color::Red);

// --- INFO POPUP CONFIGURATION ---
    infoPopupBg.setFillColor(sf::Color(10, 10, 15, 245));
    infoPopupBg.setOutlineThickness(2);
    infoPopupBg.setOutlineColor(sf::Color::Cyan);

    infoPopupText.setCharacterSize(18);
    infoPopupText.setFillColor(sf::Color::White);

    // Configure ESC text
    infoPopupEscText.setCharacterSize(14);
    infoPopupEscText.setFillColor(sf::Color(150, 150, 150)); // A discreet gray

// --- INSTRUCTION TEXT CONFIGURATION ---
    infoInputText.setCharacterSize(16);
    infoInputText.setFillColor(sf::Color::White);
    sf::FloatRect inputBounds = infoInputText.getLocalBounds();
    infoInputText.setOrigin({ inputBounds.size.x / 2.f, inputBounds.size.y / 2.f });
    infoInputText.setPosition({ 640.f, 685.f });

// --- SELECTION HIGHLIGHT CONFIGURATION ---
    selectionHighlight.setSize({540.f, 35.f}); // Slightly narrower than the table (560)
    selectionHighlight.setFillColor(sf::Color(0, 255, 255, 60)); // Very transparent cyan

// --- ACTION BUTTONS CONFIGURATION ---
    sf::Vector2f btnSize(150.f, 40.f);
    float btnY = topY + 500.f; // Y position at the bottom of the table

    // Player Button (Sell)
    playerActionBtnBg.setSize(btnSize);
    playerActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gray (Disabled by default)
    playerActionBtnBg.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    playerActionBtnBg.setPosition({leftPanelX + (panelSize.x / 2.f), btnY});
    
    playerActionBtnText.setCharacterSize(16);
    sf::FloatRect pBounds = playerActionBtnText.getLocalBounds();
    playerActionBtnText.setOrigin({pBounds.size.x / 2.f, pBounds.size.y / 2.f});
    playerActionBtnText.setPosition({leftPanelX + (panelSize.x / 2.f), btnY - 5.f});

    // Planet Button (Buy)
    planetActionBtnBg.setSize(btnSize);
    planetActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gray (Disabled by default)
    planetActionBtnBg.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    planetActionBtnBg.setPosition({rightPanelX + (panelSize.x / 2.f), btnY});
    
    planetActionBtnText.setCharacterSize(16);
    sf::FloatRect mBounds = planetActionBtnText.getLocalBounds();
    planetActionBtnText.setOrigin({mBounds.size.x / 2.f, mBounds.size.y / 2.f});
    planetActionBtnText.setPosition({rightPanelX + (panelSize.x / 2.f), btnY - 5.f});

// --- CONFIGURATION FOR TOGGLE PRICE BUTTON ---
    togglePriceBtnBg.setSize({200.f, 30.f});
    togglePriceBtnBg.setFillColor(sf::Color(50, 50, 100, 100));
    togglePriceBtnBg.setOrigin({100.f, 15.f});
    togglePriceBtnBg.setPosition({1130.f, 60.f});

    togglePriceBtnText.setString("SHOW BASE PRICES");
    togglePriceBtnText.setCharacterSize(14);
    togglePriceBtnText.setFillColor(sf::Color(255, 255, 255));
    sf::FloatRect toggleBounds = togglePriceBtnText.getLocalBounds();
    togglePriceBtnText.setOrigin({toggleBounds.size.x / 2.f, toggleBounds.size.y / 2.f});
    togglePriceBtnText.setPosition({1130.f, 55.f});
}

void TradeMenuUI::draw(sf::RenderWindow& window, const Inventory& playerInv, const Planet& currentPlanet, float playerMoney, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog, const Player& player, int playerShipLevel) {
// LAYER 1: General Translucent Dark Background
    // window.draw(generalBackground);

// LAYER 2: Prominent Massive Planet and Background Star (if planet has a sprite)
    if (currentPlanet.hasSprite()) {
        // Get base orbital sprite and create a local copy
        sf::Sprite detailedPlanetSprite(*currentPlanet.getSprite());

        const sf::Texture& texture = detailedPlanetSprite.getTexture();
        sf::Vector2u texSize = texture.getSize();
        
        // Center the origin
        detailedPlanetSprite.setOrigin({ static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f });
        
        // Massive rescaling to 70% of screen width
        float targetWidth = 1280.f * 0.7f; 
        float detailedScale = targetWidth / static_cast<float>(texSize.x);
        detailedPlanetSprite.setScale({ detailedScale, detailedScale });

        // Planet position
        detailedPlanetSprite.setPosition({ 180.f, 600.f});

        // Adjust opacity: prominent but not blinding
        detailedPlanetSprite.setColor(sf::Color(255, 255, 255, 210)); 

        // Draw the massive planet behind the tables
        window.draw(detailedPlanetSprite);
    }

// LAYER 3: User Interface (Tables and Text)
    window.draw(titleText);
    window.draw(togglePriceBtnBg);
    window.draw(togglePriceBtnText);

    // --- DRAW PLAYER (Left) ---
    window.draw(playerTableBg);
    window.draw(playerSectionTitle);
    window.draw(headerPlayerName);
    window.draw(headerPlayerQty);
    window.draw(headerPlayerSellPrice);
    window.draw(headerPlayerCategory);
    window.draw(headerPlayerQuality);

    // Update Player Money
    std::stringstream ss;
    ss << "Bs. " << std::fixed << std::setprecision(2) << playerMoney;
    playerMoneyText.setString(ss.str());
    window.draw(playerMoneyText);

    // Draw Player items (using logical scroll)
    const auto& playerSlots = playerInv.getSlots();
    int drawnCount = 0;
    int currentSlotIndex = 0;
    float startY = 200.f;
    float rowHeight = 35.f;

    for (const auto& slot : playerSlots) {
        if (slot.has_value()) {
            if (currentSlotIndex >= playerStartIndex && drawnCount < maxVisibleRows) {
                const auto& itemData = catalog.at(slot->itemID);
                float yPos = startY + (drawnCount * rowHeight);

                if (selectedItemID == slot->itemID && isPlayerItem) {
                    selectionHighlight.setPosition({60.f, yPos});
                    window.draw(selectionHighlight);
                }

                // Get rarity color
                sf::Color rarityColor;
                switch (itemData->getRarity()) {
                    case Rarity::Common: rarityColor = sf::Color::White; break;
                    case Rarity::Rare: rarityColor = sf::Color::Green; break;
                    case Rarity::Exotic: rarityColor = sf::Color::Blue; break;
                    case Rarity::Legendary: rarityColor = sf::Color(128, 0, 128); break;
                    case Rarity::Quest: rarityColor = sf::Color(255, 215, 0); break;
                }

                sf::Text nameT(font, itemData->getName());
                nameT.setCharacterSize(14);
                nameT.setFillColor(sf::Color::White);
                nameT.setPosition({ 60.f, yPos });
                window.draw(nameT);
                
                sf::Text catT(font, itemData->getCategoryString().substr(0, 8)); // Truncate if too long
                catT.setCharacterSize(14);
                catT.setFillColor(sf::Color::White);
                catT.setPosition({ 275.f, yPos });
                window.draw(catT);
                
                sf::Text qualT(font, itemData->getRarityString().substr(0, 3)); // Truncate ("Com", "Rar")
                qualT.setCharacterSize(14);
                qualT.setFillColor(rarityColor);
                qualT.setPosition({ 380.f, yPos });
                window.draw(qualT);

                sf::Text qtyT(font, std::to_string(slot->quantity));
                qtyT.setCharacterSize(14);
                qtyT.setFillColor(sf::Color::White);
                qtyT.setPosition({ 450.f, yPos });
                window.draw(qtyT);
                
                float localBasePrice = currentPlanet.getLocalBasePrice(slot->itemID, catalog);
                float planetPrice = currentPlanet.getItemPrice(slot->itemID, catalog);
                float finalSellPrice;

                sf::Text priceT(font, ""); 
                priceT.setFillColor(sf::Color::White);

                if (showingOriginalPrices) {
                    finalSellPrice = localBasePrice;
                } else {
                    finalSellPrice = TradeManager::getFinalSellPrice(*itemData, planetPrice, player);
                    
                    if (finalSellPrice > localBasePrice) {
                        priceT.setFillColor(sf::Color::Green);
                    } else if (finalSellPrice < localBasePrice) {
                        priceT.setFillColor(sf::Color::Red);
                    }
                }
                ss.str("");
                ss << "Bs." << std::fixed << std::setprecision(2) << finalSellPrice;
                priceT.setString(ss.str());
                priceT.setCharacterSize(14);
                priceT.setPosition({ 520.f, yPos });
                window.draw(priceT);

                drawnCount++;
            }
            currentSlotIndex++;
        }
    }

    // --- DRAW PLANET (Right) ---
    window.draw(planetTableBg);
    // Update Planet Name
    planetNameText.setString("MARKET - " + currentPlanet.getName());
    window.draw(planetNameText);
    
    window.draw(headerPlanetName);
    window.draw(headerPlanetQty);
    window.draw(headerPlanetBuyPrice);
    window.draw(headerPlanetCategory);
    window.draw(headerPlanetQuality);

    // Draw Planet items (Local market)
    const auto& planetSlots = currentPlanet.getLocalStock(); 
    drawnCount = 0;
    currentSlotIndex = 0;
    
    for (const auto& slot : planetSlots) {
        if (slot.has_value()) {
            const auto& itemData = catalog.at(slot->itemID);
            
            // 🆕 FILTRO: ¿este item es visible según el nivel de la nave?
            float visibility = getVisibilityPercent(itemData->getRarity(), playerShipLevel);
            
            // Usar el itemID como semilla para que el mismo item siempre sea visible/invisible
            // Esto evita que los items parpadeen al hacer scroll
            std::hash<std::string> hasher;
            size_t hashValue = hasher(slot->itemID + std::to_string(playerShipLevel));
            bool isVisible = (hashValue % 100) < (visibility * 100);
            
            if (isVisible) {
                if (currentSlotIndex >= planetStartIndex && drawnCount < maxVisibleRows) {
                    float yPos = startY + (drawnCount * rowHeight);

                    if (selectedItemID == slot->itemID && !isPlayerItem) {
                        selectionHighlight.setPosition({680.f, yPos});
                        window.draw(selectionHighlight);
                    }

                    sf::Color rarityColor;
                    switch (itemData->getRarity()) {
                        case Rarity::Common: rarityColor = sf::Color::White; break;
                        case Rarity::Rare: rarityColor = sf::Color::Green; break;
                        case Rarity::Exotic: rarityColor = sf::Color::Blue; break;
                        case Rarity::Legendary: rarityColor = sf::Color(128, 0, 128); break;
                        case Rarity::Quest: rarityColor = sf::Color(255, 215, 0); break;
                    }

                    sf::Text nameT(font, itemData->getName());
                    nameT.setCharacterSize(14);
                    nameT.setFillColor(sf::Color::White);
                    nameT.setPosition({ 680.f, yPos });
                    window.draw(nameT);
                    
                    sf::Text catT(font, itemData->getCategoryString().substr(0, 8)); 
                    catT.setCharacterSize(14);
                    catT.setFillColor(sf::Color::White);
                    catT.setPosition({ 895.f, yPos });
                    window.draw(catT);
                    
                    sf::Text qualT(font, itemData->getRarityString().substr(0, 3)); 
                    qualT.setCharacterSize(14);
                    qualT.setFillColor(rarityColor);
                    qualT.setPosition({ 1000.f, yPos });
                    window.draw(qualT);

                sf::Text qtyT(font, std::to_string(slot->quantity));
                qtyT.setCharacterSize(14); qtyT.setFillColor(sf::Color::White);
                qtyT.setPosition({ 1070.f, yPos });
                window.draw(qtyT);
                
                float localBasePrice = currentPlanet.getLocalBasePrice(slot->itemID, catalog);  // Atributos
                float planetPrice = currentPlanet.getItemPrice(slot->itemID, catalog);           // Atributos + Eventos
                float finalBuyPrice;

                sf::Text priceT(font, ""); 
                priceT.setFillColor(sf::Color::White);

                if (showingOriginalPrices) {
                    finalBuyPrice = localBasePrice;
                } else {
                    finalBuyPrice = TradeManager::getFinalBuyPrice(*itemData, planetPrice, player);
                    
                    if (finalBuyPrice < localBasePrice) {
                        priceT.setFillColor(sf::Color::Green);
                    } else if (finalBuyPrice > localBasePrice) {
                        priceT.setFillColor(sf::Color::Red);
                    }
                }
                ss.str("");
                ss << "Bs." << std::fixed << std::setprecision(2) << finalBuyPrice;
                priceT.setString(ss.str());
                priceT.setCharacterSize(14);
                priceT.setPosition({ 1140.f, yPos });
                window.draw(priceT);

                    drawnCount++;
                }
                currentSlotIndex++;
            }
        }
    }

    // Draw instruction text at the bottom
    window.draw(infoInputText);

// --- UPDATE AND DRAW ACTION BUTTONS ---
    // If a player item is selected, activate (color green) the Sell button
    if (selectedItemID != "" && isPlayerItem) {
        playerActionBtnBg.setFillColor(sf::Color(0, 150, 0)); // Green
    }
    else {
        playerActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gray
    }
    
    // If a planet item is selected, activate (color green) the Buy button
    if (selectedItemID != "" && !isPlayerItem) {
        planetActionBtnBg.setFillColor(sf::Color(0, 150, 0)); // Green
    }
    else {
        planetActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gray
    }

    // Draw buttons on screen
    window.draw(playerActionBtnBg);
    window.draw(playerActionBtnText);
    window.draw(planetActionBtnBg);
    window.draw(planetActionBtnText);

// LAYER 4: Information Popup (Top Layer)
    // Note: Removed the "showContextMenu" logic previously here.
    if (showInfoPopup) {
        // Extra semi-transparent rectangle to darken background when info is open
        sf::RectangleShape darkOverlay({1280.f, 720.f});
        darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(darkOverlay);

        window.draw(infoPopupBg);
        window.draw(infoPopupText);
        window.draw(infoPopupEscText);
    }
}

void TradeMenuUI::update(const sf::Vector2f& mousePos) {
    // If info popup is open, we don't want background items to highlight
    if (showInfoPopup) return;

    // Hover colors
    sf::Color normalColor = sf::Color::Cyan;
    sf::Color hoverColor = sf::Color::Yellow;

    // --- PLAYER HEADERS HOVER ---
    if (headerPlayerName.getGlobalBounds().contains(mousePos)) headerPlayerName.setFillColor(hoverColor);
    else headerPlayerName.setFillColor(normalColor);

    if (headerPlayerCategory.getGlobalBounds().contains(mousePos)) headerPlayerCategory.setFillColor(hoverColor);
    else headerPlayerCategory.setFillColor(normalColor);

    if (headerPlayerQuality.getGlobalBounds().contains(mousePos)) headerPlayerQuality.setFillColor(hoverColor);
    else headerPlayerQuality.setFillColor(normalColor);

    if (headerPlayerSellPrice.getGlobalBounds().contains(mousePos)) headerPlayerSellPrice.setFillColor(hoverColor);
    else headerPlayerSellPrice.setFillColor(normalColor);

    if (headerPlayerQty.getGlobalBounds().contains(mousePos)) headerPlayerQty.setFillColor(hoverColor);
    else headerPlayerQty.setFillColor(normalColor);

    // --- PLANET HEADERS HOVER ---
    if (headerPlanetName.getGlobalBounds().contains(mousePos)) headerPlanetName.setFillColor(hoverColor);
    else headerPlanetName.setFillColor(normalColor);

    if (headerPlanetCategory.getGlobalBounds().contains(mousePos)) headerPlanetCategory.setFillColor(hoverColor);
    else headerPlanetCategory.setFillColor(normalColor);

    if (headerPlanetQuality.getGlobalBounds().contains(mousePos)) headerPlanetQuality.setFillColor(hoverColor);
    else headerPlanetQuality.setFillColor(normalColor);

    if (headerPlanetBuyPrice.getGlobalBounds().contains(mousePos)) headerPlanetBuyPrice.setFillColor(hoverColor);
    else headerPlanetBuyPrice.setFillColor(normalColor);

    if (headerPlanetQty.getGlobalBounds().contains(mousePos)) headerPlanetQty.setFillColor(hoverColor);
    else headerPlanetQty.setFillColor(normalColor);

    // Hover for Toggle Prices Button
    if (togglePriceBtnBg.getGlobalBounds().contains(mousePos)) {
        togglePriceBtnText.setFillColor(hoverColor);
    }
    else {
        if(togglePriceBtnText.getString() == "SHOW BASE PRICES") {
            togglePriceBtnText.setFillColor(sf::Color(54, 54, 217));
        }
        else {
            togglePriceBtnText.setFillColor(sf::Color(255, 255, 255));
        }
    }
}

void TradeMenuUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Inventory& playerInv, Planet& currentPlanet, Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    
    // --- SCROLL DETECTION ---
    if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
            // Check if mouse is over player table (Left)
            if (playerTableBg.getGlobalBounds().contains(mousePos)) {
                // Count actual player items
                int playerItemCount = 0;
                for (const auto& slot : playerInv.getSlots()) {
                    if (slot.has_value()) playerItemCount++;
                }
                int maxPlayerScroll = std::max(0, playerItemCount - maxVisibleRows);
                
                if (mouseWheel->delta > 0) {
                    playerStartIndex = std::max(0, playerStartIndex - 1);
                }
                else if (mouseWheel->delta < 0) {
                    playerStartIndex = std::min(maxPlayerScroll, playerStartIndex + 1);
                }
            }
            // Check if mouse is over planet table (Right)
            else if (planetTableBg.getGlobalBounds().contains(mousePos)) {
                // Count actual planet items
                int planetItemCount = 0;
                for (const auto& slot : currentPlanet.getLocalStock()) {
                    if (slot.has_value()) planetItemCount++;
                }
                int maxPlanetScroll = std::max(0, planetItemCount - maxVisibleRows);
                
                if (mouseWheel->delta > 0) {
                    planetStartIndex = std::max(0, planetStartIndex - 1);
                }
                else if (mouseWheel->delta < 0) {
                    planetStartIndex = std::min(maxPlanetScroll, planetStartIndex + 1);
                }
            }
        }
    }

    // --- MAGIC AUXILIARY FUNCTION ---
    auto getClickedItemID = [](const std::vector<std::optional<ItemStack>>& slots, int startIndex, int clickedRow) -> std::string {
        int validCount = 0;
        int targetIndex = startIndex + clickedRow;
        for (const auto& slot : slots) {
            if (slot.has_value()) {
                if (validCount == targetIndex) {
                    return slot->itemID;
                }
                validCount++;
            }
        }
        return "";
    };

    // --- CLICK DETECTION ---
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        float startY = 200.f;
        float rowHeight = 35.f;

        // --- RIGHT CLICK (Open Info Menu) ---
        if (mouseBtn->button == sf::Mouse::Button::Right && !showInfoPopup) {
            if (mousePos.y >= startY && mousePos.y < startY + (maxVisibleRows * rowHeight)) {
                int clickedRow = static_cast<int>((mousePos.y - startY) / rowHeight);

                // Player Click
                if (mousePos.x >= 50.f && mousePos.x <= 610.f) {
                    std::string foundID = getClickedItemID(playerInv.getSlots(), playerStartIndex, clickedRow);
                    if (foundID != "") {
                        selectedItemID = foundID;
                        isPlayerItem = true;
                        
                        const auto& itemData = catalog.at(selectedItemID);
                        std::stringstream ss;
                        ss << "NAME: " << itemData->getName() << "\n\n"
                           << "CATEGORY: " << itemData->getCategoryString() << "\n\n"
                           << "RARITY: " << itemData->getRarityString() << "\n\n"
                           << "SELL PRICE: Bs. " << std::fixed << std::setprecision(2) << currentPlanet.getItemPrice(selectedItemID, catalog);
                        
                        infoPopupText.setString(ss.str());
                        
                        sf::FloatRect textBounds = infoPopupText.getLocalBounds();
                        float newWidth = std::max(350.f, textBounds.size.x + 60.f); 
                        float newHeight = 250.f;
                        infoPopupBg.setSize({newWidth, newHeight});
                        infoPopupBg.setOrigin({newWidth / 2.f, newHeight / 2.f});
                        infoPopupBg.setPosition({640.f, 360.f});
                        infoPopupText.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
                        infoPopupText.setPosition({640.f, 340.f});
                        sf::FloatRect escBounds = infoPopupEscText.getLocalBounds();
                        infoPopupEscText.setOrigin({escBounds.size.x / 2.f, escBounds.size.y / 2.f});
                        infoPopupEscText.setPosition({640.f, 360.f + (newHeight / 2.f) - 25.f}); 
                        
                        showInfoPopup = true;
                    }
                }
                // Planet Click
                else if (mousePos.x >= 670.f && mousePos.x <= 1230.f) {
                    std::string foundID = getClickedItemID(currentPlanet.getLocalStock(), planetStartIndex, clickedRow);
                    if (foundID != "") {
                        selectedItemID = foundID;
                        isPlayerItem = false;

                        const auto& itemData = catalog.at(selectedItemID);
                        std::stringstream ss;
                        ss << "NAME: " << itemData->getName() << "\n\n"
                           << "CATEGORY: " << itemData->getCategoryString() << "\n\n"
                           << "RARITY: " << itemData->getRarityString() << "\n\n"
                           << "BUY PRICE: Bs. " << std::fixed << std::setprecision(2) << currentPlanet.getItemPrice(selectedItemID, catalog);
                        
                        infoPopupText.setString(ss.str());
                        
                        sf::FloatRect textBounds = infoPopupText.getLocalBounds();
                        float newWidth = std::max(350.f, textBounds.size.x + 60.f); 
                        float newHeight = 250.f;
                        infoPopupBg.setSize({newWidth, newHeight});
                        infoPopupBg.setOrigin({newWidth / 2.f, newHeight / 2.f});
                        infoPopupBg.setPosition({640.f, 360.f});
                        infoPopupText.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
                        infoPopupText.setPosition({640.f, 340.f});
                        sf::FloatRect escBounds = infoPopupEscText.getLocalBounds();
                        infoPopupEscText.setOrigin({escBounds.size.x / 2.f, escBounds.size.y / 2.f});
                        infoPopupEscText.setPosition({640.f, 360.f + (newHeight / 2.f) - 25.f}); 
                        
                        showInfoPopup = true;
                    }
                }
            }
        }

        // --- LEFT CLICK (Select, Sort or Act) ---
        else if (mouseBtn->button == sf::Mouse::Button::Left) {
            if (showInfoPopup) {
                showInfoPopup = false;
                return; // Close popup and do nothing else this frame
            }

            // Toggle Price Button Click
            if (togglePriceBtnBg.getGlobalBounds().contains(mousePos)) {
                showingOriginalPrices = !showingOriginalPrices;

                if (showingOriginalPrices) {
                    togglePriceBtnText.setString("SHOW PLAYER DISCOUNTS");
                    togglePriceBtnBg.setFillColor(sf::Color(255, 255, 255, 100)); // White when showing base
                    togglePriceBtnText.setFillColor(sf::Color(0, 0, 0)); // Black text for better contrast
                }
                else {
                    togglePriceBtnText.setString("SHOW BASE PRICES");
                    togglePriceBtnBg.setFillColor(sf::Color(54, 54, 217, 100)); // Blue when showing discounts
                    togglePriceBtnText.setFillColor(sf::Color(255, 255, 255)); // White text for better contrast
                }

                // Re-center text just in case the length changed
                sf::FloatRect newBounds = togglePriceBtnText.getLocalBounds();
                togglePriceBtnText.setOrigin({newBounds.size.x / 2.f, newBounds.size.y / 2.f});
                togglePriceBtnText.setPosition({1130.f, 55.f});
                return;
            }

            // 1. CHECK CLICK ON PLAYER HEADERS (For Sorting)
            if (headerPlayerName.getGlobalBounds().contains(mousePos)) {
                if (playerCurrentSort == SortColumn::Name) playerSortAscending = !playerSortAscending;
                else { playerCurrentSort = SortColumn::Name; playerSortAscending = true; }
                playerInv.sortByName(playerSortAscending, catalog);
                return;
            }
            else if (headerPlayerCategory.getGlobalBounds().contains(mousePos)) {
                if (playerCurrentSort == SortColumn::Category) playerSortAscending = !playerSortAscending;
                else { playerCurrentSort = SortColumn::Category; playerSortAscending = true; }
                playerInv.sortByCategory(playerSortAscending, catalog);
                return;
            }
            else if (headerPlayerQuality.getGlobalBounds().contains(mousePos)) {
                if (playerCurrentSort == SortColumn::Quality) playerSortAscending = !playerSortAscending;
                else { playerCurrentSort = SortColumn::Quality; playerSortAscending = true; }
                playerInv.sortByQuality(playerSortAscending, catalog);
                return;
            }
            else if (headerPlayerSellPrice.getGlobalBounds().contains(mousePos)) {
                if (playerCurrentSort == SortColumn::Price) playerSortAscending = !playerSortAscending;
                else { playerCurrentSort = SortColumn::Price; playerSortAscending = true; }
                playerInv.sortByPrice(playerSortAscending, catalog);
                return;
            }
            else if (headerPlayerQty.getGlobalBounds().contains(mousePos)) {
                if (playerCurrentSort == SortColumn::Quantity) playerSortAscending = !playerSortAscending;
                else { playerCurrentSort = SortColumn::Quantity; playerSortAscending = true; }
                playerInv.sortByQuantity(playerSortAscending, catalog);
                return;
            }

            // 2. CHECK CLICK ON PLANET HEADERS (For Sorting)
            if (headerPlanetName.getGlobalBounds().contains(mousePos)) {
                if (planetCurrentSort == SortColumn::Name) planetSortAscending = !planetSortAscending;
                else { planetCurrentSort = SortColumn::Name; planetSortAscending = true; }
                currentPlanet.sortByName(planetSortAscending, catalog); 
                return;
            }
            else if (headerPlanetCategory.getGlobalBounds().contains(mousePos)) {
                if (planetCurrentSort == SortColumn::Category) planetSortAscending = !planetSortAscending;
                else { planetCurrentSort = SortColumn::Category; planetSortAscending = true; }
                currentPlanet.sortByCategory(planetSortAscending, catalog);
                return;
            }
            else if (headerPlanetQuality.getGlobalBounds().contains(mousePos)) {
                if (planetCurrentSort == SortColumn::Quality) planetSortAscending = !planetSortAscending;
                else { planetCurrentSort = SortColumn::Quality; planetSortAscending = true; }
                currentPlanet.sortByQuality(planetSortAscending, catalog);
                return;
            }
            else if (headerPlanetBuyPrice.getGlobalBounds().contains(mousePos)) {
                if (planetCurrentSort == SortColumn::Price) planetSortAscending = !planetSortAscending;
                else { planetCurrentSort = SortColumn::Price; planetSortAscending = true; }
                currentPlanet.sortByPrice(planetSortAscending, catalog);
                return;
            }
            else if (headerPlanetQty.getGlobalBounds().contains(mousePos)) {
                if (planetCurrentSort == SortColumn::Quantity) planetSortAscending = !planetSortAscending;
                else { planetCurrentSort = SortColumn::Quantity; planetSortAscending = true; }
                currentPlanet.sortByQuantity(planetSortAscending, catalog);
                return;
            }
            // 3. CHECK CLICK ON ACTION BUTTONS (Buy/Sell)
            if (selectedItemID != "") {
                if (isPlayerItem && playerActionBtnBg.getGlobalBounds().contains(mousePos)) {
                    // Click on SELL
                    TradeManager::sellItem(selectedItemID, player, playerInv, currentPlanet, catalog);
                    return; 
                }
                else if (!isPlayerItem && planetActionBtnBg.getGlobalBounds().contains(mousePos)) {
                    // Click on BUY
                    TradeManager::buyItem(selectedItemID, player, playerInv, currentPlanet, catalog);
                    return;
                }
            }

            // 4. CHECK SELECTION IN LISTS
            bool clickedOnItem = false;
            if (mousePos.y >= startY && mousePos.y < startY + (maxVisibleRows * rowHeight)) {
                int clickedRow = static_cast<int>((mousePos.y - startY) / rowHeight);

                // Player Click
                if (mousePos.x >= 50.f && mousePos.x <= 610.f) {
                    std::string foundID = getClickedItemID(playerInv.getSlots(), playerStartIndex, clickedRow);
                    if (foundID != "") {
                        // If we click the same one, we deselect it
                        if (selectedItemID == foundID && isPlayerItem) {
                            selectedItemID = "";
                        } else {
                            selectedItemID = foundID;
                            isPlayerItem = true;
                        }
                        clickedOnItem = true;
                    }
                }
                // Planet Click
                else if (mousePos.x >= 670.f && mousePos.x <= 1230.f) {
                    std::string foundID = getClickedItemID(currentPlanet.getLocalStock(), planetStartIndex, clickedRow);
                    if (foundID != "") {
                        if (selectedItemID == foundID && !isPlayerItem) {
                            selectedItemID = "";
                        } else {
                            selectedItemID = foundID;
                            isPlayerItem = false;
                        }
                        clickedOnItem = true;
                    }
                }
            }

            // If clicked on nothing, deselect
            if (!clickedOnItem) {
                selectedItemID = "";
            }
        }
    }
}

float TradeMenuUI::getVisibilityPercent(Rarity rarity, int shipLevel) const{
    switch (rarity){
        case Rarity::Common:
            switch(shipLevel){
                case 1: return 0.60f;
                case 2: return 0.85f;
                case 3: return 1.00f;
                default: return 1.00f;
            }

        case Rarity::Rare:
            switch(shipLevel){
                case 1: return 0.55f;
                case 2: return 0.75f;
                case 3: return 1.00f;
                default: return 1.00f;
            }
        
        case Rarity::Exotic:
            switch(shipLevel){
                case 1: return 0.50f;
                case 2: return 0.70f;
                case 3: return 1.00f;
                default: return 1.00f;
            }
        
        case Rarity::Legendary:
            switch(shipLevel){
                case 1: return 0.45f;
                case 2: return 0.70f;
                case 3: return 1.00f;
                default: return 1.00f;
            }
        
        default: return 1.00f;

    }
}