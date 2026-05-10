#include "Systems/Economy.hpp"

double Economy::calculatePrice(const Item& item, const Planet& planet) {
    double price = item.getPrice();
    double modifier = 1.0;

    // 1. Influence by Category and Planet Attributes
    if (item.isTechnology()) {
        // Higher tech level means more supply, lower price
        modifier -= (planet.getTechLevel() - 5) * 0.05; 
    }
    
    if (item.isResource()) {
        // If there is an abundance of resources, the price drops
        modifier -= (planet.getResourceAbundance() - 5) * 0.08;
    }

    if (item.isLuxury()) {
        // Higher luxury demand means the price rises
        modifier += (planet.getLuxuryDemand() - 5) * 0.15;
    }

    if (item.isIllegal()) {
        // In safe planets (Security 10), smuggling is very expensive due to risk
        modifier += (planet.getSecurityLevel() - 5) * 0.25;
    }

    // 2. Influence by Rarity (legendary items are less volatile)
    if (item.getRarity() == Rarity::Legendary) {
        modifier = std::max(1.0, modifier); // They never drop below their base price
    }

    switch (planet.getEvent()) {
        case PlanetEvent::War:
            if (item.isTechnology()) modifier *= 2.0; // Weapons/technology prices rise
            break;

        case PlanetEvent::Plague:
            if (item.isMedical()) modifier *= 3.0; // Medicine prices skyrocket
            if (item.isFood()) modifier *= 1.5;
            break;

        case PlanetEvent::Famine:
            if (item.isFood()) modifier *= 2.5; // Hunger increases the price of food
            break;

        case PlanetEvent::TechBoom:
            if (item.isTechnology()) modifier *= 0.5; // Technology becomes cheaper due to oversupply
            break;
        
        default: break;
    }

    return price * std::max(0.1, modifier); // Price can never be 0 or negative
}

std::vector<std::string> Economy::generateMarketStock(const Planet& planet, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    
    std::vector<std::string> marketStock;

    for (auto const& [name, item] : catalog) {
        bool appears = false;

        // Appearance logic based on attributes:
        if (item->isIllegal() && planet.getSecurityLevel() < 4) appears = true; // Only on dangerous planets
        if (item->isTechnology() && planet.getTechLevel() > 3) appears = true;  // Only if they have some tech
        if (item->isResource()) appears = true; // Resources are everywhere
        if (item->isFood()) appears = true;     // Everyone eats
        
        // Extra probability by rarity
        int roll = rand() % 100;
        if (item->getRarity() == Rarity::Legendary && roll > 95) appears = true;
        else if (item->getRarity() == Rarity::Common && roll > 30) appears = true;

        if (appears) {
            marketStock.push_back(name);
        }
    }

    return marketStock;
}