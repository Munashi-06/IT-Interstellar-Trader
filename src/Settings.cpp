#include "Settings.hpp"
#include "State.hpp"
#include "AudioManager.hpp"
#include <algorithm>

extern AudioManager g_Audio;

Settings::Settings(float width, float height, const sf::Font& font) : font(font), saveMessage(font, "") {
    std::vector<std::string> labels = {"Music", "SFX", "VSync", "FPS Limit", "APPLY", "BACK"};

    float xPos = (width / 2.f) - 300.f; // Ajusta el valor para centrar

    for (size_t i=0; i<labels.size(); ++i) {
        float yPos = 100.f + (i*90.f); // Espaciado vertical entre opciones
        sf::Vector2f bSize = {330.f, 60.f};
        if (i ==0 || i==1 || i==3){
            bSize.x = 600.f;
        }
        options.emplace_back(labels[i], bSize, sf::Vector2f{xPos, yPos}, const_cast<sf::Font&>(this->font));
        options.back().setAlignmentLeft(20.f);
    }
    float slidersX = xPos + 350.f;
    musicSlider = Slider({slidersX, 125.f}, 200.f, tempConfig.musicVolume);
    sfxSlider = Slider({slidersX, 215.f}, 200.f, tempConfig.sfxVolume);

    float fpsPercent = (tempConfig.fpsLimit - 5.f) / (144.f - 5.f) * 100.f;
    fpsSlider = Slider({slidersX, 395.f}, 200.f, (int)fpsPercent);
    updateLabels();

    if (tempConfig.vsync){
        fpsSlider.track.setFillColor(sf::Color(50, 50, 50));
        fpsSlider.knob.setFillColor(sf::Color(80, 80, 80));
    }

    saveMessage.setFont(this->font);
    saveMessage.setString("Settings Applied!");
    saveMessage.setCharacterSize(20);
    saveMessage.setFillColor(sf::Color::Green);
    saveMessage.setPosition({xPos, 650.f});

}

void Settings::moveUp() {
    if(options.empty()) return;
    if (selectedIndex < 0){
        selectedIndex = (int)options.size() -1;
    } else {
        options[selectedIndex].selected = false;
        selectedIndex = (selectedIndex - 1 + (int)options.size()) % (int)options.size();
    }
    options[selectedIndex].selected = true;
}

void Settings::moveDown() {
    if (selectedIndex < 0){
        selectedIndex = 0;
    } else {
        options[selectedIndex].selected = false;
        selectedIndex = (selectedIndex + 1) % (int)options.size();
    }
    options[selectedIndex].selected = true;
}

void Settings::changeValue(int delta){
    if (selectedIndex < 0 || selectedIndex >= (int)options.size()) return;
    switch (selectedIndex){
        case 0:
            tempConfig.musicVolume = std::clamp(tempConfig.musicVolume + (delta * 5), 0, 100);
            break;
        case 1:
            tempConfig.sfxVolume = std::clamp(tempConfig.sfxVolume + (delta * 5), 0, 100);
            break;
        case 2:
            if(delta != 0){
                tempConfig.vsync = !tempConfig.vsync;
            }
            break;
        case 3:
            if (!tempConfig.vsync){
                std::vector<int> commonHz = {5, 15, 30, 60, 120, 144};
                auto it = std::lower_bound(commonHz.begin(), commonHz.end(), tempConfig.fpsLimit);
                int currentIndex = std::distance(commonHz.begin(), it);

                int nextIndex = std::clamp(currentIndex + delta, 0, (int)commonHz.size() -1);
                tempConfig.fpsLimit = commonHz[nextIndex];
            }
            break;
    }
    updateSlidersFromConfig();
    updateLabels();
}

void Settings::updateSlidersFromConfig(){
    float width = 200.f;
    musicSlider.knob.setPosition({musicSlider.track.getPosition().x + (tempConfig.musicVolume * width / 100.f), musicSlider.track.getPosition().y + 5.f});
    sfxSlider.knob.setPosition({sfxSlider.track.getPosition().x + (tempConfig.sfxVolume * width / 100.f), sfxSlider.track.getPosition().y + 5.f});

    float fpsPercent = (tempConfig.fpsLimit - 5.f) / (144.f - 5.f);
    fpsSlider.knob.setPosition({fpsSlider.track.getPosition().x + (fpsPercent * width), fpsSlider.track.getPosition().y + 5.f});

    updateLabels();
}

bool Settings::updateHover(sf::Vector2f mousePos) {
    for(size_t i = 0; i< options.size(); ++i){
        if (options[i].shape.getGlobalBounds().contains(mousePos)){
            if (selectedIndex != (int)i){
                options[selectedIndex].selected = false;
                selectedIndex = (int)i;
                options[selectedIndex].selected = true;
                return true;
            }
            return false;
        }
    }
    return false;
}

void Settings::handleMouseClick(sf::Vector2f mousePos){
    if(musicSlider.knob.getGlobalBounds().contains(mousePos)) musicSlider.dragging = true;
    if(sfxSlider.knob.getGlobalBounds().contains(mousePos)) sfxSlider.dragging = true;
    if(fpsSlider.knob.getGlobalBounds().contains(mousePos)) fpsSlider.dragging = true;

    if (selectedIndex == 2) {
        tempConfig.vsync = !tempConfig.vsync;
    }
    updateLabels();
}

void Settings::handleMouseMove(sf::Vector2f mousePos) {
    auto updateSlider = [&](Slider& s, int& configVal, bool isFPS = false){
        if (isFPS && tempConfig.vsync) return; // No permitir mover el slider de FPS si VSync está activado
        if (s.dragging){
            float trackX = s.track.getPosition().x;
            float trackWidth = s.track.getSize().x;
            float newX = std::clamp(mousePos.x, trackX, trackX + trackWidth);
            s.knob.setPosition({newX, s.knob.getPosition().y});

            float percent = (newX - trackX)/trackWidth;
            if (isFPS){
                configVal = static_cast<int>(5 + (percent * (144.f - 5.f))); // Mapear el rango de fps entre 5 y 144
            } else{
                configVal = static_cast<int>(((newX - trackX) / trackWidth) * 100.f);
            }
        }
    };

    updateSlider(musicSlider, tempConfig.musicVolume);
    updateSlider(sfxSlider, tempConfig.sfxVolume);
    updateSlider(fpsSlider, tempConfig.fpsLimit, true);
    updateLabels();
}

void Settings::releaseSlider(){
    musicSlider.dragging = false;
    sfxSlider.dragging = false;
    fpsSlider.dragging = false;
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

void Settings::applySettings(sf::RenderWindow& window, GameConfig& globalConfig, AudioManager& audio){
    globalConfig = tempConfig;
    if(globalConfig.vsync){
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(0); // Desactivar límite de FPS para que VSync controle la tasa de refresco
    } else{
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(globalConfig.fpsLimit);
    }
    audio.updateVolumesFromConfig(globalConfig.musicVolume, globalConfig.sfxVolume);
    globalConfig.saveToFile("config.txt");
    showSaveMessage = true;
    saveMsgClock.restart();
}

void Settings::handleAction(State& currentState, sf::RenderWindow& window, GameConfig& globalConfig, AudioManager& audio){
    if (selectedIndex < 0 || selectedIndex >= (int)options.size()) return;
    std::string option = options[selectedIndex].text.getString();
    if (selectedIndex == 2){
        tempConfig.vsync = !tempConfig.vsync;
        updateLabels();
    } else if (selectedIndex == 4){
        applySettings(window, globalConfig, audio);
    } else if (selectedIndex == 5){
        resetTempConfig(globalConfig);
        currentState = State::Menu;
    }
}

void Settings::updateLabels(){
    options[0].text.setString("Music " + std::to_string(tempConfig.musicVolume) + "%");
    options[1].text.setString("SFX " + std::to_string(tempConfig.sfxVolume) + "%");
    options[2].text.setString("VSync " + std::string(tempConfig.vsync ? "ON" : "OFF"));
    options[3].text.setString("FPS Limit " + std::to_string(tempConfig.fpsLimit));

    if(tempConfig.vsync){
        options[3].text.setString("FPS Limit: [V-SYNC on]");
        options[3].text.setCharacterSize(22);
        options[3].text.setFillColor(sf::Color(150, 150, 150));
        fpsSlider.track.setFillColor(sf::Color(50, 50, 50));
        fpsSlider.knob.setFillColor(sf::Color(80, 80, 80));
    } else {
        options[3].text.setString("FPS Limit " + std::to_string(tempConfig.fpsLimit));
        options[3].text.setCharacterSize(30);
        options[3].text.setFillColor(sf::Color::White);
        fpsSlider.track.setFillColor(sf::Color(100, 100, 100));
        fpsSlider.knob.setFillColor(sf::Color::Cyan);
    }

    for (int i=0; i<4; ++i){
        options[i].setAlignmentLeft(20.f);
    }
}

void Settings::draw(sf::RenderWindow& window){
    for(auto& opt : options) opt.draw(window);
    window.draw(musicSlider.track);
    window.draw(musicSlider.knob);
    window.draw(sfxSlider.track);
    window.draw(sfxSlider.knob);
    window.draw(fpsSlider.track);
    window.draw(fpsSlider.knob);

    if(showSaveMessage){
        if(saveMsgClock.getElapsedTime().asSeconds() < 2.f){
            window.draw(saveMessage);
        } else{
            showSaveMessage = false;
        }
    }
}

std::string Settings::getSelectedOption() const{
    std::vector<std::string> labels = {"Music", "SFX", "VSync", "FPS Limit", "APPLY", "BACK"};
    if (selectedIndex >= 0 && selectedIndex <(int)labels.size()){
        return labels[selectedIndex];
    }
    return "";
}