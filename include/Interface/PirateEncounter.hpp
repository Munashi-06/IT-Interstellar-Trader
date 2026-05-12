#ifndef PIRATE_ENCOUNTER_HPP
#define PIRATE_ENCOUNTER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>

namespace Interface {
    class PirateEncounter {
    public:
        PirateEncounter();
        
        bool loadAssets();
        // Lógica para decidir si ocurre el ataque (ej. 30% de probabilidad)
        bool rollForEncounter(float chance = 0.3f); 
        
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void reset();

        bool isFinished() const { return finished; }
        bool isActive() const { return active; }

    private:
        sf::Texture pirateTex;
        std::unique_ptr<sf::Sprite> pirateSprite;
        
        bool active;
        bool finished;
        float displayTimer;
        float maxDisplayTime;
        float baseScale;
        
        // Generador de números aleatorios
        std::mt19937 rng;
    };
}

#endif