#include "Interface/GameIntroAnimation.hpp"
#include <iostream>

namespace Interface {

    //=============================================================================
    // Constructor
    //=============================================================================
    GameIntroAnimation::GameIntroAnimation(float width, float height)
        : state(GameIntroState::Intro1),
          fadeAlpha(255.f),
          fadeSpeed(200.f),
          isFading(true),
          pendingState(GameIntroState::Intro1),
          screenWidth(width),
          screenHeight(height),
          skipRequested(false),
          isHoveringSkip(false)
    {
        fadeOverlay = std::make_unique<sf::RectangleShape>(sf::Vector2f(1280.f, 720.f));
        fadeOverlay->setFillColor(sf::Color(0, 0, 0, 255));
    }

    //=============================================================================
    // Load all assets
    //=============================================================================
    bool GameIntroAnimation::loadAssets(const sf::Font& font) {
        // Create UI elements
        dialogueBox = std::make_unique<sf::RectangleShape>(sf::Vector2f(1000.f, 150.f));
        dialogueBox->setFillColor(sf::Color(0, 0, 0, 210));
        dialogueBox->setOutlineThickness(2);
        dialogueBox->setOutlineColor(sf::Color::White);
        dialogueBox->setOrigin({500.f, 75.f});
        dialogueBox->setPosition({640.f, 600.f});
        
        mainText = std::make_unique<sf::Text>(font, "", 26);
        mainText->setFillColor(sf::Color::White);

        // Create SKIP button
        skipButton = std::make_unique<sf::RectangleShape>(sf::Vector2f(100.f, 40.f));
        skipButton->setFillColor(sf::Color(50, 50, 50, 200));
        skipButton->setOutlineThickness(2);
        skipButton->setOutlineColor(sf::Color::White);
        skipButton->setPosition({1150.f, 20.f});
        
        skipButtonText = std::make_unique<sf::Text>(font, "SKIP", 20);
        skipButtonText->setFillColor(sf::Color::White);
        sf::FloatRect skipBounds = skipButtonText->getLocalBounds();
        skipButtonText->setOrigin({skipBounds.size.x / 2.f, skipBounds.size.y / 2.f});
        skipButtonText->setPosition({1200.f, 35.f});

        backgroundMusic = std::make_unique<sf::Music>();

        // Load main texture
        if (!bgTex1.loadFromFile("assets/anim04.png")) {
            std::cerr << "[GameIntro] Error: assets/anim04.png not found" << std::endl;
            return false;
        }
        
        backgroundSprite = std::make_unique<sf::Sprite>(bgTex1);
        backgroundSprite->setScale({1280.f / bgTex1.getSize().x, 720.f / bgTex1.getSize().y});

        // Load remaining textures
        if (!bgTex2.loadFromFile("assets/anim09.png")) {
            std::cerr << "[GameIntro] Error: assets/anim09.png not found" << std::endl;
            bgTex2 = bgTex1;
        }
        
        if (!bgTex3.loadFromFile("assets/anim07.png")) {
            std::cerr << "[GameIntro] Error: assets/anim07.png not found" << std::endl;
            bgTex3 = bgTex1;
        }
        
        if (!bgTex4.loadFromFile("assets/anim10.png")) {
            std::cerr << "[GameIntro] Error: assets/anim10.png not found" << std::endl;
            bgTex4 = bgTex1;
        }
        
        // Load music
        if (!backgroundMusic->openFromFile("assets/audio/undertale_dogsong.ogg")) {
            std::cerr << "[GameIntro] Error: assets/audio/undertale_dogsong.ogg not found" << std::endl;
            return false; 
        }
        else {
            backgroundMusic->setLooping(true);
        }
        
        return true;
    }

    //=============================================================================
    // Start the intro
    //=============================================================================
    void GameIntroAnimation::start() {
        reset();
        skipRequested = false;
        isHoveringSkip = false;
        state = GameIntroState::Intro1;
        backgroundSprite->setTexture(bgTex1, true);
        isFading = true;
        fadeAlpha = 255.f;
        fadeOverlay->setFillColor(sf::Color(0, 0, 0, 255));
        
        if (backgroundMusic) {
            backgroundMusic->stop();
            backgroundMusic->play();
        }
    }

    //=============================================================================
    // Skip the intro
    //=============================================================================
    void GameIntroAnimation::skip() {
        if (!skipRequested && !isFading) {
            skipRequested = true;
        }
    }

    //=============================================================================
    // Handle mouse movement for skip button
    //=============================================================================
    void GameIntroAnimation::handleMouseMove(const sf::Vector2f& mousePos) {
        if (!skipButton) return;
        
        isHoveringSkip = skipButton->getGlobalBounds().contains(mousePos);
        
        if (isHoveringSkip) {
            skipButton->setFillColor(sf::Color(150, 150, 150, 230));
            skipButton->setOutlineColor(sf::Color::Yellow);
        } else {
            skipButton->setFillColor(sf::Color(50, 50, 50, 200));
            skipButton->setOutlineColor(sf::Color::White);
        }
    }

    //=============================================================================
    // Handle mouse click on skip button
    //=============================================================================
    bool GameIntroAnimation::handleMouseClick(const sf::Vector2f& mousePos) {
        if (!skipButton) return false;
        
        if (skipButton->getGlobalBounds().contains(mousePos)) {
            skip();
            return true;
        }
        return false;
    }

    //=============================================================================
    // Start fade transition
    //=============================================================================
    void GameIntroAnimation::startFadeToNext() {
        isFading = true;
        fadeAlpha = 0.f;
    }

    //=============================================================================
    // Handle keyboard input
    //=============================================================================
    void GameIntroAnimation::handleInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Escape || key == sf::Keyboard::Key::S || key == sf::Keyboard::Key::Space) {
            skip();
        }
        
        if (key == sf::Keyboard::Key::Enter && !isFading && !skipRequested) {
            switch (state) {
                case GameIntroState::Intro1:
                    pendingState = GameIntroState::Intro2;
                    startFadeToNext();
                    break;
                case GameIntroState::Intro2:
                    pendingState = GameIntroState::Intro3;
                    startFadeToNext();
                    break;
                case GameIntroState::Intro3:
                    pendingState = GameIntroState::Intro4;
                    startFadeToNext();
                    break;
                case GameIntroState::Intro4:
                    pendingState = GameIntroState::FadingOut;
                    startFadeToNext();
                    break;
                default:
                    break;
            }
        }
    }

    //=============================================================================
    // Update animation
    //=============================================================================
    void GameIntroAnimation::update(float dt) {
        if (skipRequested && !isFading) {
            pendingState = GameIntroState::FadingOut;
            startFadeToNext();
            skipRequested = false;
        }
        
        switch (state) {
            case GameIntroState::Intro1:
                mainText->setString("Your ship's engines have failed.\nNow you find yourself stranded in a strange galaxy...\n(Press ENTER)");
                break;
            case GameIntroState::Intro2:
                mainText->setString("You must repair and upgrade your ship\nto be able to return home with your dog.\n(Press ENTER)");
                break;
            case GameIntroState::Intro3:
                mainText->setString("There is only one way: you will need to earn money.\n(Press ENTER)");
                break;
            case GameIntroState::Intro4:
                mainText->setString("You will have to negotiate with different planets\nuntil you buy everything you need for your ship.\n\nGood luck on your mission, Juan Jose Josefino.\n(Press ENTER to begin your journey)");
                break;
            default:
                break;
        }
        
        if (isFading) {
            if (pendingState == GameIntroState::FadingOut) {
                fadeAlpha += fadeSpeed * dt;
                if (fadeAlpha >= 255.f) {
                    fadeAlpha = 255.f;
                    isFading = false;
                    state = GameIntroState::Finished;
                    if (backgroundMusic) backgroundMusic->stop();
                }
                fadeOverlay->setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
            } else {
                fadeAlpha += fadeSpeed * dt;
                
                if (fadeAlpha >= 255.f) {
                    fadeAlpha = 255.f;
                    isFading = false;
                    
                    switch (pendingState) {
                        case GameIntroState::Intro2:
                            backgroundSprite->setTexture(bgTex2, true);
                            break;
                        case GameIntroState::Intro3:
                            backgroundSprite->setTexture(bgTex3, true);
                            break;
                        case GameIntroState::Intro4:
                            backgroundSprite->setTexture(bgTex4, true);
                            break;
                        default:
                            break;
                    }
                    state = pendingState;
                    isFading = true;
                    pendingState = GameIntroState::Finished;
                } else {
                    fadeOverlay->setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
                }
            }
            
            if (pendingState == GameIntroState::Finished && isFading && fadeAlpha > 0) {
                fadeAlpha -= fadeSpeed * dt * 2;
                if (fadeAlpha <= 0) {
                    fadeAlpha = 0;
                    isFading = false;
                    pendingState = GameIntroState::Intro1;
                }
                fadeOverlay->setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
            }
        }
        
        if (state == GameIntroState::Intro1 || 
            state == GameIntroState::Intro2 || 
            state == GameIntroState::Intro3 || 
            state == GameIntroState::Intro4) {
            sf::FloatRect bounds = mainText->getLocalBounds();
            mainText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
            mainText->setPosition({640.f, 600.f});
        }
    }

    //=============================================================================
    // Draw all elements
    //=============================================================================
    void GameIntroAnimation::draw(sf::RenderWindow& window) {
        if (!backgroundSprite || !dialogueBox || !mainText) return;

        window.draw(*backgroundSprite);
        
        if (state == GameIntroState::Intro1 || 
            state == GameIntroState::Intro2 || 
            state == GameIntroState::Intro3 || 
            state == GameIntroState::Intro4) {
            window.draw(*dialogueBox);
            window.draw(*mainText);
        }
        
        if (skipButton && skipButtonText) {
            window.draw(*skipButton);
            window.draw(*skipButtonText);
        }
        
        if (isFading && fadeOverlay) {
            window.draw(*fadeOverlay);
        }
    }

    //=============================================================================
    // Reset animation
    //=============================================================================
    void GameIntroAnimation::reset() {
        state = GameIntroState::Intro1;
        fadeAlpha = 255.f;
        isFading = true;
        pendingState = GameIntroState::Intro1;
        skipRequested = false;
        isHoveringSkip = false;
        
        if (backgroundMusic) {
            backgroundMusic->stop();
        }
        
        backgroundSprite->setTexture(bgTex1, true);
        fadeOverlay->setFillColor(sf::Color(0, 0, 0, 255));
        mainText->setString("");
    }

    //=============================================================================
    // Check if music is playing
    //=============================================================================
    bool GameIntroAnimation::isMusicPlaying() const {
        return backgroundMusic && backgroundMusic->getStatus() == sf::SoundSource::Status::Playing;
    }

}