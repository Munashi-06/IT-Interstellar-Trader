#include "Interface/Popup.hpp"
#include <algorithm>

Popup::Popup(const sf::Font& f) :
    active(false),
    autoCloseTimer(0.f),
    font(f),
    promptText(f, "")
{
    // Set the basic styles for the box (the size will be calculated in the show method)
    background.setFillColor(sf::Color(20, 20, 30, 240));
    background.setOutlineThickness(3);
    background.setOutlineColor(sf::Color::Cyan);

    // Help text below
    promptText.setString("Press ENTER or CLICK to continue");
    promptText.setCharacterSize(14);
    promptText.setFillColor(sf::Color::Yellow);
    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin({promptBounds.size.x / 2.f, promptBounds.size.y / 2.f});
}

void Popup::show(const std::string& message, float timeToClose) {
    // 1. AUTO-FORMAT: Replace “. ” with “.\n” and “: ” with “:\n”
    std::string processedMessage = message;

    // Replace periods
    size_t pos = 0;
    while ((pos = processedMessage.find(". ", pos)) != std::string::npos) {
        processedMessage.replace(pos, 2, ".\n");
        pos += 2; 
    }

    // Replace colons
    pos = 0;
    while ((pos = processedMessage.find(": ", pos)) != std::string::npos) {
        processedMessage.replace(pos, 2, ":\n");
        pos += 2; 
    }

    // 2. CREATE CENTERED LINES
    messageLines.clear();
    std::stringstream ss(processedMessage);
    std::string lineStr;
    
    float totalHeight = 0.f;
    float maxWidth = 0.f;
    float lineSpacing = 10.f;

    while (std::getline(ss, lineStr, '\n')) {
        sf::Text lineText(font, lineStr, 20);
        lineText.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = lineText.getLocalBounds();
        // We center the exact origin of this individual line
        lineText.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
        
        if (bounds.size.x > maxWidth) maxWidth = bounds.size.x;
        
        messageLines.push_back(lineText);
        totalHeight += bounds.size.y + lineSpacing;
    }
    if (!messageLines.empty()) totalHeight -= lineSpacing; // Remove the extra space at the end

    // 3. CALCULATE THE DYNAMIC SIZE OF THE BOX
    // We add padding of 80px to the width and 100px to the height.
    // If the text is very short, we maintain a minimum size of 400x150 so it doesn't look ridiculous.
    float boxWidth = std::max(maxWidth + 80.f, 400.f);
    float boxHeight = std::max(totalHeight + 100.f, 150.f);
    
    // If it doesn't close on its own, we need extra space for the “Press ENTER...” prompt
    if (timeToClose <= 0.f) boxHeight += 40.f;

    background.setSize({boxWidth, boxHeight});
    background.setOrigin({boxWidth / 2.f, boxHeight / 2.f});
    background.setPosition({640.f, 360.f}); // Always centered on the screen

    // 4. POSITION THE LINES OF TEXT
    float startY = 360.f - (totalHeight / 2.f);
    if (timeToClose <= 0.f) startY -= 15.f; // Move the text up slightly if “Press ENTER” is displayed

    for (auto& lineText : messageLines) {
        sf::FloatRect bounds = lineText.getLocalBounds();
        lineText.setPosition({640.f, startY + (bounds.size.y / 2.f)});
        startY += bounds.size.y + lineSpacing;
    }

    // 5. POSITION THE HELP TEXT (Bottom center of the dynamic box)
    promptText.setPosition({640.f, 360.f + (boxHeight / 2.f) - 25.f});

    autoCloseTimer = timeToClose;
    active = true;
}

void Popup::update(float dt) {
    if (!active) return;
    if (autoCloseTimer > 0.f) {
        autoCloseTimer -= dt;
        if (autoCloseTimer <= 0.f) hide();
    }
}

void Popup::draw(sf::RenderWindow& window) {
    if (!active) return;
    
    sf::RectangleShape darkOverlay({1280.f, 720.f});
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    
    window.draw(darkOverlay);
    window.draw(background);
    
    for (const auto& line : messageLines) {
        window.draw(line);
    }
    
    if (autoCloseTimer <= 0.f) {
        window.draw(promptText);
    }
}

void Popup::hide() { active = false; }

void Popup::handleInput(sf::Keyboard::Key key) {
    if (active && autoCloseTimer <= 0.f) {
        if (key == sf::Keyboard::Key::Enter || key == sf::Keyboard::Key::Escape || key == sf::Keyboard::Key::Space) {
            hide();
        }
    }
}

void Popup::handleMouseClick(const sf::Vector2f& mousePos) {
    if (active && autoCloseTimer <= 0.f) hide();
}