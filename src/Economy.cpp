#include "Economy.hpp"

double Economy::calculatePrice(const Item& item, const Planet& planet) {
    double price = item.getPrice();
    double modifier = 1.0;

    // 1. Influencia por Categoría y Atributos del Planeta
    if (item.isTechnology()) {
        // A mayor nivel tecnológico, más oferta, menor precio
        modifier -= (planet.getTechLevel() - 5) * 0.05; 
    }
    
    if (item.isResource()) {
        // Si hay abundancia de recursos, el precio baja
        modifier -= (planet.getResourceAbundance() - 5) * 0.08;
    }

    if (item.isLuxury()) {
        // A mayor demanda de lujo, el precio sube
        modifier += (planet.getLuxuryDemand() - 5) * 0.15;
    }

    if (item.isIllegal()) {
        // En planetas seguros (Security 10), el contrabando es carísimo por el riesgo
        modifier += (planet.getSecurityLevel() - 5) * 0.25;
    }

    // 2. Influencia por Rareza (los items legendarios son menos volátiles)
    if (item.getRarity() == Rarity::Legendary) {
        modifier = std::max(1.0, modifier); // Nunca bajan de su precio base
    }

    switch (planet.getEvent()) {
        case PlanetEvent::War:
            if (item.isTechnology()) modifier *= 2.0; // Las armas/tecnología suben
            break;

        case PlanetEvent::Plague:
            if (item.isMedical()) modifier *= 3.0; // Las medicinas se disparan
            if (item.isFood()) modifier *= 1.5;
            break;

        case PlanetEvent::Famine:
            if (item.isFood()) modifier *= 2.5; // El hambre sube el precio de la comida
            break;

        case PlanetEvent::TechBoom:
            if (item.isTechnology()) modifier *= 0.5; // La tecnología se abarata por la sobreoferta
            break;
        
        default: break;
    }

    return price * std::max(0.1, modifier); // El precio nunca puede ser 0 o negativo
}

std::vector<std::string> Economy::generateMarketStock(const Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    
    std::vector<std::string> marketStock;

    for (auto const& [name, item] : catalog) {
        bool appears = false;

        // Lógica de aparición basada en atributos:
        if (item->isIllegal() && planet.getSecurityLevel() < 4) appears = true; // Solo en planetas peligrosos
        if (item->isTechnology() && planet.getTechLevel() > 3) appears = true;  // Solo si tienen algo de tech
        if (item->isResource()) appears = true; // Los recursos están en todos lados
        if (item->isFood()) appears = true;     // Todos comen
        
        // Probabilidad extra por rareza
        int roll = rand() % 100;
        if (item->getRarity() == Rarity::Legendary && roll > 95) appears = true;
        else if (item->getRarity() == Rarity::Common && roll > 30) appears = true;

        if (appears) {
            marketStock.push_back(name);
        }
    }

    return marketStock;
}