#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

class AssetManager {
private:
    static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, sf::Font> fonts;

public:
    static bool loadAll();

    static const sf::Texture& getTexture(const std::string& name);
    static sf::Font& getFont(const std::string& name);
};