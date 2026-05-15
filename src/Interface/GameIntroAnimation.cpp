#include "Interface/GameIntroAnimation.hpp"
#include <iostream>

namespace Interface {

    GameIntroAnimation::GameIntroAnimation(float width, float height)
        : state(GameIntroState::Intro1),
          fadeAlpha(255.f),
          fadeSpeed(200.f),
          isFading(true),  // Comienza con fade in
          pendingState(GameIntroState::Intro1),
          screenWidth(width),
          screenHeight(height)
    {
        fadeOverlay.setSize({1280.f, 720.f});
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, 255));
    }

    bool GameIntroAnimation::loadAssets(const sf::Font& font) {
        // --- 1. CREAR LOS OBJETOS QUE NO DEPENDEN DE TEXTURAS ---
        dialogueBox = std::make_unique<sf::RectangleShape>(sf::Vector2f(1000.f, 150.f));
        dialogueBox->setFillColor(sf::Color(0, 0, 0, 210));
        dialogueBox->setOutlineThickness(2);
        dialogueBox->setOutlineColor(sf::Color::White);
        dialogueBox->setOrigin({500.f, 75.f});
        dialogueBox->setPosition({640.f, 600.f});
        
        mainText = std::make_unique<sf::Text>(font, "", 26);
        mainText->setFillColor(sf::Color::White);

        backgroundMusic = std::make_unique<sf::Music>();

        // --- 2. CARGAR TEXTURA PRINCIPAL PRIMERO ---
        if (!bgTex1.loadFromFile("assets/anim04.png")) {
            std::cerr << "[GameIntro] Error: assets/anim04.png not found" << std::endl;
            return false; // Si falla, abortamos (El check en el draw evitará el crasheo)
        }
        
        // --- 3. CREAR EL SPRITE CON LA TEXTURA (Regla de SFML 3) ---
        backgroundSprite = std::make_unique<sf::Sprite>(bgTex1);
        backgroundSprite->setScale({1280.f / bgTex1.getSize().x, 720.f / bgTex1.getSize().y});

        // --- 4. CARGAR EL RESTO DE TEXTURAS ---
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
        
        // --- 5. LOAD MUSIC ---
        if (!backgroundMusic->openFromFile("assets/audio/undertale_dogsong.ogg")) {
            std::cerr << "[GameIntro] Error: assets/audio/undertale_dogsong.ogg not found" << std::endl;
            return false; 
        }
        else {
            backgroundMusic->setLooping(true);
        }
        
        return true;
    }

    void GameIntroAnimation::start() {
        reset();
        state = GameIntroState::Intro1;
        backgroundSprite->setTexture(bgTex1, true);
        isFading = true;
        fadeAlpha = 255.f;
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, 255));
        
        if (backgroundMusic) {
            backgroundMusic->stop();
            backgroundMusic->play();
        }
    }

    void GameIntroAnimation::startFadeToNext() {
        isFading = true;
        fadeAlpha = 0.f;
    }

    void GameIntroAnimation::handleInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Enter && !isFading) {
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

    void GameIntroAnimation::update(float dt) {
        // Actualizar textos según el estado actual
        switch (state) {
            case GameIntroState::Intro1:
                mainText->setString("Your ship's engines have failed,\nnow you find yourself stranded in a strange galaxy...\n(Press Enter)");
                break;
            case GameIntroState::Intro2:
                mainText->setString("You must repair and upgrade your ship,\nto be able to return home with your dog.\n(Press Enter)");
                break;
            case GameIntroState::Intro3:
                mainText->setString("There is only one way, you will need to find money.\n(Press Enter)");
                break;
            case GameIntroState::Intro4:
                mainText->setString("You will have to negotiate with different planets,\nuntil you buy what you need for your ship\nGood luck on your mission Juan Jose Josefino.\n(Press Enter to begin your journey)");
                break;
            default:
                break;
        }
        
        // Manejar el fade
        if (isFading) {
            if (pendingState == GameIntroState::FadingOut) {
                // Desvanecimiento a negro (aumenta alpha)
                fadeAlpha += fadeSpeed * dt;
                if (fadeAlpha >= 255.f) {
                    fadeAlpha = 255.f;
                    isFading = false;
                    state = GameIntroState::Finished;
                    if (backgroundMusic) backgroundMusic->stop();
                }
                fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
            } else {
                // Transición entre fondos
                fadeAlpha += fadeSpeed * dt;
                
                if (fadeAlpha >= 255.f) {
                    // Completar transición
                    fadeAlpha = 255.f;
                    isFading = false;
                    
                    // Cambiar al siguiente fondo
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
                    
                    // Hacer fade out del overlay
                    isFading = true;
                    pendingState = GameIntroState::Finished;  // Marcador para segundo fade
                } else {
                    // Durante el fade: overlay negro con alpha creciente
                    fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
                }
            }
            
            // Segundo fade (después de cambiar la imagen)
            if (pendingState == GameIntroState::Finished && isFading && fadeAlpha > 0) {
                fadeAlpha -= fadeSpeed * dt * 2;
                if (fadeAlpha <= 0) {
                    fadeAlpha = 0;
                    isFading = false;
                    pendingState = GameIntroState::Intro1;
                }
                fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
            }
        }
        
        // Centrar texto
        if (state == GameIntroState::Intro1 || 
            state == GameIntroState::Intro2 || 
            state == GameIntroState::Intro3 || 
            state == GameIntroState::Intro4) {
            sf::FloatRect bounds = mainText->getLocalBounds();
            mainText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
            mainText->setPosition({640.f, 600.f});
        }
    }

    void GameIntroAnimation::draw(sf::RenderWindow& window) {
        // SAFETY: If something is null, we abort the drawing instead of crashing
        if (!backgroundSprite || !dialogueBox || !mainText) return;

        // Draw background
        window.draw(*backgroundSprite);
        
        // Draw dialog
        if (state == GameIntroState::Intro1 || 
            state == GameIntroState::Intro2 || 
            state == GameIntroState::Intro3 || 
            state == GameIntroState::Intro4) {
            window.draw(*dialogueBox);
            window.draw(*mainText);
        }
        
        // Draw the fade overlay if it is active
        if (isFading) {
            window.draw(fadeOverlay);
        }
    }

    void GameIntroAnimation::reset() {
        state = GameIntroState::Intro1;
        fadeAlpha = 255.f;
        isFading = true;
        pendingState = GameIntroState::Intro1;
        
        if (backgroundMusic) {
            backgroundMusic->stop();
        }
        
        backgroundSprite->setTexture(bgTex1, true);
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, 255));
        mainText->setString("");
    }

    bool GameIntroAnimation::isMusicPlaying() const {
        return backgroundMusic && backgroundMusic->getStatus() == sf::SoundSource::Status::Playing;
    }

}