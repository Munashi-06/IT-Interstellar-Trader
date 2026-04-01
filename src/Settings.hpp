#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include "Menu.hpp"

struct GameConfig{
    int musicVolume = 50;
    int sfxVolume = 50;
    bool vsync = true;
    int fpsLimit = 60;
};

struct Slider {
    sf::RectangleShape track;
    sf::RectangleShape knob;
    bool dragging = false;

    Slider () : dragging(false) {}
    Slider(sf::Vector2f pos, float width, int value){
        track.setSize({width, 10.f});
        track.setPosition(pos);
        track.setFillColor(sf::Color(100, 100, 100));

        knob.setSize({15.f, 25.f});
        knob.setOrigin({7.5f, 12.5f});
        knob.setPosition({pos.x + (value*width/100.f), pos.y + 5.f});
        knob.setFillColor(sf::Color::Cyan);
    }
};

class Settings{
    public:
        Settings(float width, float height, const sf::Font& font);
        void draw(sf::RenderWindow& window);
        void updateHover(sf::Vector2f mousePos);
        void handleMouseClick(sf::Vector2f mousePos);
        void handleMouseMove(sf::Vector2f mousePos);
        void releaseSlider();
        
        void applySettings(sf::RenderWindow& window, GameConfig& globalConfig);
        void resetTempConfig(const GameConfig& globalConfig); //descartar cambios si no se hace apply
        std::string getSelectedOption() const;

    private:
        int selectedIndex = -1;
        sf::Font font;
        std::vector<Button> options;
        //valores temporales antes de aplicar
        Slider musicSlider;
        Slider sfxSlider;

        GameConfig tempConfig;
        void updateLabels(); // Para que el texto diga "Music 50%", etc.
};