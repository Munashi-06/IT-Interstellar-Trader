#include "Interface/UpgradeTreeUI.hpp"
#include <iostream>

UpgradeTreeUI::UpgradeTreeUI(const sf::Font& f) : 
    font(f),
    titleText(font, ""),
    textLogistics(font, ""),
    textPropulsion(font, ""),
    textTrading(font, ""),
    tooltipText(font, ""),
    backText(font, ""),
    moneyText(font, "")
    {
    // --- MAIN BACKGROUND ---
    background.setSize({1280.f, 720.f});        
    background.setFillColor(sf::Color(10, 10, 20, 220)); 
    background.setOutlineThickness(0);
    background.setOrigin({640.f, 360.f});
    background.setPosition({640.f, 360.f});

    // --- MAIN TITLE ---
    titleText.setFont(font);
    titleText.setString("SHIP UPGRADES");
    titleText.setCharacterSize(25);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition({150.f, 90.f});

    // --- SETUP TABS ---
    sf::Vector2f tabSize(200.f, 40.f);
    float startX = 350.f;
    float tabY = 90.f;

    // Logistics Tab
    tabLogistics.setSize(tabSize);
    tabLogistics.setPosition({startX, tabY});
    textLogistics.setFont(font); textLogistics.setString("LOGISTICS"); textLogistics.setCharacterSize(16);
    textLogistics.setPosition({startX + 50.f, tabY + 10.f});

    // Propulsion Tab
    tabPropulsion.setSize(tabSize);
    tabPropulsion.setPosition({startX + 210.f, tabY});
    textPropulsion.setFont(font); textPropulsion.setString("PROPULSION"); textPropulsion.setCharacterSize(16);
    textPropulsion.setPosition({startX + 210.f + 50.f, tabY + 10.f});

    // Trading Tab
    tabTrading.setSize(tabSize);
    tabTrading.setPosition({startX + 420.f, tabY});
    textTrading.setFont(font); textTrading.setString("TRADING"); textTrading.setCharacterSize(16);
    textTrading.setPosition({startX + 420.f + 60.f, tabY + 10.f});

    // --- SETUP TOOLTIP ---
    tooltipBg.setFillColor(sf::Color(15, 15, 20, 245)); // Fondo oscuro casi opaco
    tooltipBg.setOutlineThickness(1.f);
    tooltipBg.setOutlineColor(sf::Color::Yellow);
    
    tooltipText.setFont(font);
    tooltipText.setCharacterSize(14);
    tooltipText.setFillColor(sf::Color::White);

    // --- BACK BUTTON ---
    backBtn.setSize({150.f, 40.f});
    backBtn.setFillColor(sf::Color(150, 0, 0));
    backBtn.setPosition({50.f, 660.f});

    backText.setString("BACK");
    backText.setCharacterSize(16);
    backText.setFillColor(sf::Color::White);
    sf::FloatRect bRect = backText.getLocalBounds();
    backText.setOrigin({bRect.size.x / 2.f, bRect.size.y / 2.f});
    backText.setPosition({125.f, 676.f});

    // --- MONEY TEXT ---
    moneyText.setCharacterSize(20);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition({1050.f, 100.f});
}

void UpgradeTreeUI::drawConnection(sf::RenderWindow& window, sf::Vector2f parentPos, sf::Vector2f childPos, sf::Color color) {
    // Draws a line between a parent node and a child node using direct assignment
    sf::Vertex line[2];
    
    line[0].position = parentPos;
    line[0].color = color;
    
    line[1].position = childPos;
    line[1].color = color;
    
    window.draw(line, 2, sf::PrimitiveType::Lines);
}

void UpgradeTreeUI::drawNode(sf::RenderWindow& window, std::shared_ptr<BinNode<Upgrade>> node, sf::Vector2f pos, float hSpacing, float vSpacing) {
    if (!node) return;

    // 1. Draw connections to children FIRST (so lines render behind the boxes)
    if (L(node)) {
        sf::Vector2f leftChildPos(pos.x - hSpacing, pos.y + vSpacing);
        drawConnection(window, pos, leftChildPos, sf::Color(100, 100, 100)); 
        drawNode(window, L(node), leftChildPos, hSpacing * 0.55f, vSpacing); 
    }
    
    if (R(node)) {
        sf::Vector2f rightChildPos(pos.x + hSpacing, pos.y + vSpacing);
        drawConnection(window, pos, rightChildPos, sf::Color(100, 100, 100));
        drawNode(window, R(node), rightChildPos, hSpacing * 0.55f, vSpacing);
    }

    // 2. Draw the Node Box
    sf::RectangleShape nodeBox({140.f, 50.f}); // Slightly wider for text
    nodeBox.setOrigin({70.f, 25.f});
    nodeBox.setPosition(pos);
    nodeBox.setOutlineThickness(2.f);
    
    // Choose color based on the Upgrade Status
    UpgradeStatus status = K(node).status;
    if (status == UpgradeStatus::PURCHASED) {
        nodeBox.setFillColor(sf::Color(0, 100, 0)); // Green
        nodeBox.setOutlineColor(sf::Color::Green);
    } 
    else if (status == UpgradeStatus::AVAILABLE) {
        nodeBox.setFillColor(sf::Color(100, 100, 0)); // Yellow
        nodeBox.setOutlineColor(sf::Color::Yellow);
    } 
    else if (status == UpgradeStatus::LOCKED) {
        nodeBox.setFillColor(sf::Color(40, 40, 40)); // Dark Gray
        nodeBox.setOutlineColor(sf::Color(100, 100, 100));
    } 
    else if (status == UpgradeStatus::BLOCKED_BY_CHOICE) {
        nodeBox.setFillColor(sf::Color(100, 0, 0)); // Dark Red
        nodeBox.setOutlineColor(sf::Color::Red);
    }

    window.draw(nodeBox);

    // 3. Draw the Upgrade Name (Centered Multi-line)
    // Usamos un ancho de 16 caracteres para que encaje lindo en la caja de 140px
    std::string wrappedName = wrapText(K(node).name, 16); 
    std::stringstream ss(wrappedName);
    std::string lineStr;
    
    std::vector<sf::Text> lines;
    float totalHeight = 0.f;
    float lineSpacing = 4.f; // Espaciado vertical entre líneas

    // Desmenuzamos el texto en líneas individuales
    while (std::getline(ss, lineStr, '\n')) {
        sf::Text lineText(font, lineStr);
        lineText.setCharacterSize(13); // Tamaño sutil para que no desborde
        lineText.setFillColor(sf::Color::White);
        
        // Centrar el origen de cada palabra (SFML 3 SYNTAX)
        sf::FloatRect bounds = lineText.getLocalBounds();
        lineText.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
        
        lines.push_back(lineText);
        totalHeight += bounds.size.y + lineSpacing;
    }

    if (lines.size() > 0) totalHeight -= lineSpacing;

    // Calcular el punto Y inicial para que todo el bloque quede en el centro de la caja
    float startY = pos.y - (totalHeight / 2.f);

    // Dibujar cada línea
    for (auto& lineText : lines) {
        sf::FloatRect bounds = lineText.getLocalBounds();
        // Usamos la X central de la caja (pos.x) y bajamos progresivamente en Y
        lineText.setPosition({pos.x, startY + (bounds.size.y / 2.f)});
        window.draw(lineText);
        
        startY += bounds.size.y + lineSpacing;
    }
}

std::shared_ptr<BinNode<Upgrade>> UpgradeTreeUI::getHoveredNode(std::shared_ptr<BinNode<Upgrade>> node, sf::Vector2f pos, float hSpacing, float vSpacing, const sf::Vector2f& mousePos) {
    if (!node) return nullptr;

    // Recreamos el area matemática exacta del nodo (SFML 3 Syntax)
    sf::FloatRect nodeBounds({pos.x - 70.f, pos.y - 25.f}, {140.f, 50.f});

    // Si el mouse está aquí, ¡encontramos el nodo!
    if (nodeBounds.contains(mousePos)) {
        return node;
    }

    // Si no, buscamos en los hijos
    if (L(node)) {
        sf::Vector2f leftChildPos(pos.x - hSpacing, pos.y + vSpacing);
        auto foundLeft = getHoveredNode(L(node), leftChildPos, hSpacing * 0.55f, vSpacing, mousePos);
        if (foundLeft) return foundLeft;
    }
    
    if (R(node)) {
        sf::Vector2f rightChildPos(pos.x + hSpacing, pos.y + vSpacing);
        auto foundRight = getHoveredNode(R(node), rightChildPos, hSpacing * 0.55f, vSpacing, mousePos);
        if (foundRight) return foundRight;
    }

    return nullptr; // No estamos sobre ningún nodo en esta rama
}

std::string UpgradeTreeUI::wrapText(const std::string& text, unsigned maxCharsPerLine) {
    std::string result = "";
    std::string word;
    std::stringstream ss(text);
    unsigned currentLineLength = 0;

    while (ss >> word) {
        // If adding this word exceeds the limit, break the line
        if (currentLineLength + word.length() > maxCharsPerLine) {
            result += "\n";
            currentLineLength = 0;
        } 
        // If it's not the first word of the line, add a space
        else if (currentLineLength > 0) {
            result += " ";
            currentLineLength += 1;
        }
        
        result += word;
        currentLineLength += word.length();
    }
    
    return result;
}

void UpgradeTreeUI::draw(sf::RenderWindow& window, UpgradeManager& manager, float playerMoney) {
    // Update money string dynamically
    moneyText.setString("Bs. " + std::to_string(static_cast<int>(playerMoney)));
    // 1. Draw base panel
    window.draw(titleText);
    window.draw(backBtn);
    window.draw(backText);
    window.draw(moneyText);

    // 2. Highlight the active tab
    sf::Color activeColor(50, 50, 100);
    sf::Color inactiveColor(30, 30, 50);

    tabLogistics.setFillColor(currentTab == 0 ? activeColor : inactiveColor);
    tabPropulsion.setFillColor(currentTab == 1 ? activeColor : inactiveColor);
    tabTrading.setFillColor(currentTab == 2 ? activeColor : inactiveColor);

    window.draw(tabLogistics);  
    window.draw(textLogistics);

    window.draw(tabPropulsion);
    window.draw(textPropulsion);

    window.draw(tabTrading);
    window.draw(textTrading);

    // 3. Get the correct root node based on the selected tab
    std::shared_ptr<BinNode<Upgrade>> currentRoot = nullptr;
    if (currentTab == 0) currentRoot = manager.getLogisticsRoot();
    else if (currentTab == 1) currentRoot = manager.getPropulsionRoot();
    else if (currentTab == 2) currentRoot = manager.getTradingRoot();

    // 4. Start drawing the tree (Recursively)
    if (currentRoot) {
        sf::Vector2f startPos(640.f, 200.f); // Top center of the panel
        float initialHSpacing = 220.f;       // Initial horizontal spread
        float vSpacing = 100.f;              // Vertical distance between levels
        
        drawNode(window, currentRoot, startPos, initialHSpacing, vSpacing);
    }

    // 5. Draw tooltip ON TOP of everything else if active
    if (showTooltip) {
        window.draw(tooltipBg);
        window.draw(tooltipText);
    }
}

// Recursive helper to check if a node was clicked
void UpgradeTreeUI::handleNodeClick(std::shared_ptr<BinNode<Upgrade>> node, std::shared_ptr<BinNode<Upgrade>> sibling, sf::Vector2f pos, float hSpacing, float vSpacing, const sf::Vector2f& mousePos, UpgradeManager& manager, float& playerMoney, std::string& outMessage) {
    if (!node) return;

    sf::FloatRect nodeBounds({pos.x - 70.f, pos.y - 25.f}, {140.f, 50.f});

    if (nodeBounds.contains(mousePos)) {
        // Check the status before attempting to purchase
        if (K(node).status == UpgradeStatus::PURCHASED) {
            outMessage = "You already have this upgrade installed.";
        } 
        else if (K(node).status == UpgradeStatus::LOCKED) {
            outMessage = "Upgrade blocked. You must purchase the prerequisites.";
        } 
        else if (K(node).status == UpgradeStatus::BLOCKED_BY_CHOICE) {
            outMessage = "Incompatible. You chose the opposing branch, and this option has been permanently blocked.";
        } 
        else if (K(node).status == UpgradeStatus::AVAILABLE) {
            if (playerMoney >= K(node).cost) {
                // There is enough money; purchase the upgrade
                manager.purchaseUpgrade(node, sibling, playerMoney);
                std::cout << "[UPGRADE] Successfully purchased: " << K(node).name << "\n";
            } else {
                // No money
                outMessage = "Insufficient funds. You need: " + std::to_string(static_cast<int>(K(node).cost)) + " Bs.";
            }
        }
        return; // Click processed, no further search
    }

    // Search children and pass the outMessage
    if (L(node)) {
        sf::Vector2f leftChildPos(pos.x - hSpacing, pos.y + vSpacing);
        handleNodeClick(L(node), R(node), leftChildPos, hSpacing * 0.55f, vSpacing, mousePos, manager, playerMoney, outMessage);
    }
    if (R(node)) {
        sf::Vector2f rightChildPos(pos.x + hSpacing, pos.y + vSpacing);
        handleNodeClick(R(node), L(node), rightChildPos, hSpacing * 0.55f, vSpacing, mousePos, manager, playerMoney, outMessage);
    }
}
        
std::string UpgradeTreeUI::handleInput(const sf::Event& event, const sf::Vector2f& mousePos, UpgradeManager& manager, float& playerMoney, State& currentState) {
    std::string popupMessage = ""; // Initialize the message as empty

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            // 1. Check Tab Clicks
            if (tabLogistics.getGlobalBounds().contains(mousePos)) currentTab = 0;
            else if (tabPropulsion.getGlobalBounds().contains(mousePos)) currentTab = 1;
            else if (tabTrading.getGlobalBounds().contains(mousePos)) currentTab = 2;

            // 2. Check Node Clicks
            std::shared_ptr<BinNode<Upgrade>> currentRoot = nullptr;
            if (currentTab == 0) currentRoot = manager.getLogisticsRoot();
            else if (currentTab == 1) currentRoot = manager.getPropulsionRoot();
            else if (currentTab == 2) currentRoot = manager.getTradingRoot();

            if (currentRoot) {
                sf::Vector2f startPos(640.f, 200.f); 
                float initialHSpacing = 220.f;       
                float vSpacing = 100.f;              
                
                // We pass our popupMessage variable at the end
                handleNodeClick(currentRoot, nullptr, startPos, initialHSpacing, vSpacing, mousePos, manager, playerMoney, popupMessage);
            }

            // 3. Check Back Button
            if (backBtn.getGlobalBounds().contains(mousePos)) {
                currentState = State::ShipMenu;
                return "";
            }
        }
    }
    return popupMessage; // We return whatever happened
}

void UpgradeTreeUI::update(const sf::Vector2f& mousePos, UpgradeManager& manager) {
    // 1. Tab hover effect (The code you already had)
    sf::Color hoverColor(70, 70, 120);
    sf::Color activeColor(50, 50, 100);
    sf::Color inactiveColor(30, 30, 50);
    if (backBtn.getGlobalBounds().contains(mousePos)) backBtn.setFillColor(sf::Color(200, 0, 0));
    else backBtn.setFillColor(sf::Color(150, 0, 0));

    if (currentTab != 0) tabLogistics.setFillColor(tabLogistics.getGlobalBounds().contains(mousePos) ? hoverColor : inactiveColor);
    if (currentTab != 1) tabPropulsion.setFillColor(tabPropulsion.getGlobalBounds().contains(mousePos) ? hoverColor : inactiveColor);
    if (currentTab != 2) tabTrading.setFillColor(tabTrading.getGlobalBounds().contains(mousePos) ? hoverColor : inactiveColor);

    // 2. Tooltip Logic (Information Popup)
    showTooltip = false; // We hide it by default on every frame

    std::shared_ptr<BinNode<Upgrade>> currentRoot = nullptr;
    if (currentTab == 0) currentRoot = manager.getLogisticsRoot();
    else if (currentTab == 1) currentRoot = manager.getPropulsionRoot();
    else if (currentTab == 2) currentRoot = manager.getTradingRoot();

    if (currentRoot) {
        sf::Vector2f startPos(640.f, 200.f); 
        float initialHSpacing = 220.f;       
        float vSpacing = 100.f;              
        
        auto hoveredNode = getHoveredNode(currentRoot, startPos, initialHSpacing, vSpacing, mousePos);
        
        if (hoveredNode) {
            showTooltip = true;
            
            // Build the text to be displayed
            std::stringstream ss;
            
            // We use our wrapText function with a limit of 45 characters
            ss << K(hoveredNode).name << "\n\n"
               << wrapText(K(hoveredNode).description, 45) << "\n\n"
               << "Cost: Bs. " << K(hoveredNode).cost << "\n"
               << "Status: ";
            
            // Display the current status
            switch(K(hoveredNode).status) {
                case UpgradeStatus::AVAILABLE: ss << "AVAILABLE"; break;
                case UpgradeStatus::PURCHASED: ss << "PURCHASED"; break;
                case UpgradeStatus::LOCKED: ss << "LOCKED"; break;
                case UpgradeStatus::BLOCKED_BY_CHOICE: ss << "BLOCKED (Mutually Exclusive)"; break;
            }

            tooltipText.setString(ss.str());

            // DYNAMIC SIZE ADJUSTMENT
            sf::FloatRect textBounds = tooltipText.getLocalBounds();
            // We add 15 pixels of padding on each side
            tooltipBg.setSize({textBounds.size.x + 30.f, textBounds.size.y + 30.f});

            // POSITION ADJUSTMENT (Follows the mouse)
            sf::Vector2f tooltipPos = mousePos + sf::Vector2f(15.f, 15.f); // Slightly below and to the right of the cursor
            
            // Prevent it from going off-screen to the right or bottom
            if (tooltipPos.x + tooltipBg.getSize().x > 1280.f) {
                tooltipPos.x = mousePos.x - tooltipBg.getSize().x - 10.f;
            }
            if (tooltipPos.y + tooltipBg.getSize().y > 720.f) {
                tooltipPos.y = mousePos.y - tooltipBg.getSize().y - 10.f;
            }

            tooltipBg.setPosition(tooltipPos);
            // The text is positioned based on the box plus the padding
            tooltipText.setPosition(tooltipPos + sf::Vector2f(15.f, 15.f));
        }
    }
}