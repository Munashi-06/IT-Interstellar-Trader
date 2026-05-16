#ifndef INTRO_ANIMATION_HPP
#define INTRO_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

namespace Interface {
    enum class AnimState {
        Watching,
        Dialogue1,
        Poema,
        Dialogue2,
        Transition,
        Credits,
        Finished
    };

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
        float fadeAlpha;
        float scrollSpeed;
        float screenHeight;
        float screenWidth;
        
        // Textures and sprites
        sf::Texture bgTex;
        sf::Texture personTex;
        sf::Texture anim01Tex;
        std::unique_ptr<sf::Sprite> backgroundSprite;
        std::unique_ptr<sf::Sprite> personSprite;
        std::unique_ptr<sf::Sprite> anim01Sprite;
        
        // Music
        std::unique_ptr<sf::Music> backgroundMusic;
        
        // UI Elements
        std::unique_ptr<sf::RectangleShape> dialogueBox;
        std::unique_ptr<sf::Text> mainText;        // SOLO UNA VEZ
        std::unique_ptr<sf::Text> instructionText;
        std::unique_ptr<sf::Text> creditsText;     // SOLO UNA VEZ
    };
}

#endif