#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include "Core/State.hpp"
#include "Interface/Menu.hpp"
#include "Core/AudioManager.hpp"
#include <fstream>


struct GameConfig{
    int musicVolume = 50;
    int sfxVolume = 50;
    bool vsync = true;
    int fpsLimit = 60;

    void saveToFile(const std::string& filename) { //function to save the modified options
        std::ofstream file(filename);
        if (file.is_open()){
            file << musicVolume << "\n";
            file << sfxVolume << "\n";
            file << vsync << "\n";
            file << fpsLimit <<"\n";
            file.close();
        }
    }

    void loadFromSavedFile(const std::string& filename){ //function to load from disk
        std::ifstream file(filename);
        if(file.is_open()){
            file >> musicVolume;
            file >> sfxVolume;
            file >> vsync;
            file >> fpsLimit;
            file.close();
        }
    }
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
        bool updateHover(sf::Vector2f mousePos);
        void handleMouseClick(sf::Vector2f mousePos);
        void handleMouseMove(sf::Vector2f mousePos);
        void releaseSlider();
        void moveUp();
        void moveDown();
        void changeValue(int delta);
        void handleAction(State& currentState, sf::RenderWindow& window, GameConfig& globalConfig, AudioManager& audio);
        void updateSlidersFromConfig();
        void resetTempConfig(const GameConfig& globalConfig); //Discard changes if no apply is applied
        int getTempMusicVolume() const { return tempConfig.musicVolume; }
        int getTempSfxVolume() const { return tempConfig.sfxVolume; }
        void applySettings(sf::RenderWindow& window, GameConfig& globalConfig, AudioManager& audio);
        std::string getSelectedOption() const;

    private:
        int selectedIndex = 0;
        sf::Font font;
        std::vector<Button> options;
        sf::Text saveMessage;
        sf::Clock saveMsgClock;
        bool showSaveMessage = false;
        //temporary values ​​before applying
        Slider musicSlider;
        Slider sfxSlider;
        Slider fpsSlider;

        GameConfig tempConfig;
        void updateLabels();
        bool isIndexValid() const{
            return selectedIndex >= 0 && selectedIndex < (int)options.size(); 
        }
};