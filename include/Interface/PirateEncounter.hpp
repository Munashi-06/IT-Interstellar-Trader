#ifndef PIRATE_ENCOUNTER_HPP
#define PIRATE_ENCOUNTER_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include <vector>
#include <string>

namespace Interface {
    enum class PirateMenu { Main, Bribery, Result };

    class PirateEncounter {
    public:
        PirateEncounter();
        bool loadAssets();
        void update(float dt);
        void draw(sf::RenderWindow& window, sf::Font& font);
        
        bool rollForEncounter(float chance);
        void reset(); 
        void stop();  

        void setResult(const std::string& message);

        void handleInput(sf::Keyboard::Key key);
        void setShowButtons(bool show);
        bool isShowingButtons() const;
        
        PirateMenu getCurrentMenu() const { return currentMenu; }
        int getSelectedButton() const { return selectedButton; }
        void setMenu(PirateMenu menu) { currentMenu = menu; selectedButton = 0; }

    private:
        sf::Texture pirateTex;
        std::unique_ptr<sf::Sprite> pirateSprite;
        bool active;
        float displayTimer;
        float baseScale;
        bool showButtons;
        PirateMenu currentMenu;
        int selectedButton;
        std::string resultMessage; 
        std::vector<std::string> mainOptions;
        std::vector<std::string> briberyOptions;
        std::mt19937 rng;
    };
}
#endif