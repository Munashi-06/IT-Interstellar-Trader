#include "Systems/AssetManager.hpp"

std::unordered_map<std::string, sf::Texture> AssetManager::textures;
std::unordered_map<std::string, sf::Font> AssetManager::fonts;

bool AssetManager::loadAll() {
    bool success = true;

    // --- LOAD FONTS ---
    sf::Font mainFont;
    if (!mainFont.openFromFile("assets/fonts/04B_03__.TTF")) {
        std::cerr << "[FILES ERROR] No se pudo cargar la fuente principal.\n";
        success = false;
    }
    else {
        fonts["main_font"] = mainFont;
    }

    // --- AUXILIARY FUNCTION FOR LOADING TEXTURES CLEANLY ---
    auto loadTex = [&](const std::string& name, const std::string& path) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "[FILES ERROR] No se pudo cargar textura: " << path << "\n";
            success = false;
        } else {
            textures[name] = tex;
        }
    };

    // --- LOAD TEXTURES ---
    loadTex("bg_main", "assets/mainMenu_background.jpeg");
    loadTex("bg_settings", "assets/settingsMenu_background.png");
    loadTex("alert_icon", "assets/alert_icon.png");
    // Add all future textures here! e.g., loadTex(“player”, “assets/player.png”);

    std::cout << "[FILES] Todos los recursos cargados exitosamente.\n";
    return success;
}

const sf::Texture& AssetManager::getTexture(const std::string& name) {
    return textures.at(name); // .at() is safe and will throw an error if you use the wrong name
}

sf::Font& AssetManager::getFont(const std::string& name) {
    return fonts.at(name);
}