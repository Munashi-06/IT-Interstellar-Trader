#include "ShipMenuUI.hpp"

ShipMenuUI::ShipMenuUI(const sf::Font& f, const sf::Texture& shipTex) 
    : font(f),                          // Inicializamos la referencia a la fuente
      shipPreview(shipTex),              // Sprite NECESITA la textura ahora
      titleText(f, ""),                  // Text NECESITA la fuente y un string inicial
      escText(f, ""),
      upgradeText(f, ""),
      headerName(f, "NOMBRE"),           // Podemos pasarle el texto de una vez
      headerCategory(f, "CATEGORIA"),
      headerQuality(f, "CALIDAD"),
      headerPrice(f, "PRECIO") 
{
    float tableStartX = 450.f;
    float headerY = 200.f;
    int fontSize = 18;

    // 1. Fondo del 80% (1024x576) y fondo de la tabla
    background.setSize({1024.f, 576.f});
    background.setFillColor(sf::Color(15, 15, 25, 220)); // Un poco más oscuro
    background.setOutlineThickness(3);
    background.setOutlineColor(sf::Color::Cyan);
    background.setOrigin({512.f, 288.f});
    background.setPosition({640.f, 360.f}); // Centro de pantalla.

    // Fondo específico para la tabla, para que resalte más
    tableBackground.setSize({600.f, 400.f});
    tableBackground.setFillColor(sf::Color(20, 20, 30, 200));
    tableBackground.setPosition({tableStartX - 12.f, headerY - 5.f}); // Posicionado para cubrir la zona de la tabla a la derecha
    tableBackground.setOutlineThickness(1);
    tableBackground.setOutlineColor(sf::Color::White);

    // 2. Nave en la parte izquierda
    shipPreview.setTexture(shipTex);
    shipPreview.setScale({0.5f, 0.5f}); // Más pequeña
    // Posicionada relativa a la ventana
    shipPreview.setPosition({150.f, 90.f});

    // --- CONFIGURACIÓN DEL BOTÓN DE MEJORA ---
    upgradeBtn.setSize({200.f, 40.f});
    upgradeBtn.setFillColor(sf::Color(0, 150, 0));
    upgradeBtn.setPosition({170.f, 450.f}); // Ubicado debajo de la nave

    upgradeText.setString("SUBIR DE NIVEL");
    upgradeText.setCharacterSize(18);
    upgradeText.setFillColor(sf::Color::White);
    
    // Centramos el texto matemáticamente dentro del botón
    sf::FloatRect upRect = upgradeText.getLocalBounds();
    upgradeText.setOrigin({upRect.size.x / 2.f, upRect.size.y / 2.f});
    upgradeText.setPosition({(tableStartX - 280.f) + 100.f, 450.f + 15.f}); // X + mitad ancho, Y + mitad alto

    // 3. Título general
    titleText.setFont(font);
    titleText.setString("ESTADO DE LA NAVE E INVENTARIO");
    titleText.setCharacterSize(25);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition({400.f, 100.f}); // Arriba al centro

    // 4. Cabeceras de la Tabla (Lado derecho)
    headerName.setFont(font);
    headerName.setString("NOMBRE");
    headerName.setPosition({tableStartX, headerY});
    headerName.setCharacterSize(fontSize);

    headerCategory.setFont(font);
    headerCategory.setString("CATEGORIA");
    headerCategory.setPosition({tableStartX + 220.f, headerY});
    headerCategory.setCharacterSize(fontSize);

    headerQuality.setFont(font);
    headerQuality.setString("CALIDAD");
    headerQuality.setPosition({tableStartX + 370.f, headerY});
    headerQuality.setCharacterSize(fontSize);

    headerPrice.setFont(font);
    headerPrice.setString("PRECIO");
    headerPrice.setPosition({tableStartX + 490.f, headerY});
    headerPrice.setCharacterSize(fontSize);
    
    // 5. Configurar la barra de scroll visual (A la derecha de la tabla)
    scrollTrack.setSize({10.f, 400.f}); // Altura total de la lista
    scrollTrack.setFillColor(sf::Color(50, 50, 50, 150));
    scrollTrack.setPosition({tableStartX + 600.f, headerY - 5.f}); // A la derecha de la tabla, alineado con el header

    scrollThumb.setSize({10.f, 50.f}); // Se ajustará dinámicamente luego
    scrollThumb.setFillColor(sf::Color::Cyan);
    scrollThumb.setPosition({tableStartX + 600.f, headerY - 5.f}); // A la derecha de la tabla, alineado con el header

    // (Ajustar tamaños de letra y colores de las cabeceras aquí)
}

void ShipMenuUI::draw(sf::RenderWindow& window, const Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    // Dibujar fondos, nave, botones y cabeceras
    window.draw(background);
    window.draw(shipPreview);
    window.draw(titleText);
    window.draw(tableBackground);
    window.draw(upgradeBtn);
    window.draw(upgradeText);
    // ... dibujar cabeceras ...
    window.draw(headerName);
    window.draw(headerCategory);
    window.draw(headerQuality);
    window.draw(headerPrice);

    // --- DIBUJAR LA TABLA CON SCROLL ---
    const auto& slots = inventory.getSlots(); 
    
    // Contamos cuántos ítems reales tenemos para ajustar el scroll
    int totalItems = 0;
    for (const auto& slot : slots) {
        if (slot.has_value()) totalItems++;
    }

    // Dibujar ítems basándonos en el startIndex
    float startY = 240.f;
    float rowHeight = 35.f;
    int itemsDrawn = 0;
    int currentItemIndex = 0;

    for (const auto& slot : slots) {
        if (slot.has_value()) {
            // Solo dibujamos si está dentro del rango visible del scroll
            if (currentItemIndex >= startIndex && itemsDrawn < maxVisibleItems) {
                const auto& itemData = catalog.at(slot->itemID);

                sf::Text rowText(font, itemData->getName());
                sf::Text categoryText(font, itemData->getCategoryString());
                sf::Text qualityText(font, itemData->getRarityString());
                
                // Precio mostrando nada mas 2 decimales
                std::string price = std::to_string(itemData->getPrice());
                price = price.substr(0, price.find(".") + 3); // Truncar a 2 decimales
                price = "Bs. " + price; // Agregar prefijo de moneda
                sf::Text priceText(font, price);

                sf::Color rarityColor;
                switch (itemData->getRarity()) {
                    case Rarity::Common: rarityColor = sf::Color::White; break;
                    case Rarity::Rare: rarityColor = sf::Color::Green; break;
                    case Rarity::Exotic: rarityColor = sf::Color::Blue; break;
                    case Rarity::Legendary: rarityColor = sf::Color(128, 0, 128); break; // Morado
                    case Rarity::Quest: rarityColor = sf::Color(255, 215, 0); break; // Dorado
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

                categoryText.setPosition({450.f + 220.f, yPos});
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

    // Actualizar posición de la barra de scroll (Thumb)
    if (totalItems > maxVisibleItems) {
        float scrollPercent = (float)startIndex / (totalItems - maxVisibleItems);
        float thumbY = 195.f + (scrollPercent * (400.f - scrollThumb.getSize().y));
        scrollThumb.setPosition({450.f + 600.f, thumbY});
        
        window.draw(scrollTrack);
        window.draw(scrollThumb);
    }
}

void ShipMenuUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, int totalItems, Inventory& inventory, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    // Detectar scroll del ratón
    if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
            if (mouseWheel->delta > 0) {
                // Scroll arriba
                startIndex--;
            } else if (mouseWheel->delta < 0) {
                // Scroll abajo
                startIndex++;
            }

            // Limitar el scroll para no salirnos del array
            if (startIndex < 0) startIndex = 0;
            int maxStart = std::max(0, totalItems - maxVisibleItems);
            if (startIndex > maxStart) startIndex = maxStart;
        }
    }

    // Detectar clics del ratón
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            // Clic en cabecera NOMBRE
            if (headerName.getGlobalBounds().contains(mousePos)) {
                // Si ya estábamos ordenando por nombre, invertimos el orden (Ascendente/Descendente)
                if (currentSort == SortColumn::Name) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Name; sortAscending = true; }
                inventory.sortByName(sortAscending, catalog);
            }
            
            // Clic en cabecera CATEGORIAsa
            else if (headerCategory.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Category) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Category; sortAscending = true; }
                inventory.sortByCategory(sortAscending, catalog);
            }
            
            // Clic en cabecera CALIDAD
            else if (headerQuality.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Quality) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Quality; sortAscending = true; }
                inventory.sortByQuality(sortAscending, catalog);
            }
            
            // Clic en cabecera PRECIO
            else if (headerPrice.getGlobalBounds().contains(mousePos)) {
                if (currentSort == SortColumn::Price) sortAscending = !sortAscending;
                else { currentSort = SortColumn::Price; sortAscending = true; }
                inventory.sortByPrice(sortAscending, catalog);
            }
        }
    }

}

void ShipMenuUI::update(const sf::Vector2f& mousePos) {
    // --- HOVER PARA BOTÓN DE MEJORA ---
    if (upgradeBtn.getGlobalBounds().contains(mousePos)) {
        upgradeBtn.setFillColor(sf::Color(0, 200, 0));
    } else {
        upgradeBtn.setFillColor(sf::Color(0, 150, 0));
    }

    // --- HOVER PARA CABECERAS ---
    // Color normal (ej. Blanco) y color resaltado (ej. Amarillo)
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