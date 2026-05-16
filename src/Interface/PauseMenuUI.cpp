#include "Interface/PauseMenuUI.hpp"

PauseMenuUI::PauseMenuUI(const sf::Font& font) :
    titleText(font, "")

{
    selectedItemIndex = 0;

    // Semi-transparent dark background
    overlay.setSize({1280.f, 720.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    // Tittle
    titleText.setString("PAUSE");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Cyan);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titleText.setPosition({640.f, 200.f});

    // Options
    std::vector<std::string> options = {"CONTINUE", "OPTIONS", "SAVE", "EXIT TO MENU"};

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(font, options[i], 20);
        sf::FloatRect bounds = text.getLocalBounds();
        
        // Minor visual adjustment (+ 6.f) to perfectly center SFML text vertically
        text.setOrigin({bounds.size.x / 2.f, (bounds.size.y / 2.f) + 6.f});
        
        // Set both Text and Button to the EXACT same Y position (280.f)
        text.setPosition({640.f, 280.f + (i * 70.f)});
        menuTexts.push_back(text);

        sf::RectangleShape btn({300.f, 50.f});
        btn.setOrigin({150.f, 25.f});
        btn.setPosition({640.f, 280.f + (i * 70.f)}); // <--- FIXED ALIGNMENT
        btn.setFillColor(sf::Color(20, 20, 20, 200));
        btn.setOutlineThickness(2);
        buttonBgs.push_back(btn);
    }
    
    // Color the first one
    menuTexts[0].setFillColor(sf::Color::Yellow);
    buttonBgs[0].setOutlineColor(sf::Color::Cyan);
}

void PauseMenuUI::draw(sf::RenderWindow& window) {
    window.draw(overlay);
    window.draw(titleText);
    for (size_t i = 0; i < menuTexts.size(); ++i) {
        window.draw(buttonBgs[i]);
        window.draw(menuTexts[i]);
    }
}

void PauseMenuUI::moveUp() {
    menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
    buttonBgs[selectedItemIndex].setOutlineColor(sf::Color::White);
    selectedItemIndex = (selectedItemIndex - 1 + menuTexts.size()) % menuTexts.size();
    menuTexts[selectedItemIndex].setFillColor(sf::Color::Yellow);
    buttonBgs[selectedItemIndex].setOutlineColor(sf::Color::Cyan);
}

void PauseMenuUI::moveDown() {
    menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
    buttonBgs[selectedItemIndex].setOutlineColor(sf::Color::White);
    selectedItemIndex = (selectedItemIndex + 1) % menuTexts.size();
    menuTexts[selectedItemIndex].setFillColor(sf::Color::Yellow);
    buttonBgs[selectedItemIndex].setOutlineColor(sf::Color::Cyan);
}

std::string PauseMenuUI::getSelectedOption() const {
    return menuTexts[selectedItemIndex].getString();
}

bool PauseMenuUI::updateHover(const sf::Vector2f& mousePos) {
    bool changed = false;
    for (size_t i = 0; i < buttonBgs.size(); ++i) {
        if (buttonBgs[i].getGlobalBounds().contains(mousePos)) {
            if (selectedItemIndex != static_cast<int>(i)) {
                menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
                buttonBgs[selectedItemIndex].setOutlineColor(sf::Color::White);
                selectedItemIndex = i;
                menuTexts[selectedItemIndex].setFillColor(sf::Color::Yellow);
                buttonBgs[selectedItemIndex].setOutlineColor(sf::Color::Cyan);
                changed = true;
            }
            return changed;
        }
    }
    return changed;
}

void PauseMenuUI::handleMouseClick(const sf::Vector2f& mousePos) {
    updateHover(mousePos);
}