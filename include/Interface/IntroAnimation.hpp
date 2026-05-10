#ifndef INTRO_ANIMATION_HPP
#define INTRO_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

namespace Interface {
    // Updated states for narrative and transitions
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
        
        // Texture resources
        sf::Texture personTex, bgTex, anim01Tex;
        
        // Smart pointers for SFML 3 (avoids error C2512)
        std::unique_ptr<sf::Sprite> personSprite;
        std::unique_ptr<sf::Sprite> backgroundSprite;
        std::unique_ptr<sf::Sprite> anim01Sprite;
        std::unique_ptr<sf::Music> backgroundMusic; 

        // UI elements
        std::unique_ptr<sf::RectangleShape> dialogueBox;
        std::unique_ptr<sf::Text> mainText;
        std::unique_ptr<sf::Text> creditsText;
        
        float fadeAlpha;     // Opacity control for fading
        float scrollSpeed;   // Scroll speed of the credits text
        float screenHeight;
        float screenWidth;
    };
}

#endif