#include "Player.hpp"
#include <cmath>


Player::Player(float x, float y, const std::string& texturePath) 
    : hitbox({30.f, 35.f})
{
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Error cargando la textura del jugador");
    }
    else {
        // 1. Vincular la textura AQUÍ, una vez cargada
        sprite = std::make_unique<sf::Sprite>(texture);
        sprite->setPosition({x, y});

        // 2. Resetear el Rect de la textura (por si acaso)
        sprite->setTextureRect(sf::IntRect({0, 0}, { (int)texture.getSize().x, (int)texture.getSize().y }));

        sf::Vector2u size = texture.getSize();
        
        // 3. El origen debe ser la mitad del tamaño real de la imagen
        sprite->setOrigin({ static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f });

        // 4. Reescalado
        float rescale = 60.f;
        float scale = rescale / static_cast<float>(size.x);
        sprite->setScale({ scale, scale });
        
        sprite->setPosition({ x, y });
    }

    // Configuración del hitbox
    
    // Controno rojo para depuración (quitar en producción)
    // hitbox.setOutlineColor(sf::Color::Red);
    // hitbox.setOutlineThickness(2);

    hitbox.setFillColor(sf::Color::Transparent);
    // Centramos la hitbox también para que coincida con el sprite
    hitbox.setOrigin(hitbox.getSize() / 2.f);
    hitbox.setPosition(sprite->getPosition());
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(hitbox); // Dibuja el hitbox para depuración (quitar en producción)
    window.draw(*sprite);
}

void Player::update(float deltaTime) {
    if (!sprite) return;

    // Fricción espacial sutil
    velocity *= std::pow(drag, deltaTime * 60.f);

    // Rotación Interpolada (Suave)
    float currentRot = sprite->getRotation().asDegrees();
    float diff = targetRotation - currentRot;
    
    // Normalizar ángulo para que siempre gire por el camino más corto
    while (diff > 180) diff -= 360;
    while (diff < -180) diff += 360;

    // Aplicar giro suave
    sprite->rotate(sf::degrees(diff * rotationSpeed * deltaTime));

    // Mover la nave con la velocidad acumulada
    sprite->move(velocity * deltaTime);

    hitbox.setPosition(sprite->getPosition());
}

void Player::move(sf::Vector2f direction, float deltaTime) {
    // 1. Rotación: Solo cambiamos el objetivo de giro si hay input
    if (direction.x != 0 || direction.y != 0) {
        targetRotation = (std::atan2(direction.y, direction.x) * 180.f / 3.141592f) + 90.f;
        
        // 2. Aceleración: En lugar de usar 'direction' (input), 
        // usamos el ángulo actual del sprite para empujar la nave.
        float angleRad = (sprite->getRotation().asDegrees() - 90.f) * 3.141592f / 180.f;
        sf::Vector2f thrustDir(std::cos(angleRad), std::sin(angleRad));

        velocity += thrustDir * acceleration * deltaTime;
    }
}