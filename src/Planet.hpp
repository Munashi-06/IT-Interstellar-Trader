#pragma once
#include <iostream>
#include <string>

enum class PlanetEvent { None, War, Plague, TechBoom, Famine };

class Planet {
private:
    std::string name;
    std::string description;
    PlanetEvent currentEvent = PlanetEvent::None;
    int orbit; // Representa la orbita del planeta, entre 1 y 10 (1 es el más cercano a la estrella, 10 el más lejano)
    int techLevel; // Nivel tecnológico del planeta, podría influir en los precios y tipos de items disponibles
    int securityLevel; // Nivel de seguridad del planeta, podría influir en la presencia de items ilegales y su precio
    int resourceAbundance; // Abundancia de recursos naturales, podría influir en la oferta de items de categoría Resource
    int luxuryDemand; // Demanda de bienes de lujo, podría influir en la oferta y precio de items de categoría Luxury
    int moonCount; // Número de lunas, podría influir en la presencia de estaciones espaciales y su oferta de items
public:
    Planet(std::string name, std::string description);
    std::string getName() const;
    std::string getDescription() const;
    int getOrbit() const;
    int getTechLevel() const;
    int getSecurityLevel() const;
    int getResourceAbundance() const;
    int getLuxuryDemand() const;
    int getMoonCount() const;
    void setEvent(PlanetEvent e) { currentEvent = e; }
    PlanetEvent getEvent() const { return currentEvent; }

    // Metodos para calcular la influencia de sus atributos
    
};