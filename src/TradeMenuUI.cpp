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
      infoInputText(f, "Use el Scroll del Mouse para navegar por los objetos, RMB para mas informacion, LMB para seleccionar, Esc. para salir"),
      playerActionBtnText(f, "VENDER"),
      planetActionBtnText(f, "COMPRAR")
{
// --- LAYOUT CONSTANTS (Ajustes visuales) ---
    sf::Vector2f panelSize(560.f, 550.f);
    sf::Color panelFill(0, 0, 0, 180); // Muy oscuro, casi opaco para legibilidad
    sf::Color cyanOutline(sf::Color::Cyan);
    int headerSize = 20;

    // 1. Fondo Oscuro Translucido General (Fullscreen)
    generalBackground.setSize({ 1280.f, 720.f });
    generalBackground.setFillColor(sf::Color(0, 0, 15)); // Oscurecemos la escena de fondo

    // 2. Título General
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

    headerPlayerName.setFont(font);    headerPlayerName.setString("ITEM");     headerPlayerName.setCharacterSize(headerSize); headerPlayerName.setFillColor(cyanOutline); headerPlayerName.setPosition({ leftPanelX + 20.f, headerY });
    headerPlayerQty.setFont(font);     headerPlayerQty.setString("CANT.");      headerPlayerQty.setCharacterSize(headerSize); headerPlayerQty.setFillColor(cyanOutline); headerPlayerQty.setPosition({ leftPanelX + 310.f, headerY });
    headerPlayerSellPrice.setFont(font); headerPlayerSellPrice.setString("VENTA"); headerPlayerSellPrice.setCharacterSize(headerSize); headerPlayerSellPrice.setFillColor(cyanOutline); headerPlayerSellPrice.setPosition({ leftPanelX + 430.f, headerY });


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

    headerPlanetName.setFont(font);    headerPlanetName.setString("ITEM");     headerPlanetName.setCharacterSize(headerSize); headerPlanetName.setFillColor(cyanOutline); headerPlanetName.setPosition({ rightPanelX + 20.f, headerY });
    headerPlanetQty.setFont(font);     headerPlanetQty.setString("CANT.");      headerPlanetQty.setCharacterSize(headerSize); headerPlanetQty.setFillColor(cyanOutline); headerPlanetQty.setPosition({ rightPanelX + 310.f, headerY });
    headerPlanetBuyPrice.setFont(font); headerPlanetBuyPrice.setString("COMPRA"); headerPlanetBuyPrice.setCharacterSize(headerSize); headerPlanetBuyPrice.setFillColor(cyanOutline); headerPlanetBuyPrice.setPosition({ rightPanelX + 430.f, headerY });

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
    float btnY = topY + 450.f; // Posición Y en la parte inferior de la tabla

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
    window.draw(generalBackground);

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

                // --- NUEVO: Dibujar el resaltado si este ítem está seleccionado ---
                if (selectedItemID == slot->itemID && isPlayerItem) {
                    selectionHighlight.setPosition({60.f, yPos}); // Ajustado a la tabla izquierda
                    window.draw(selectionHighlight);
                }

                // Dibujamos Nombre
                sf::Text nameT(font, itemData->getName()); nameT.setCharacterSize(16); nameT.setFillColor(sf::Color::White); nameT.setPosition({ 70.f, yPos }); window.draw(nameT);
                // Dibujamos Cantidad
                sf::Text qtyT(font, std::to_string(slot->quantity)); qtyT.setCharacterSize(16); qtyT.setFillColor(sf::Color::White); qtyT.setPosition({ 360.f, yPos }); window.draw(qtyT);
                
                // Dibujamos Precio Venta (Usamos el Market Price del planeta)
                float sellPrice = currentPlanet.getItemPrice(slot->itemID, catalog);
                ss.str(""); ss << "Bs. " << std::fixed << std::setprecision(2) << sellPrice;
                sf::Text priceT(font, ss.str()); priceT.setCharacterSize(16); priceT.setFillColor(sf::Color::White); priceT.setPosition({ 480.f, yPos }); window.draw(priceT);

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

    // Dibujar ítems del Planeta (Mercado local)
    const auto& planetSlots = currentPlanet.getLocalStock(); 
    drawnCount = 0;
    currentSlotIndex = 0;
    
    for (const auto& slot : planetSlots) {
        if (slot.has_value()) {
            if (currentSlotIndex >= planetStartIndex && drawnCount < maxVisibleRows) {
                const auto& itemData = catalog.at(slot->itemID);
                float yPos = startY + (drawnCount * rowHeight);

                // --- NUEVO: Dibujar el resaltado si este ítem está seleccionado ---
                if (selectedItemID == slot->itemID && !isPlayerItem) {
                    selectionHighlight.setPosition({680.f, yPos}); // Ajustado a la tabla derecha
                    window.draw(selectionHighlight);
                }

                // Dibujamos Nombre
                sf::Text nameT(font, itemData->getName()); nameT.setCharacterSize(16); nameT.setFillColor(sf::Color::White); nameT.setPosition({ 690.f, yPos }); window.draw(nameT);
                // Dibujamos Cantidad
                sf::Text qtyT(font, std::to_string(slot->quantity)); qtyT.setCharacterSize(16); qtyT.setFillColor(sf::Color::White); qtyT.setPosition({ 980.f, yPos }); window.draw(qtyT);
                
                // Dibujamos Precio Compra (Usamos el Market Price del planeta)
                float buyPrice = currentPlanet.getItemPrice(slot->itemID, catalog);
                ss.str(""); ss << "Bs. " << std::fixed << std::setprecision(2) << buyPrice;
                sf::Text priceT(font, ss.str()); priceT.setCharacterSize(16); priceT.setFillColor(sf::Color::White); priceT.setPosition({ 1100.f, yPos }); window.draw(priceT);

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
    } else {
        playerActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Gris
    }
    
    // Si hay un item del planeta seleccionado, activamos (pintamos de verde) el botón de Compra
    if (selectedItemID != "" && !isPlayerItem) {
        planetActionBtnBg.setFillColor(sf::Color(0, 150, 0)); // Verde
    } else {
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
void TradeMenuUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, Inventory& playerInv, Planet& currentPlanet, Player& player, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    
    // --- DETECCIÓN DE SCROLL ---
    if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
            
            // Verificamos si el mouse está sobre la tabla del jugador (Izquierda)
            if (playerTableBg.getGlobalBounds().contains(mousePos)) {
                if (mouseWheel->delta > 0) {
                    playerStartIndex = std::max(0, playerStartIndex - 1);
                }
                else if (mouseWheel->delta < 0) {
                    playerStartIndex++;
                } // Luego le pondremos límite máximo
            }
            // Verificamos si el mouse está sobre la tabla del planeta (Derecha)
            else if (planetTableBg.getGlobalBounds().contains(mousePos)) {
                if (mouseWheel->delta > 0) {
                    planetStartIndex = std::max(0, planetStartIndex - 1);
                }
                else if (mouseWheel->delta < 0) {
                    planetStartIndex++;
                } // Luego le pondremos límite máximo
            }
        }
    }

    // --- DETECCIÓN DE CLICS ---
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        float startY = 200.f;
        float rowHeight = 35.f;

        // --- CLIC DERECHO (Abrir Menú de Info) ---
        if (mouseBtn->button == sf::Mouse::Button::Right && !showInfoPopup) {
            if (mousePos.y >= startY && mousePos.y < startY + (maxVisibleRows * rowHeight)) {
                int clickedRow = static_cast<int>((mousePos.y - startY) / rowHeight);

                // Clic en Jugador
                if (mousePos.x >= 50.f && mousePos.x <= 610.f) {
                    int actualIndex = playerStartIndex + clickedRow;
                    const auto& slots = playerInv.getSlots();
                    if (actualIndex < slots.size() && slots[actualIndex].has_value()) {
                        selectedItemID = slots[actualIndex]->itemID;
                        // Ya no usamos el menu contextual pequeño, abrimos directo el popup de info
                        // (Copia aquí la lógica de armado de string que tenías antes para la info)
                        const auto& itemData = catalog.at(selectedItemID);
                        std::stringstream ss;
                        ss << "NOMBRE: " << itemData->getName() << "\n\n"
                           << "CATEGORIA: " << itemData->getCategoryString() << "\n\n"
                           << "RAREZA: " << itemData->getRarityString() << "\n\n"
                           << "PRECIO VENTA: Bs. " << std::fixed << std::setprecision(2) << currentPlanet.getItemPrice(selectedItemID, catalog);
                        
                        infoPopupText.setString(ss.str());
                        // Lógica de centrado del popup que ya tienes... (calculo dinamico)
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
                // Clic en Planeta
                else if (mousePos.x >= 670.f && mousePos.x <= 1230.f) {
                    int actualIndex = planetStartIndex + clickedRow;
                    const auto& slots = currentPlanet.getLocalStock();
                    if (actualIndex < slots.size() && slots[actualIndex].has_value()) {
                        selectedItemID = slots[actualIndex]->itemID;
                        const auto& itemData = catalog.at(selectedItemID);
                        std::stringstream ss;
                        ss << "NOMBRE: " << itemData->getName() << "\n\n"
                           << "CATEGORIA: " << itemData->getCategoryString() << "\n\n"
                           << "RAREZA: " << itemData->getRarityString() << "\n\n"
                           << "PRECIO COMPRA: Bs. " << std::fixed << std::setprecision(2) << currentPlanet.getItemPrice(selectedItemID, catalog);
                        
                        infoPopupText.setString(ss.str());
                        // Lógica de centrado...
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

        // --- CLIC IZQUIERDO (Seleccionar o Actuar) ---
        else if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            if (showInfoPopup) {
                showInfoPopup = false;
                return;
            }

            // 1. Verificar clic en los botones de acción
            if (selectedItemID != "") {
                if (isPlayerItem && playerActionBtnBg.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Vendiendo: " << selectedItemID << std::endl;
                    // Lógica de venta
                    return; // Importante para no deseleccionar justo después
                }
                else if (!isPlayerItem && planetActionBtnBg.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Comprando: " << selectedItemID << std::endl;
                    // Lógica de compra
                    return;
                }
            }

            // 2. Verificar selección en las listas
            bool clickedOnItem = false;
            if (mousePos.y >= startY && mousePos.y < startY + (maxVisibleRows * rowHeight)) {
                int clickedRow = static_cast<int>((mousePos.y - startY) / rowHeight);

                // Clic en Jugador
                if (mousePos.x >= 50.f && mousePos.x <= 610.f) {
                    int actualIndex = playerStartIndex + clickedRow;
                    const auto& slots = playerInv.getSlots();
                    if (actualIndex < slots.size() && slots[actualIndex].has_value()) {
                        // Si clicamos el mismo, lo deseleccionamos
                        if (selectedItemID == slots[actualIndex]->itemID && isPlayerItem) {
                            selectedItemID = "";
                        } else {
                            selectedItemID = slots[actualIndex]->itemID;
                            isPlayerItem = true;
                        }
                        clickedOnItem = true;
                    }
                }
                // Clic en Planeta
                else if (mousePos.x >= 670.f && mousePos.x <= 1230.f) {
                    int actualIndex = planetStartIndex + clickedRow;
                    const auto& slots = currentPlanet.getLocalStock();
                    if (actualIndex < slots.size() && slots[actualIndex].has_value()) {
                         if (selectedItemID == slots[actualIndex]->itemID && !isPlayerItem) {
                            selectedItemID = "";
                        } else {
                            selectedItemID = slots[actualIndex]->itemID;
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
