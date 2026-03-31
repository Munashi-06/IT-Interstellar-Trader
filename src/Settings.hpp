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

class Settings{
    public:
        Settings(float width, float height, const sf::Font& font);
        void draw(sf::RenderWindow& window);
        void moveUp();
        void moveDown();
        void changeValue(int delta); //para subir/bajar volumen, etc.
        void applySettings(sf::RenderWindow& window, GameConfig& globalConfig);
        std::string getSelectedOption() const;

    private:
        int selectedIndex = 0;
        sf::Font font;
        std::vector<Button> options;
        //valores temporales antes de aplicar
        GameConfig tempConfig;

        void updateLabels(); // Para que el texto diga "Music 50%", etc.
};