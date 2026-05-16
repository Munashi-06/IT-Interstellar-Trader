#ifndef INTRO_ANIMATION_HPP
#define INTRO_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>

namespace Interface {
    enum class AnimState {
        Watching,
        Dialogue1,
        Poema,
        Dialogue2,
        Transition,
        Credits,
        ThankYou,
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
        // Función para centrar textos
        void centerText(sf::Text& text, float y);
        void centerText(sf::Text& text, const sf::RectangleShape& rect);
        
        // NUEVO: Crear líneas de créditos individuales
        void createCreditsLines(const sf::Font& font);
        
        AnimState state;
        float fadeAlpha;
        float scrollSpeed;
        float screenHeight;
        float screenWidth;
        float thankYouTimer;
        float musicDuration;
        float creditsScrollY;  // Posición Y para el scroll de créditos
        
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
        std::unique_ptr<sf::Text> mainText;
        std::unique_ptr<sf::Text> instructionText;
        
        // NUEVO: Vector de líneas de créditos (cada línea es un texto independiente)
        std::vector<std::unique_ptr<sf::Text>> creditLines;
        
        // Textos de agradecimiento
        std::unique_ptr<sf::Text> thankYouText;
        std::unique_ptr<sf::Text> escuadronText;
        std::unique_ptr<sf::Text> exitInstructionText;
    };
}

#endif