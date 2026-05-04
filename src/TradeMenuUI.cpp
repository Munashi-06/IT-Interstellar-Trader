#include "TradeMenuUI.hpp"
#include <iomanip> // Para formatear decimales (std::fixed, std::setprecision)
#include <sstream>

/*
Colores alternativos para el menú:
    1. Fondo: 
    (10, 10, 20, 235) - Muy oscuro
    (20, 20, 40, 220) - Oscuro con un toque de azul
    (20, 20, 40, 200) - Oscuro pero más transparente
    negro transparente: (0, 0, 0, 180)
    2. Texto:
    Cyan: (0, 255, 255)
    Yellow: (255, 255, 0)
    3. Bordes:
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
      ctxBtnCancel(f, "Cancelar"),
      infoPopupText(f, ""),
      infoPopupEscText(f, "Presione Esc. para cerrar"),
      infoInputText(f, "Use el Scroll del Mouse para navegar por los objetos, RMB para mas informacion, LMB para seleccionar, Esc. para salir."),
      playerActionBtnText(f, "VENDER"),
      planetActionBtnText(f, "COMPRAR"),
      headerPlayerCategory(f, "CATEGORIA"),
      headerPlayerQuality(f, "CALIDAD"),
      headerPlanetCategory(f, "CATEGORIA"),
      headerPlanetQuality(f, "CALIDAD")
{
// --- LAYOUT CONSTANTS (Ajustes visuales) ---
    sf::Vector2f panelSize(560.f, 550.f);
    sf::Color panelFill(0, 0, 0, 180); // Muy oscuro, casi opaco para legibilidad
    sf::Color cyanOutline(sf::Color::Cyan);
    int headerSize = 20;

    // Título General
    titleText.setFont(font);
    titleText.setString("MENU DE COMERCIO");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Cyan);
    // Centramos el título
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
    titleText.setPosition({ 640.f, 50.f });

// --- SECCIÓN IZQUIERDA: JUGADOR ---
    float leftPanelX = 50.f;
    float topY = 100.f;
    float headerY = topY + 60.f;

    playerTableBg.setSize(panelSize);
    playerTableBg.setFillColor(panelFill);
    playerTableBg.setOutlineThickness(3);
    playerTableBg.setOutlineColor(cyanOutline);
    playerTableBg.setPosition({ leftPanelX, topY });

    playerSectionTitle.setFont(font);
    playerSectionTitle.setString("TU INVENTARIO");
    playerSectionTitle.setCharacterSize(25);
    playerSectionTitle.setFillColor(sf::Color::Cyan);
    playerSectionTitle.setPosition({ leftPanelX + 20.f, topY + 15.f });

    playerMoneyText.setFont(font);
    playerMoneyText.setCharacterSize(18);
    playerMoneyText.setFillColor(sf::Color::Yellow);
    playerMoneyText.setPosition({ leftPanelX + 380.f, topY + 22.f });

    // Ajuste de posiciones X relativas a leftPanelX para acomodar más columnas
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
    headerPlayerQuality.setString("CAL.");
    headerPlayerQuality.setCharacterSize(16);
    headerPlayerQuality.setFillColor(cyanOutline);
    headerPlayerQuality.setPosition({ leftPanelX + 330.f, headerY });

    headerPlayerQty.setFont(font);
    headerPlayerQty.setString("CANT.");
    headerPlayerQty.setCharacterSize(16);
    headerPlayerQty.setFillColor(cyanOutline);
    headerPlayerQty.setPosition({ leftPanelX + 400.f, headerY });

    headerPlayerSellPrice.setFont(font);
    headerPlayerSellPrice.setString("VENTA");
    headerPlayerSellPrice.setCharacterSize(16);
    headerPlayerSellPrice.setFillColor(cyanOutline);
    headerPlayerSellPrice.setPosition({ leftPanelX + 470.f, headerY });

// --- SECCIÓN DERECHA: PLANETA ---
    float rightPanelX = 670.f;

    planetTableBg.setSize(panelSize);
    planetTableBg.setFillColor(panelFill);
    planetTableBg.setOutlineThickness(3);
    planetTableBg.setOutlineColor(cyanOutline);
    planetTableBg.setPosition({ rightPanelX, topY });

    planetNameText.setFont(font);
    // El nombre del planeta se actualiza dinámicamente en draw()
    planetNameText.setCharacterSize(25);
    planetNameText.setFillColor(sf::Color::Cyan);
    planetNameText.setPosition({ rightPanelX + 20.f, topY + 15.f });

    // Ajuste de posiciones X relativas a rightPanelX
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
    headerPlanetQuality.setString("CAL.");
    headerPlanetQuality.setCharacterSize(16);
    headerPlanetQuality.setFillColor(cyanOutline);
    headerPlanetQuality.setPosition({ rightPanelX + 330.f, headerY });

    headerPlanetQty.setFont(font);
    headerPlanetQty.setString("CANT.");
    headerPlanetQty.setCharacterSize(16);
    headerPlanetQty.setFillColor(cyanOutline);
    headerPlanetQty.setPosition({ rightPanelX + 400.f, headerY });

    headerPlanetBuyPrice.setFont(font);
    headerPlanetBuyPrice.setString("COMPRA");
    headerPlanetBuyPrice.setCharacterSize(16);
    headerPlanetBuyPrice.setFillColor(cyanOutline);
    headerPlanetBuyPrice.setPosition({ rightPanelX + 470.f, headerY });
    
// --- CONFIGURACIÓN DEL MENU CONTEXTUAL ---
    contextMenuBg.setSize({ 150.f, 100.f });
    contextMenuBg.setFillColor(sf::Color(20, 20, 30, 240)); // Muy opaco
    contextMenuBg.setOutlineThickness(2);
    contextMenuBg.setOutlineColor(sf::Color::Yellow); // Resalta para que se note

    int ctxFontSize = 16;
    ctxBtnInfo.setCharacterSize(ctxFontSize);   ctxBtnInfo.setFillColor(sf::Color::White);
    ctxBtnAction.setCharacterSize(ctxFontSize); ctxBtnAction.setFillColor(sf::Color::White);
    ctxBtnCancel.setCharacterSize(ctxFontSize); ctxBtnCancel.setFillColor(sf::Color::Red);

// --- CONFIGURACIÓN DEL POPUP DE INFO ---
    infoPopupBg.setFillColor(sf::Color(10, 10, 15, 245));
    infoPopupBg.setOutlineThickness(2);
    infoPopupBg.setOutlineColor(sf::Color::Cyan);

    infoPopupText.setCharacterSize(18);
    infoPopupText.setFillColor(sf::Color::White);

    // Configuramos el texto del ESC
    infoPopupEscText.setCharacterSize(14);
    infoPopupEscText.setFillColor(sf::Color(150, 150, 150)); // Un gris discreto

// --- CONFIGURACIÓN DEL TEXTO DE INSTRUCCIONES ---
    infoInputText.setCharacterSize(16);
    infoInputText.setFillColor(sf::Color::White);
    sf::FloatRect inputBounds = infoInputText.getLocalBounds();
    infoInputText.setOrigin({ inputBounds.size.x / 2.f, inputBounds.size.y / 2.f });
    infoInputText.setPosition({ 640.f, 685.f });

// --- CONFIGURACIÓN DEL RESALTADO DE SELECCIÓN ---
    selectionHighlight.setSize({540.f, 35.f}); // Un poco más estrecho que la tabla (560)
    selectionHighlight.setFillColor(sf::Color(0, 255, 255, 60)); // Cian muy transparente

// --- CONFIGURACIÓN DE LOS BOTONES DE ACCIÓN ---
    sf::Vector2f btnSize(150.f, 40.f);
    float btnY = topY + 500.f; // Posición Y en la parte inferior de la tabla

    // Botón Jugador (Vender)
    playerActionBtnBg.setSize(btnSize);
    playerActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gris (Desactivado por defecto)
    playerActionBtnBg.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    playerActionBtnBg.setPosition({leftPanelX + (panelSize.x / 2.f), btnY});
    
    playerActionBtnText.setCharacterSize(16);
    sf::FloatRect pBounds = playerActionBtnText.getLocalBounds();
    playerActionBtnText.setOrigin({pBounds.size.x / 2.f, pBounds.size.y / 2.f});
    playerActionBtnText.setPosition({leftPanelX + (panelSize.x / 2.f), btnY});

    // Botón Planeta (Comprar)
    planetActionBtnBg.setSize(btnSize);
    planetActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gris (Desactivado por defecto)
    planetActionBtnBg.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    planetActionBtnBg.setPosition({rightPanelX + (panelSize.x / 2.f), btnY});
    
    planetActionBtnText.setCharacterSize(16);
    sf::FloatRect mBounds = planetActionBtnText.getLocalBounds();
    planetActionBtnText.setOrigin({mBounds.size.x / 2.f, mBounds.size.y / 2.f});
    planetActionBtnText.setPosition({rightPanelX + (panelSize.x / 2.f), btnY});
}

void TradeMenuUI::draw(sf::RenderWindow& window, const Inventory& playerInv, const Planet& currentPlanet, float playerMoney, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
// CAPA 1: Fondo Oscuro Translucido General
    // window.draw(generalBackground);

// CAPA 2: Planeta Masivo Prominente y Estrella de Fondo (si el planeta tiene sprite)
    if (currentPlanet.hasSprite()) {
        // Obtenemos el sprite orbital base y creamos una copia local
        sf::Sprite detailedPlanetSprite(*currentPlanet.getSprite());

        const sf::Texture& texture = detailedPlanetSprite.getTexture();
        sf::Vector2u texSize = texture.getSize();
        
        // Centramos el origen
        detailedPlanetSprite.setOrigin({ static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f });
        
        // Reescalado masivo al 70% del ancho de pantalla
        float targetWidth = 1280.f * 0.7f; 
        float detailedScale = targetWidth / static_cast<float>(texSize.x);
        detailedPlanetSprite.setScale({ detailedScale, detailedScale });

        // Posicion del planeta
        detailedPlanetSprite.setPosition({ 180.f, 600.f});

        // Ajustamos la opacidad: prominente pero no deslumbrante
        detailedPlanetSprite.setColor(sf::Color(255, 255, 255, 210)); 

        // Dibujamos el planeta masivo detrás de las tablas
        window.draw(detailedPlanetSprite);
    }

// CAPA 3: Interfaz de Usuario (Tablas y Texto)
    window.draw(titleText);

    // --- DIBUJAR JUGADOR (Izquierda) ---
    window.draw(playerTableBg);
    window.draw(playerSectionTitle);
    window.draw(headerPlayerName);
    window.draw(headerPlayerQty);
    window.draw(headerPlayerSellPrice);
    window.draw(headerPlayerCategory);
    window.draw(headerPlayerQuality);

    // Actualizar Dinero del Jugador
    std::stringstream ss;
    ss << "Bs. " << std::fixed << std::setprecision(2) << playerMoney;
    playerMoneyText.setString(ss.str());
    window.draw(playerMoneyText);

    // Dibujar ítems del Jugador (usando scroll lógico)
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

                // Obtener color de rareza
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
                
                sf::Text catT(font, itemData->getCategoryString().substr(0, 8)); // Truncamos si es muy largo
                catT.setCharacterSize(14);
                catT.setFillColor(sf::Color::White);
                catT.setPosition({ 275.f, yPos });
                window.draw(catT);
                
                sf::Text qualT(font, itemData->getRarityString().substr(0, 3)); // Truncamos ("Com", "Rar")
                qualT.setCharacterSize(14);
                qualT.setFillColor(rarityColor);
                qualT.setPosition({ 380.f, yPos });
                window.draw(qualT);

                sf::Text qtyT(font, std::to_string(slot->quantity));
                qtyT.setCharacterSize(14);
                qtyT.setFillColor(sf::Color::White);
                qtyT.setPosition({ 450.f, yPos });
                window.draw(qtyT);
                
                float sellPrice = currentPlanet.getItemPrice(slot->itemID, catalog);
                ss.str(""); ss << "Bs." << std::fixed << std::setprecision(0) << sellPrice; // Quitamos decimales por espacio
                sf::Text priceT(font, ss.str());
                priceT.setCharacterSize(14);
                priceT.setFillColor(sf::Color::White);
                priceT.setPosition({ 520.f, yPos });
                window.draw(priceT);

                drawnCount++;
            }
            currentSlotIndex++;
        }
    }

    // --- DIBUJAR PLANETA (Derecha) ---
    window.draw(planetTableBg);
    // Actualizar Nombre del Planeta
    planetNameText.setString("MERCADO - " + currentPlanet.getName());
    window.draw(planetNameText);
    
    window.draw(headerPlanetName);
    window.draw(headerPlanetQty);
    window.draw(headerPlanetBuyPrice);
    window.draw(headerPlanetCategory);
    window.draw(headerPlanetQuality);

    // Dibujar ítems del Planeta (Mercado local)
    const auto& planetSlots = currentPlanet.getLocalStock(); 
    drawnCount = 0;
    currentSlotIndex = 0;
    
    for (const auto& slot : planetSlots) {
        if (slot.has_value()) {
            if (currentSlotIndex >= planetStartIndex && drawnCount < maxVisibleRows) {
                const auto& itemData = catalog.at(slot->itemID);
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
                
                float buyPrice = currentPlanet.getItemPrice(slot->itemID, catalog);
                ss.str(""); ss << "Bs." << std::fixed << std::setprecision(0) << buyPrice;
                sf::Text priceT(font, ss.str());
                priceT.setCharacterSize(14);
                priceT.setFillColor(sf::Color::White);
                priceT.setPosition({ 1140.f, yPos });
                window.draw(priceT);

                drawnCount++;
            }
            currentSlotIndex++;
        }
    }

    // Dibujamos el texto de instrucciones en la parte inferior
    window.draw(infoInputText);

// --- ACTUALIZAR Y DIBUJAR BOTONES DE ACCION ---
    // Si hay un item del jugador seleccionado, activamos (pintamos de verde) el botón de Venta
    if (selectedItemID != "" && isPlayerItem) {
        playerActionBtnBg.setFillColor(sf::Color(0, 150, 0)); // Verde
    }
    else {
        playerActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gris
    }
    
    // Si hay un item del planeta seleccionado, activamos (pintamos de verde) el botón de Compra
    if (selectedItemID != "" && !isPlayerItem) {
        planetActionBtnBg.setFillColor(sf::Color(0, 150, 0)); // Verde
    }
    else {
        planetActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gris
    }

    // Dibujamos los botones en la pantalla
    window.draw(playerActionBtnBg);
    window.draw(playerActionBtnText);
    window.draw(planetActionBtnBg);
    window.draw(planetActionBtnText);

// CAPA 4: Popup de Información (Top Layer)
    // Nota: Eliminamos la lógica de "showContextMenu" que tenías antes aquí.
    if (showInfoPopup) {
        // Un rectangulito extra semi-transparente para oscurecer el fondo cuando abres la info
        sf::RectangleShape darkOverlay({1280.f, 720.f});
        darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(darkOverlay);

        window.draw(infoPopupBg);
        window.draw(infoPopupText);
        window.draw(infoPopupEscText);
    }
}

void TradeMenuUI::update(const sf::Vector2f& mousePos) {
    // Si el popup de info está abierto, no queremos que las cosas del fondo se iluminen
    if (showInfoPopup) return;

    // Colores para el hover
    sf::Color normalColor = sf::Color::Cyan;
    sf::Color hoverColor = sf::Color::Yellow;

    // --- HOVER CABECERAS JUGADOR ---
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

    // --- HOVER CABECERAS PLANETA ---
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
}

void TradeMenuUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Inventory& playerInv, Planet& currentPlanet, Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    
    // --- DETECCIÓN DE SCROLL ---
    if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
            // Verificamos si el mouse está sobre la tabla del jugador (Izquierda)
            if (playerTableBg.getGlobalBounds().contains(mousePos)) {
                if (mouseWheel->delta > 0) playerStartIndex = std::max(0, playerStartIndex - 1);
                else if (mouseWheel->delta < 0) playerStartIndex++;
            }
            // Verificamos si el mouse está sobre la tabla del planeta (Derecha)
            else if (planetTableBg.getGlobalBounds().contains(mousePos)) {
                if (mouseWheel->delta > 0) planetStartIndex = std::max(0, planetStartIndex - 1);
                else if (mouseWheel->delta < 0) planetStartIndex++;
            }
        }
    }

    // --- FUNCIÓN AUXILIAR MAGICA ---
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

    // --- DETECCIÓN DE CLICS ---
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        float startY = 200.f;
        float rowHeight = 35.f;

        // --- CLIC DERECHO (Abrir Menú de Info) ---
        if (mouseBtn->button == sf::Mouse::Button::Right && !showInfoPopup) {
            if (mousePos.y >= startY && mousePos.y < startY + (maxVisibleRows * rowHeight)) {
                int clickedRow = static_cast<int>((mousePos.y - startY) / rowHeight);

                // Click en Jugador
                if (mousePos.x >= 50.f && mousePos.x <= 610.f) {
                    std::string foundID = getClickedItemID(playerInv.getSlots(), playerStartIndex, clickedRow);
                    if (foundID != "") {
                        selectedItemID = foundID;
                        isPlayerItem = true;
                        
                        const auto& itemData = catalog.at(selectedItemID);
                        std::stringstream ss;
                        ss << "NOMBRE: " << itemData->getName() << "\n\n"
                           << "CATEGORIA: " << itemData->getCategoryString() << "\n\n"
                           << "RAREZA: " << itemData->getRarityString() << "\n\n"
                           << "PRECIO VENTA: Bs. " << std::fixed << std::setprecision(2) << currentPlanet.getItemPrice(selectedItemID, catalog);
                        
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
                // Click en Planeta
                else if (mousePos.x >= 670.f && mousePos.x <= 1230.f) {
                    std::string foundID = getClickedItemID(currentPlanet.getLocalStock(), planetStartIndex, clickedRow);
                    if (foundID != "") {
                        selectedItemID = foundID;
                        isPlayerItem = false;

                        const auto& itemData = catalog.at(selectedItemID);
                        std::stringstream ss;
                        ss << "NOMBRE: " << itemData->getName() << "\n\n"
                           << "CATEGORIA: " << itemData->getCategoryString() << "\n\n"
                           << "RAREZA: " << itemData->getRarityString() << "\n\n"
                           << "PRECIO COMPRA: Bs. " << std::fixed << std::setprecision(2) << currentPlanet.getItemPrice(selectedItemID, catalog);
                        
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

        // --- CLIC IZQUIERDO (Seleccionar, Ordenar o Actuar) ---
        else if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            if (showInfoPopup) {
                showInfoPopup = false;
                return; // Cerramos el popup y no hacemos nada más este frame
            }

            // 1. VERIFICAR CLIC EN CABECERAS DEL JUGADOR (Para Ordenar)
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

            // 2. VERIFICAR CLIC EN CABECERAS DEL PLANETA (Para Ordenar)
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

            // 3. VERIFICAR CLIC EN LOS BOTONES DE ACCIÓN (Comprar/Vender)
            if (selectedItemID != "") {
                if (isPlayerItem && playerActionBtnBg.getGlobalBounds().contains(mousePos)) {
                    // Clic en VENDER
                    TradeManager::sellItem(selectedItemID, player, playerInv, currentPlanet, catalog);
                    return; 
                }
                else if (!isPlayerItem && planetActionBtnBg.getGlobalBounds().contains(mousePos)) {
                    // Clic en COMPRAR
                    TradeManager::buyItem(selectedItemID, player, playerInv, currentPlanet, catalog);
                    return;
                }
            }

            // 4. VERIFICAR SELECCIÓN EN LAS LISTAS
            bool clickedOnItem = false;
            if (mousePos.y >= startY && mousePos.y < startY + (maxVisibleRows * rowHeight)) {
                int clickedRow = static_cast<int>((mousePos.y - startY) / rowHeight);

                // Clic en Jugador
                if (mousePos.x >= 50.f && mousePos.x <= 610.f) {
                    std::string foundID = getClickedItemID(playerInv.getSlots(), playerStartIndex, clickedRow);
                    if (foundID != "") {
                        // Si clicamos el mismo, lo deseleccionamos
                        if (selectedItemID == foundID && isPlayerItem) {
                            selectedItemID = "";
                        } else {
                            selectedItemID = foundID;
                            isPlayerItem = true;
                        }
                        clickedOnItem = true;
                    }
                }
                // Clic en Planeta
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

            // Si hizo clic en la nada, deseleccionar
            if (!clickedOnItem) {
                selectedItemID = "";
            }
        }
    }
}