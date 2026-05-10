#ifndef INTRO_ANIMATION_HPP
#define INTRO_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

namespace Interface {
    // Estados actualizados para la narrativa y transiciones
    enum class AnimState { Watching, Dialogue1, Poema, Dialogue2, Transition, Credits, Finished };

    class IntroAnimation {
    public:
        IntroAnimation(float width, float height);
        
        bool loadAssets(const sf::Font& font); 
        void handleInput(sf::Keyboard::Key key);
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void reset(); 
        
        AnimState getState() const { return state; }
        bool isMusicPlaying() const;

    private:
        AnimState state;
        
        // Recursos de texturas
        sf::Texture personTex, bgTex, anim01Tex;
        
        // Punteros inteligentes para SFML 3 (evitan error C2512)
        std::unique_ptr<sf::Sprite> personSprite;
        std::unique_ptr<sf::Sprite> backgroundSprite;
        std::unique_ptr<sf::Sprite> anim01Sprite;
        std::unique_ptr<sf::Music> backgroundMusic; 

        // Elementos de UI
        std::unique_ptr<sf::RectangleShape> dialogueBox;
        std::unique_ptr<sf::Text> mainText;
        std::unique_ptr<sf::Text> creditsText;
        
        float fadeAlpha;     // Control de opacidad para el difuminado
        float scrollSpeed;   // Velocidad del texto de créditos
        float screenHeight;
        float screenWidth;
    };
}

#endif