#include "Settings.hpp"
#include <algorithm>

Settings::Settings(float width, float height, const sf::Font& font) : font(font) {
    std::vector<std::string> labels = {"Music", "SFX", "VSync", "FPS Limit", "APPLY", "BACK"};
    for (size_t i=0; i<labels.size(); ++i) {
        float xPos = (width / 2.f) - 165.f; // Ajusta el valor para centrar
        float yPos = 150.f + (i*80.f); // Espaciado vertical entre opciones
        options.emplace_back(labels[i], sf::Vector2f{330.f, 60.f}, sf::Vector2f{xPos, yPos}, const_cast<sf::Font&>(this->font));
    }
    options[0].selected = true;
    updateLabels();
}

void Settings::draw(sf::RenderWindow& window) {
    for (auto& option : options) {
        option.draw(window);
    }
}

void Settings::moveUp() {
    options[selectedIndex].selected = false;
    selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
    options[selectedIndex].selected = true;
}

void Settings::moveDown() {
    options[selectedIndex].selected = false;
    selectedIndex = (selectedIndex + 1) % options.size();
    options[selectedIndex].selected = true;
}

void Settings::changeValue(int delta){
    if (selectedIndex == 0) tempConfig.musicVolume = std::clamp(tempConfig.musicVolume + delta, 0, 100);
    if (selectedIndex == 1) tempConfig.sfxVolume = std::clamp(tempConfig.sfxVolume + delta, 0, 100);
    if (selectedIndex == 2) tempConfig.vsync = !tempConfig.vsync;
    if (selectedIndex == 3) {
        tempConfig.fpsLimit += (delta > 0 ? 30 : -30);
        if (tempConfig.fpsLimit < 30) tempConfig.fpsLimit = 30;
        if (tempConfig.fpsLimit > 144) tempConfig.fpsLimit = 144;
    }
    updateLabels();
}

void Settings::updateLabels(){
    options[0].text.setString("Music: " + std::to_string(tempConfig.musicVolume) + "%");
    options[1].text.setString("SFX: " + std::to_string(tempConfig.sfxVolume) + "%");
    options[2].text.setString(std::string("VSync: ") + (tempConfig.vsync ? "ON" : "OFF"));
    options[3].text.setString("FPS Limit: " + std::to_string(tempConfig.fpsLimit));
}

void Settings::applySettings(sf::RenderWindow& window, GameConfig& globalConfig){
    globalConfig = tempConfig; //guarda cambios
    window.setVerticalSyncEnabled(globalConfig.vsync);
    window.setFramerateLimit(globalConfig.fpsLimit);
}

std::string Settings::getSelectedOption() const{
    std::vector<std::string> labels = {"Music", "SFX", "VSync", "FPS Limit", "APPLY", "BACK"};
    return labels[selectedIndex];
}