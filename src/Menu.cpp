#include "Menu.hpp"

Menu::Menu(float width, float height) {
    if (!font.openFromFile("assets/fonts/04B_03__.TTF")) {
        std::cerr << "Error cargando la fuente" << std::endl;
    }

    std::vector<std::string> labels = {"START", "SETTINGS", "EXIT"};
    
    for (size_t i = 0; i < labels.size(); ++i) {
        float bWidth = 330.f;
        float bHeight = 70.f;
        float xPos = (width / 2.f) - (bWidth / 2.f);
        float yPos = (height / 2.f) + (i * 100.f);

        options.emplace_back(labels[i], sf::Vector2f{bWidth, bHeight}, sf::Vector2f{xPos, yPos}, font);
    }
    options[0].selected = true;
}

void Menu::draw(sf::RenderWindow& window) {
    for (auto& btn : options) {
        btn.draw(window);
    }
}

void Menu::moveUp() {
    options[selectedItemIndex].selected = false;
    selectedItemIndex = (selectedItemIndex - 1 + options.size()) % options.size();
    options[selectedItemIndex].selected = true;
}

void Menu::moveDown() {
    options[selectedItemIndex].selected = false;
    selectedItemIndex = (selectedItemIndex + 1) % options.size();
    options[selectedItemIndex].selected = true;
}

void Menu::updateHover(sf::Vector2f mousePos) {
    static sf::Vector2f lastMousePos = {0, 0};
    
    // Solo procesamos el hover si el mouse se ha movido físicamente
    if (mousePos != lastMousePos) {
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i].shape.getGlobalBounds().contains(mousePos)) {
                options[selectedItemIndex].selected = false;
                selectedItemIndex = i;
                options[selectedItemIndex].selected = true;
            }
        }
        lastMousePos = mousePos;
    }
}

std::string Menu::getSelectedOption() const {
    return options[selectedItemIndex].text.getString();
}