#include "Menu.hpp"

Button::Button(const std::string& label, sf::Vector2f size, sf::Vector2f pos, sf::Font& font)
    : text(font, label, 35) // Corregimos C2512 aquí
{
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent); // Invisible por defecto
    shape.setOutlineColor(selectedColor);
    shape.setOutlineThickness(2);

    text.setFillColor(unselectedColor);
    
    // Centrar texto en la caja
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f + 12.f});
    text.setPosition({pos.x + size.x / 2.f, pos.y + size.y / 2.f});
}

void Button::setAlignmentLeft(float margin = 15.f){
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({0.f, bounds.size.y / 2.f + 12.f});
    text.setPosition({shape.getPosition().x + margin, shape.getPosition().y + shape.getSize().y / 2.f});
}

void Button::draw(sf::RenderWindow& window) {
    if (selected) {
        window.draw(shape); // Solo dibujamos la caja si está seleccionado
        text.setFillColor(selectedColor);
    }
    else {
        text.setFillColor(unselectedColor);
    }
    window.draw(text);
}

Menu::Menu(float width, float height)
    : title(font, "", 70)
{
    if (!font.openFromFile("assets/fonts/04B_03__.TTF")) {
        std::cerr << "Error cargando la fuente" << std::endl;
    }

    // --- Configuración del Título ---
    title.setFont(font);
    title.setString("INTERSTELLAR TRADER");
    title.setCharacterSize(70); // Más grande que los botones
    title.setFillColor(sf::Color::White); // Color distintivo
    
    // Centrar el título horizontalmente
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    title.setPosition({width / 2.f, 160.f}); // 100 píxeles desde arriba

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
    window.draw(title);
    for (auto& btn : options) {
        btn.draw(window);
    }
}

void Menu::moveUp() {
    if(selectedItemIndex < 0){
        selectedItemIndex = options.size() - 1; // Si no hay ningún botón seleccionado, seleccionamos el último
    } else {
        options[selectedItemIndex].selected = false;
        selectedItemIndex = (selectedItemIndex - 1 + options.size()) % options.size();
    }
    options[selectedItemIndex].selected = true;
}

void Menu::moveDown() {
    if (selectedItemIndex < 0) {
        selectedItemIndex = 0; // Si no hay ningún botón seleccionado, seleccionamos el primero
    } else {
        options[selectedItemIndex].selected = false;
        selectedItemIndex = (selectedItemIndex + 1) % options.size();
    }
    options[selectedItemIndex].selected = true;
}

bool Menu::updateHover(sf::Vector2f mousePos) {
    int oldIndex = selectedItemIndex;
    int hoveredIndex = -1;
    
    for (size_t i =0; i < options.size(); ++i) {
        if (options[i].shape.getGlobalBounds().contains(mousePos)){
            hoveredIndex = (int)i;
            break;
        }
    }
    if (hoveredIndex != -1 && hoveredIndex != oldIndex) {
        if (oldIndex >= 0 && oldIndex < (int)options.size()){
            options[oldIndex].selected = false;
        }
        selectedItemIndex = hoveredIndex;
        options[selectedItemIndex].selected = true;
        return true; // Se ha cambiado la selección
        }
    return false; // No se ha cambiado la selección
}

std::string Menu::getSelectedOption() const {
    return options[selectedItemIndex].text.getString();
}