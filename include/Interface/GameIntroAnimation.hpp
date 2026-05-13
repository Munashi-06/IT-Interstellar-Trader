#ifndef GAME_INTRO_ANIMATION_HPP
#define GAME_INTRO_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

namespace Interface {

    enum class GameIntroState {
        Intro1,     // Primer fondo (anim02)
        Intro2,     // Segundo fondo (anim03)
        Intro3,     // Tercer fondo (anim04)
        Intro4,     // Cuarto fondo (anim05)
        FadingOut,  // Desvanecimiento final
        Finished    // Intro terminada
    };

    class GameIntroAnimation {
    public:
        GameIntroAnimation(float width, float height);
        ~GameIntroAnimation() = default;

        bool loadAssets(const sf::Font& font);
        void handleInput(sf::Keyboard::Key key);
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void reset();
        void start();
        bool isFinished() const { return state == GameIntroState::Finished; }
        bool isMusicPlaying() const;
        GameIntroState getState() const { return state; }

    private:
        void startFadeToNext();
        
        GameIntroState state;
        
        // Texturas para los diferentes fondos
        sf::Texture bgTex1;  // anim02
        sf::Texture bgTex2;  // anim03
        sf::Texture bgTex3;  // anim04
        sf::Texture bgTex4;  // anim05
        
        std::unique_ptr<sf::Sprite> backgroundSprite;
        
        // Para el fade
        sf::RectangleShape fadeOverlay;
        float fadeAlpha;
        float fadeSpeed;
        bool isFading;
        GameIntroState pendingState;
        
        // Música
        std::unique_ptr<sf::Music> backgroundMusic;
        
        // Elementos de UI
        std::unique_ptr<sf::RectangleShape> dialogueBox;
        std::unique_ptr<sf::Text> mainText;
        
        // Dimensiones de pantalla
        float screenWidth;
        float screenHeight;
    };

} // namespace Interface

#endif