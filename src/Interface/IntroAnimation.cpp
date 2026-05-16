#include "Interface/IntroAnimation.hpp"
#include <iostream>

namespace Interface {
    
    //=============================================================================
    // Constructor
    //=============================================================================
    IntroAnimation::IntroAnimation(float width, float height) 
        : state(AnimState::Watching), 
          fadeAlpha(0.f), 
          scrollSpeed(90.f), 
          screenHeight(height),
          screenWidth(width)
    {}

    //=============================================================================
    // Load all assets
    //=============================================================================
    bool IntroAnimation::loadAssets(const sf::Font& font) {
        // 1. Load Textures
        if (!bgTex.loadFromFile("assets/anim02.png") ||
            !anim01Tex.loadFromFile("assets/anim01.png")) {
            return false;
        }
        
        // 2. Music
        backgroundMusic = std::make_unique<sf::Music>();
        if (!backgroundMusic->openFromFile("assets/audio/end.ogg")) {
            std::cerr << "Error: assets/audio/end.ogg not found" << std::endl;
        } else {
            backgroundMusic->setLooping(false);
        }

        // 3. Create Sprites
        backgroundSprite = std::make_unique<sf::Sprite>(bgTex);
        personSprite = std::make_unique<sf::Sprite>(personTex);
        anim01Sprite = std::make_unique<sf::Sprite>(anim01Tex);

        // Scale backgrounds to 1280x720
        backgroundSprite->setScale({1280.f / bgTex.getSize().x, 720.f / bgTex.getSize().y});
        anim01Sprite->setScale({1280.f / anim01Tex.getSize().x, 720.f / anim01Tex.getSize().y});
        
        // Anim01 invisible at start
        anim01Sprite->setColor(sf::Color(255, 255, 255, 0));

        // 4. Configure Dialogue Box
        dialogueBox = std::make_unique<sf::RectangleShape>(sf::Vector2f(1000.f, 150.f));
        dialogueBox->setFillColor(sf::Color(0, 0, 0, 210));
        dialogueBox->setOutlineThickness(2);
        dialogueBox->setOutlineColor(sf::Color::White);
        dialogueBox->setOrigin({500.f, 75.f});
        dialogueBox->setPosition({640.f, 600.f});

        mainText = std::make_unique<sf::Text>(font, "", 26);
        mainText->setFillColor(sf::Color::White);

        // 5. Configure Instruction Text
        instructionText = std::make_unique<sf::Text>(font, "Press ENTER to continue...", 20);
        instructionText->setFillColor(sf::Color::Yellow);
        instructionText->setOutlineColor(sf::Color::Black);
        instructionText->setOutlineThickness(1);
        instructionText->setStyle(sf::Text::Italic);

        sf::FloatRect instrBounds = instructionText->getLocalBounds();
        instructionText->setOrigin({instrBounds.size.x / 2.f, instrBounds.size.y / 2.f});
        instructionText->setPosition({640.f, 680.f});

        // 6. Configure Credits
        std::string creditsStr = "IT: INTERSTELLAR TRADER\n\n"
                                 "STORY AND CODE\nAngel Garcia\nAbraham Vergara\nReinier Manrique\n\n"
                                 "VISUAL ART\nReinier Manrique\n\n"
                                 "HEAD OF TRADING STRATEGY\nAbraham Vergara\n\n"
                                 "PROJECT MANAGER\nAngel Garcia\n\n"
                                 "SYSTEMS\nProject IT 2026\n\n\n"
                                 "THANKS FOR PLAYING";
        creditsText = std::make_unique<sf::Text>(font, creditsStr, 28);
        creditsText->setFillColor(sf::Color::Yellow);
        creditsText->setOutlineColor(sf::Color::Black);
        creditsText->setOutlineThickness(1);
        
        sf::FloatRect creditsBounds = creditsText->getLocalBounds();
        creditsText->setOrigin({creditsBounds.size.x / 2.f, 0.f});
        creditsText->setPosition({640.f, 750.f});

        return true;
    }

    //=============================================================================
    // Handle keyboard input
    //=============================================================================
    void IntroAnimation::handleInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Enter) {
            if (state == AnimState::Watching) {
                state = AnimState::Dialogue1;
                if (backgroundMusic) backgroundMusic->play();
            }
            else if (state == AnimState::Dialogue1) state = AnimState::Poema;
            else if (state == AnimState::Poema) state = AnimState::Dialogue2;
            else if (state == AnimState::Dialogue2) state = AnimState::Transition;
            else if (state == AnimState::Credits) {
                state = AnimState::Finished;
                if (backgroundMusic) backgroundMusic->stop();
            }
        }
    }

    //=============================================================================
    // Update animation logic
    //=============================================================================
    void IntroAnimation::update(float dt) {
        if (state == AnimState::Dialogue1) {
            mainText->setString("Thus, Juan Jose Josefino was able to return to his home planet...");
        }
        else if (state == AnimState::Poema) {
            mainText->setString("Among the stars, his solitude danced,\nFlying through the void with great will.\nCosmic dust traced his path.");
        }
        else if (state == AnimState::Dialogue2) {
            mainText->setString("The journey has ended, but stellar trade never sleeps.\n(Press ENTER to finish)");
        }
        else if (state == AnimState::Transition) {
            fadeAlpha += 110.f * dt;
            if (fadeAlpha > 255.f) {
                fadeAlpha = 255.f;
                state = AnimState::Credits;
            }
            personSprite->setColor(sf::Color(255, 255, 255, 255 - static_cast<uint8_t>(fadeAlpha)));
            anim01Sprite->setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(fadeAlpha)));
        }
        else if (state == AnimState::Credits) {
            creditsText->move({0.f, -scrollSpeed * dt});
            
            float textBottom = creditsText->getPosition().y + creditsText->getLocalBounds().size.y;
            if (textBottom < 0 && !isMusicPlaying()) {
                state = AnimState::Finished;
            }
        }

        sf::FloatRect bounds = mainText->getLocalBounds();
        mainText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        mainText->setPosition({640.f, 600.f});
    }

    //=============================================================================
    // Draw all elements
    //=============================================================================
    void IntroAnimation::draw(sf::RenderWindow& window) {
        window.draw(*backgroundSprite);
        
        if (state == AnimState::Transition || state == AnimState::Credits) {
            window.draw(*anim01Sprite);
        }

        if (state != AnimState::Credits && state != AnimState::Finished) {
            window.draw(*personSprite);
        }

        if (state == AnimState::Watching || state == AnimState::Dialogue1 || 
            state == AnimState::Poema || state == AnimState::Dialogue2) {
            
            if (state != AnimState::Watching) {
                window.draw(*dialogueBox);
                window.draw(*mainText);
            }
            
            if (instructionText) {
                window.draw(*instructionText);
            }
        }

        if (state == AnimState::Credits) {
            window.draw(*creditsText);
        }
    }

    //=============================================================================
    // Check if music is playing
    //=============================================================================
    bool IntroAnimation::isMusicPlaying() const {
        return backgroundMusic && backgroundMusic->getStatus() == sf::SoundSource::Status::Playing;
    }

    //=============================================================================
    // Reset animation to initial state
    //=============================================================================
    void IntroAnimation::reset() {
        state = AnimState::Watching;
        fadeAlpha = 0.f;
        if (backgroundMusic) backgroundMusic->stop();
        personSprite->setColor(sf::Color::White);
        anim01Sprite->setColor(sf::Color(255, 255, 255, 0));
        creditsText->setPosition({640.f, 750.f});
    }
}