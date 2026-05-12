#include "Interface/PirateEncounter.hpp"
#include <iostream>
#include <cmath>

namespace Interface {
    PirateEncounter::PirateEncounter() 
        : active(false), 
          finished(false), 
          displayTimer(0.f), 
          maxDisplayTime(3.5f),
          baseScale(1.0f)
    {
        std::random_device rd;
        rng.seed(rd());
    }

    bool PirateEncounter::loadAssets() {
        // Cargamos anim03.png
        if (!pirateTex.loadFromFile("assets/anim03.png")) {
            std::cerr << "[ERROR] No se pudo cargar assets/anim03.png" << std::endl;
            return false;
        }
        
        pirateSprite = std::make_unique<sf::Sprite>(pirateTex);
        sf::Vector2u texSize = pirateTex.getSize();
        
        // 1. Centrar origen
        pirateSprite->setOrigin({texSize.x / 2.f, texSize.y / 2.f});
        
        // 2. Posición central
        pirateSprite->setPosition({640.f, 360.f});
        
        // 3. Escala base
        float scale = 1.0f;
        if (texSize.x > 800) scale = 800.0f / texSize.x;
        if (texSize.y > 600) scale = std::min(scale, 600.0f / texSize.y);
        
        baseScale = scale; 
        pirateSprite->setScale({baseScale, baseScale});
        
        return true;
    }

    bool PirateEncounter::rollForEncounter(float chance) {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        if (dist(rng) < chance) {
            active = true;
            finished = false;
            displayTimer = 0.f;
            return true;
        }
        return false;
    }

    void PirateEncounter::update(float dt) {
        if (!active) return;

        displayTimer += dt;
        
        // Animación senoidal de pulso
        float pulse = 1.0f + 0.05f * std::sin(displayTimer * 10.f);
        pirateSprite->setScale({baseScale * pulse, baseScale * pulse});

        // Eliminamos el autocierre: ya no ponemos active = false por tiempo.
    }

    void PirateEncounter::draw(sf::RenderWindow& window) {
        // Solo dibujamos si active es true
        if (active && pirateSprite) {
            // Aseguramos que sea visible y blanco total
            pirateSprite->setColor(sf::Color::White);
            window.draw(*pirateSprite);
        }
    }

    void PirateEncounter::reset() {
        // IMPORTANTE: Para que aparezca al presionar la tecla o el evento, 
        // active debe ser true al iniciar el encuentro.
        active = true; 
        finished = false;
        displayTimer = 0.f;

        if (pirateSprite) {
            pirateSprite->setScale({baseScale, baseScale});
        }
    }
}