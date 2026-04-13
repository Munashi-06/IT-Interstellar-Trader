#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Item.hpp"

static int MAX_STOCK = 100; // Cantidad máxima total de items que un planeta puede tener en su mercado

enum class PlanetEvent { None, War, Plague, TechBoom, Famine, Piracy };

class Planet {
private:
    std::string name;
    std::string description;
    PlanetEvent currentEvent = PlanetEvent::None;
    int EVENT_DURATION;
    std::unordered_map<std::string, int> localStock; // ItemName -> Cantidad
    int orbit; // Representa la orbita del planeta, entre 1 y 10 (1 es el más cercano a la estrella, 10 el más lejano)
    int techLevel; // Nivel tecnológico del planeta, podría influir en los precios y tipos de items disponibles
    int securityLevel; // Nivel de seguridad del planeta, podría influir en la presencia de items ilegales y su precio
    int resourceAbundance; // Abundancia de recursos naturales, podría influir en la oferta de items de categoría Resource
    int luxuryDemand; // Demanda de bienes de lujo, podría influir en la oferta y precio de items de categoría Luxury
    int moonCount; // Número de lunas, podría influir en la presencia de estaciones espaciales y su oferta de items
    int medicalTech; // Nivel de tecnología médica, podría influir en la oferta y precio de items de categoría Medical
public:
    Planet();
    Planet(std::string n, std::string d, int orb, int tech, int sec, int res, int lux, int moon, int med);
    Planet(const Planet& other); // Constructor de copia
    Planet& operator=(const Planet& other); // Operador de asignación

    std::string getName() const;
    std::string getDescription() const;
    PlanetEvent getEvent() const;
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

    void refreshMarket(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog);
    bool canBuyItem(const Item& item) const; // Lógica de "necesito esto?"
    // Metodos para calcular la influencia de sus atributos
    
};