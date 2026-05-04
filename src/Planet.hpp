#pragma once
#include "Item.hpp"
#include "StockSorter.hpp"
#include <SFML/Graphics.hpp>


static int MAX_STOCK = 100; // Cantidad máxima total de items que un planeta puede tener en su mercado

enum class PlanetEvent { None, War, Plague, TechBoom, Famine, Piracy };

class Planet {
private:
    std::string name;
    std::string description;
    PlanetEvent currentEvent = PlanetEvent::None;
    int EVENT_DURATION;
    std::vector<std::optional<ItemStack>> localStock; // Stock local del planeta, mapeado por ID de item
    int orbit; // Representa la orbita del planeta, entre 1 y 10 (1 es el más cercano a la estrella, 10 el más lejano)
    int techLevel; // Nivel tecnológico del planeta, podría influir en los precios y tipos de items disponibles
    int securityLevel; // Nivel de seguridad del planeta, podría influir en la presencia de items ilegales y su precio
    int resourceAbundance; // Abundancia de recursos naturales, podría influir en la oferta de items de categoría Resource
    int luxuryDemand; // Demanda de bienes de lujo, podría influir en la oferta y precio de items de categoría Luxury
    int moonCount; // Número de lunas, podría influir en la presencia de estaciones espaciales y su oferta de items
    int medicalTech; // Nivel de tecnología médica, podría influir en la oferta y precio de items de categoría Medical

    std::unique_ptr<sf::Texture> texture; // Textura del planeta
    std::unique_ptr<sf::Sprite> sprite; // Sprite del planeta
    float baseScale = 1.0f;
    bool highlighted = false;

public:
    Planet();
    Planet(std::string n, std::string d, int orb, int tech, int sec, int res, int lux, int moon, int med);
    Planet(const Planet& other); // Constructor de copia
    Planet& operator=(const Planet& other); // Operador de asignación

    // Getters y Setters
    std::string getName() const;
    std::string getDescription() const;
    PlanetEvent getEvent() const;
    std::string getEventName() const;
    void setEvent(PlanetEvent e);
    int getEventDuration() const;
    void setEventDuration(int duration);
    void decreaseEventDuration();
    int getOrbit() const;
    int getTechLevel() const;
    int getSecurityLevel() const;
    int getResourceAbundance() const;
    int getLuxuryDemand() const;
    int getMoonCount() const;
    int getMedicalTech() const;
    const std::vector<std::optional<ItemStack>>& getLocalStock() const noexcept { return localStock; }
    sf::Sprite* getSprite() { return sprite.get(); }
    const sf::Sprite* getSprite() const { return sprite.get(); }
    void setHighlighted(bool h);

    void refreshMarket(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog); // Actualiza el stock local basado en la especialización del planeta y eventos actuales
    bool canBuyItem(const Item& item) const; // Lógica de "necesito esto?"
    float getItemPrice(const std::string& itemID, const std::unordered_map<std::string, std::unique_ptr<Item>>& globalCatalog) const; // Calcula el precio de un item basado en la oferta/demanda y eventos actuales
    bool addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice);
    void removeItem(const std::string& itemID, int qty);
    void sortByName(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByCategory(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByQuality(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByPrice(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    void sortByQuantity(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);

    void loadTexture();
    bool hasSprite() const { return sprite != nullptr; }
    bool isHighlighted() const { return highlighted; }
    bool isPointNear(const sf::Vector2f& point, const sf::Vector2f& planetPos) const;
    void updateScale(float deltaTime);
};