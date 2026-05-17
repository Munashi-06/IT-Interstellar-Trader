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
          screenWidth(width),
          thankYouTimer(0.f),
          musicDuration(0.f),
          creditsScrollY(720.f)
    {}

    //=============================================================================
    // FUNCIÓN PARA CENTRAR TEXTO
    //=============================================================================
    void IntroAnimation::centerText(sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        float centerX = bounds.position.x + bounds.size.x / 2.f;
        text.setOrigin({centerX, 0.f});
        text.setPosition({screenWidth / 2.f, y});
    }

    void IntroAnimation::centerText(sf::Text& text, const sf::RectangleShape& rect) {
        sf::FloatRect bounds = text.getLocalBounds();
        float textCenterX = bounds.position.x + bounds.size.x / 2.f;
        float textCenterY = bounds.position.y + bounds.size.y / 2.f;
        
        float rectCenterX = rect.getPosition().x + rect.getSize().x / 2.f;
        float rectCenterY = rect.getPosition().y + rect.getSize().y / 2.f;
        
        text.setOrigin({textCenterX, textCenterY});
        text.setPosition({rectCenterX, rectCenterY});
    }

    //=============================================================================
    // CREAR LÍNEAS DE CRÉDITOS
    //=============================================================================
    void IntroAnimation::createCreditsLines(const sf::Font& font) {
        creditLines.clear();
        
        std::vector<std::string> lines = {
            "=============================================================================",
            "IT: INTERSTELLAR TRADER",
            "Space Trading Simulator",
            "=============================================================================",
            "",
            "DEVELOPED BY",
            "ESCUADRON ALPHA",
            "BUENA MARAVILLA BUENA ONDA LOBO SALVAJE",
            "",
            "MAIN CREDITS",
            "",
            "[ PROJECT DIRECTION ]",
            "Project Manager: Angel Garcia",
            "Lead Game Designer: Abraham Vergara",
            "Technical Director: Reinier Manrique",
            "",
            "[ ENGINEERING & CODE ]",
            "Software Architecture: Angel Garcia",
            "State Machine & Core Loop: Angel Garcia",
            "State System: Abraham Vergara",
            "Physics Engine (Orbits): Reinier Manrique",
            "Random Event System: Abraham Vergara",
            "Dialogue System: Angel Garcia",
            "Save/Load System: Reinier Manrique",
            "",
            "[ ECONOMY & TRADING ]",
            "Head of Trading Strategy: Abraham Vergara",
            "Local Market System: Abraham Vergara",
            "Item Catalog: Angel Garcia",
            "Pricing Algorithms: Abraham Vergara",
            "Event Modifiers: Reinier Manrique",
            "Supply/Demand System: Abraham Vergara",
            "",
            "[ UI / UX ]",
            "Lead Interface Designer: Reinier Manrique",
            "HUD & Menus: Reinier Manrique",
            "Visual Inventory System: Angel Garcia",
            "Priority Radar (Heap): Abraham Vergara",
            "Animations & Transitions: Reinier Manrique",
            "",
            "[ UPGRADE TREE ]",
            "Progression Design: Angel Garcia",
            "Propulsion Tree: Abraham Vergara",
            "Logistics Tree: Angel Garcia",
            "Commerce Tree: Abraham Vergara",
            "State Persistence: Reinier Manrique",
            "Event Damage Mechanic: Angel Garcia",
            "",
            "[ COMBAT & PIRATES ]",
            "Encounter Design: Abraham Vergara",
            "Combat System: Angel Garcia",
            "Bribery Mechanics: Abraham Vergara",
            "Surrender System: Reinier Manrique",
            "",
            "[ ART & AUDIO ]",
            "Visual Art Director: Reinier Manrique",
            "Planet Design: Reinier Manrique",
            "Ship Design: Reinier Manrique",
            "Visual Effects (Particles): Reinier Manrique",
            "UI Graphics: Reinier Manrique",
            "Icons & Textures: Reinier Manrique",
            "Audio FX Integration: Angel Garcia",
            "Ambient Music: Angel Garcia",
            "",
            "[ SYSTEMS & SUPPORT ]",
            "Systems Architecture: Project IT 2026",
            "QA Testing: Squadron Alpha",
            "Game Balance: Abraham Vergara",
            "Optimization: Reinier Manrique",
            "Technical Documentation: Angel Garcia",
            "",
            "=============================================================================",
            "THANK YOU FOR PLAYING!",
            "May your trading routes be prosperous and safe",
            "=============================================================================",
            "",
            "IT: Interstellar Trader - Where every decision counts",
            "Version 1.0 - Project IT 2026"
        };
        
        float currentY = 0.f;
        
        for (const auto& line : lines) {
            auto text = std::make_unique<sf::Text>(font, line, 24);
            text->setFillColor(sf::Color::Yellow);
            text->setOutlineColor(sf::Color::Black);
            text->setOutlineThickness(1);
            
            if (line == "IT: INTERSTELLAR TRADER") {
                text->setCharacterSize(36);
                text->setFillColor(sf::Color::Cyan);
            }
            else if (line.find("[") != std::string::npos) {
                text->setCharacterSize(28);
                text->setFillColor(sf::Color(255, 200, 100));
            }
            else if (line.empty()) {
                text->setString(" ");
                text->setCharacterSize(20);
            }
            else if (line == "=============================================================================") {
                text->setCharacterSize(20);
                text->setFillColor(sf::Color(150, 150, 150));
            }
            
            centerText(*text, currentY);
            creditLines.push_back(std::move(text));
            currentY += 35.f;
        }
    }

    //=============================================================================
    // Load all assets
    //=============================================================================
    bool IntroAnimation::loadAssets(const sf::Font& font) {
        if (!bgTex.loadFromFile("assets/anim02.png") ||
            !anim01Tex.loadFromFile("assets/anim01.png")) {
            return false;
        }
        
        backgroundMusic = std::make_unique<sf::Music>();
        if (!backgroundMusic->openFromFile("assets/audio/end.ogg")) {
            std::cerr << "Error: assets/audio/end.ogg not found" << std::endl;
        } else {
            backgroundMusic->setLooping(false);
            musicDuration = backgroundMusic->getDuration().asSeconds();
        }

        backgroundSprite = std::make_unique<sf::Sprite>(bgTex);
        personSprite = std::make_unique<sf::Sprite>(personTex);
        anim01Sprite = std::make_unique<sf::Sprite>(anim01Tex);

        backgroundSprite->setScale({1280.f / bgTex.getSize().x, 720.f / bgTex.getSize().y});
        anim01Sprite->setScale({1280.f / anim01Tex.getSize().x, 720.f / anim01Tex.getSize().y});
        anim01Sprite->setColor(sf::Color(255, 255, 255, 0));

        // Caja negra centrada en pantalla (Ancho: 1000, Alto: 150)
        dialogueBox = std::make_unique<sf::RectangleShape>(sf::Vector2f(1000.f, 150.f));
        dialogueBox->setFillColor(sf::Color(0, 0, 0, 210));
        dialogueBox->setOutlineThickness(2);
        dialogueBox->setOutlineColor(sf::Color::White);
        dialogueBox->setOrigin({500.f, 75.f});
        dialogueBox->setPosition({640.f, 550.f});

        mainText = std::make_unique<sf::Text>(font, "", 26);
        mainText->setFillColor(sf::Color::White);

        instructionText = std::make_unique<sf::Text>(font, "Press ENTER to continue...", 20);
        instructionText->setFillColor(sf::Color::Yellow);
        instructionText->setOutlineColor(sf::Color::Black);
        instructionText->setOutlineThickness(1);
        instructionText->setStyle(sf::Text::Italic);
        centerText(*instructionText, 685.f);

        createCreditsLines(font);

        thankYouText = std::make_unique<sf::Text>(font, "THANK YOU FOR PLAYING", 52);
        thankYouText->setFillColor(sf::Color::Yellow);
        thankYouText->setOutlineColor(sf::Color::Black);
        thankYouText->setOutlineThickness(2);
        centerText(*thankYouText, 280.f);

        escuadronText = std::make_unique<sf::Text>(font, 
            "Developed by: Escuadron Alpha\nBuena Maravilla Buena Onda Lobo Salvaje", 26);
        escuadronText->setFillColor(sf::Color::White);
        escuadronText->setOutlineColor(sf::Color::Black);
        escuadronText->setOutlineThickness(1);
        escuadronText->setStyle(sf::Text::Italic);
        centerText(*escuadronText, 380.f);

        exitInstructionText = std::make_unique<sf::Text>(font, "Press ENTER to continue...", 18);
        exitInstructionText->setFillColor(sf::Color(150, 150, 150));
        exitInstructionText->setOutlineColor(sf::Color::Black);
        exitInstructionText->setOutlineThickness(1);
        centerText(*exitInstructionText, 620.f);

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
                state = AnimState::ThankYou;
                thankYouTimer = 0.f;
            }
            else if (state == AnimState::ThankYou) {
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
            mainText->setString("Thus, Juan Jose Josefino was able to\nreturn to his home planet...");
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
                creditsScrollY = 720.f;
            }
            personSprite->setColor(sf::Color(255, 255, 255, 255 - static_cast<uint8_t>(fadeAlpha)));
            anim01Sprite->setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(fadeAlpha)));
        }
        else if (state == AnimState::Credits) {
            creditsScrollY -= scrollSpeed * dt;
            float y = creditsScrollY;
            for (auto& line : creditLines) {
                line->setPosition({screenWidth / 2.f, y});
                y += 35.f;
            }
            
            float lastLineY = creditsScrollY + (creditLines.size() * 35.f);
            if (lastLineY < 0) {
                state = AnimState::ThankYou;
                thankYouTimer = 0.f;
            }
        }

        // 🛠️ ACOMODADO MANUAL SOLICITADO:
        // Ajustamos dinámicamente el origen según los bounds reales del texto de turno
        // y lo posicionamos fijo en (640.f, 550.f) para que calce perfecto en el centro de tu caja negra.
        sf::FloatRect bounds = mainText->getLocalBounds();
        mainText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        mainText->setPosition({640.f, 550.f});
    }

    //=============================================================================
    // Draw all elements
    //=============================================================================
    void IntroAnimation::draw(sf::RenderWindow& window) {
        window.draw(*backgroundSprite);
        
        if (state == AnimState::Transition || state == AnimState::Credits || state == AnimState::ThankYou) {
            window.draw(*anim01Sprite);
        }

        if (state != AnimState::Credits && state != AnimState::ThankYou && state != AnimState::Finished) {
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
            for (auto& line : creditLines) {
                window.draw(*line);
            }
        }

        if (state == AnimState::ThankYou) {
            window.draw(*thankYouText);
            window.draw(*escuadronText);
            window.draw(*exitInstructionText);
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
        thankYouTimer = 0.f;
        creditsScrollY = 720.f;
        if (backgroundMusic) backgroundMusic->stop();
        personSprite->setColor(sf::Color::White);
        anim01Sprite->setColor(sf::Color(255, 255, 255, 0));
        centerText(*instructionText, 685.f);
    }
}   