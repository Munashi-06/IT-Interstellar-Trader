#include "Settings.hpp"
#include <algorithm>

Settings::Settings(float width, float height, const sf::Font& font) : font(font) {
    std::vector<std::string> labels = {"Music", "SFX", "VSync", "FPS Limit", "APPLY", "BACK"};

    float xPos = (width / 2.f) - 300.f; // Ajusta el valor para centrar

    for (size_t i=0; i<labels.size(); ++i) {
        float yPos = 100.f + (i*90.f); // Espaciado vertical entre opciones
        sf::Vector2f bSize = {330.f, 60.f};
        if (i ==0 || i==1){
            bSize.x = 600.f;
        }
        options.emplace_back(labels[i], bSize, sf::Vector2f{xPos, yPos}, const_cast<sf::Font&>(this->font));
        options.back().setAlignmentLeft(20.f);
    }
    float slidersX = xPos + 350.f;
    musicSlider = Slider({slidersX, 125.f}, 200.f, tempConfig.musicVolume);
    sfxSlider = Slider({slidersX, 215.f}, 200.f, tempConfig.sfxVolume);
    updateLabels();
}

void Settings::updateHover(sf::Vector2f mousePos) {
    selectedIndex= -1;
    for(size_t i = 0; i< options.size(); ++i){
        options[i].selected = false;
        if (options[i].shape.getGlobalBounds().contains(mousePos)){
            selectedIndex = (int)i;
            options[i].selected = true;
        }
    }
}

void Settings::handleMouseClick(sf::Vector2f mousePos){
    if(musicSlider.knob.getGlobalBounds().contains(mousePos)) musicSlider.dragging = true;
    else if(sfxSlider.knob.getGlobalBounds().contains(mousePos)) sfxSlider.dragging = true;

    if (selectedIndex == 2) {
        tempConfig.vsync = !tempConfig.vsync;
    }
    updateLabels();
}

void Settings::handleMouseMove(sf::Vector2f mousePos) {
    auto updateSlider = [&](Slider& s, int& configVal){
        if (s.dragging){
            float trackX = s.track.getPosition().x;
            float trackWidth = s.track.getSize().x;

            float newX = std::clamp(mousePos.x, trackX, trackX + trackWidth);
            s.knob.setPosition({newX, s.knob.getPosition().y});
            configVal = static_cast<int>(((newX - trackX) / trackWidth) * 100.f);
        }
    };

    updateSlider(musicSlider, tempConfig.musicVolume);
    updateSlider(sfxSlider, tempConfig.sfxVolume);
    updateLabels();
}

void Settings::releaseSlider(){
    musicSlider.dragging = false;
    sfxSlider.dragging = false;
}

void Settings::resetTempConfig(const GameConfig& globalConfig){
    tempConfig = globalConfig; //se regresa a los valores globales, descartando cambios si no se da apply
    float slidersX = options[0].shape.getPosition().x + 350.f;
    float mX = slidersX + (tempConfig.musicVolume * (musicSlider.track.getSize().x / 100.f));
    musicSlider.knob.setPosition({mX, musicSlider.knob.getPosition().y});
    float sX = slidersX + (tempConfig.sfxVolume * (sfxSlider.track.getSize().x / 100.f));
    sfxSlider.knob.setPosition({sX, sfxSlider.knob.getPosition().y});
    updateLabels();
}

void Settings::applySettings(sf::RenderWindow& window, GameConfig& globalConfig){
    globalConfig = tempConfig;
    window.setVerticalSyncEnabled(globalConfig.vsync);
    window.setFramerateLimit(globalConfig.fpsLimit);
}

void Settings::updateLabels(){
    options[0].text.setString("Music " + std::to_string(tempConfig.musicVolume) + "%");
    options[1].text.setString("SFX " + std::to_string(tempConfig.sfxVolume) + "%");
    options[2].text.setString("VSync " + std::string(tempConfig.vsync ? "ON" : "OFF"));

    options[0].setAlignmentLeft(20.f);
    options[1].setAlignmentLeft(20.f);
    options[2].setAlignmentLeft(20.f);
}

void Settings::draw(sf::RenderWindow& window){
    for(auto& opt : options)opt.draw(window);
    window.draw(musicSlider.track);
    window.draw(musicSlider.knob);
    window.draw(sfxSlider.track);
    window.draw(sfxSlider.knob);
}

std::string Settings::getSelectedOption() const{
    if (selectedIndex != -1) {
        std::vector<std::string> labels = {"Music", "SFX", "VSync", "FPS Limit", "APPLY", "BACK"};
        return labels[selectedIndex];
    }
    return "";
}