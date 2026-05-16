#ifndef GAME_INTRO_ANIMATION_HPP
#define GAME_INTRO_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

namespace Interface {
    enum class GameIntroState {
        Intro1, Intro2, Intro3, Intro4, FadingOut, Finished
    };

    class GameIntroAnimation {
    public:
        GameIntroAnimation(float width, float height);
        
        bool loadAssets(const sf::Font& font);
        void handleInput(sf::Keyboard::Key key);
        void handleMouseMove(const sf::Vector2f& mousePos);
        bool handleMouseClick(const sf::Vector2f& mousePos);
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void reset();
        void start();
        void skip();
        
        bool isFinished() const { return state == GameIntroState::Finished; }
        bool isMusicPlaying() const;

    private:
        void startFadeToNext();
        
        GameIntroState state;
        float fadeAlpha;
        float fadeSpeed;
        bool isFading;
        GameIntroState pendingState;
        float screenWidth;
        float screenHeight;
        
        bool skipRequested;        // SOLO UNA VEZ
        bool isHoveringSkip;       // SOLO UNA VEZ
        
        // Textures
        sf::Texture bgTex1, bgTex2, bgTex3, bgTex4;
        std::unique_ptr<sf::Sprite> backgroundSprite;
        
        // Music
        std::unique_ptr<sf::Music> backgroundMusic;
        
        // UI Elements
        std::unique_ptr<sf::RectangleShape> dialogueBox;
        std::unique_ptr<sf::Text> mainText;
        std::unique_ptr<sf::RectangleShape> fadeOverlay;
        
        // Skip button
        std::unique_ptr<sf::RectangleShape> skipButton;
        std::unique_ptr<sf::Text> skipButtonText;
    };
}

#endif