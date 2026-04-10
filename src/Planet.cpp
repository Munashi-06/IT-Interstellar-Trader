#include "Planet.hpp"

Planet::Planet() : name("Empty"), description("Empty"), orbit(0), techLevel(0), securityLevel(0), resourceAbundance(0), luxuryDemand(0), moonCount(0), medicalTech(0)
{
    // Constructor por defecto
}

Planet::Planet(std::string n, std::string d, int orb, int tech, int sec, int res, int lux, int moon, int med)
    : name(n), description(d), orbit(orb), techLevel(tech), securityLevel(sec), resourceAbundance(res), luxuryDemand(lux), moonCount(moon), medicalTech(med)
{}

std::string Planet::getName() const {
    return name;
}

std::string Planet::getDescription() const {
    return description;
}

PlanetEvent Planet::getEvent() const {
    return currentEvent;
}

void Planet::setEvent(PlanetEvent e) {
    currentEvent = e;
}

int Planet::getEventDuration() const {
    return EVENT_DURATION;
}

void Planet::setEventDuration(int duration) {
    EVENT_DURATION = duration;
}

void Planet::decreaseEventDuration() {
    if (currentEvent != PlanetEvent::None) {
        EVENT_DURATION--;
        if (EVENT_DURATION <= 0) {
            currentEvent = PlanetEvent::None; // El evento termina
        }
    }
}

int Planet::getOrbit() const {
    return orbit;
}

int Planet::getTechLevel() const {
    return techLevel;
}

int Planet::getSecurityLevel() const {
    return securityLevel;
}

int Planet::getResourceAbundance() const {
    return resourceAbundance;
}

int Planet::getLuxuryDemand() const {
    return luxuryDemand;
}

int Planet::getMoonCount() const {
    return moonCount;
}

int Planet::getMedicalTech() const {
    return medicalTech;
}

void Planet::refreshMarket(const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    // 1. Calcular ocupación actual
    int currentTotal = 0;
    for (auto const& [name, qty] : localStock) currentTotal += qty;
    if (currentTotal >= MAX_STOCK) return; // Límite de almacén planetario

    // 2. Producción basada en especialización
    for (auto const& [name, item] : catalog) {
        bool canProduce = false;

        // Filtros de Especialidad
        if (item->isFood() && this->resourceAbundance > 6) {
            // Si hay hambruna, la producción de comida cae a 0
            if (this->currentEvent != PlanetEvent::Famine) canProduce = true;
        }
        if (item->isTechnology() && this->techLevel > 7) canProduce = true;
        if (item->isMedic() && this->medicalTech > 7) canProduce = true;

        if (canProduce) {
            // Probabilidad según rareza (más común = más probable)
            int chance = (item->getRarity() == Rarity::Common) ? 40 : (item->getRarity() == Rarity::Rare) ? 25 : (item->getRarity() == Rarity::Exotic) ? 10 : 5;
            if (rand() % 100 < chance) {
                localStock[name] = std::min(localStock[name] + 1, 10); 
            }
        }
    }

    // 3. Eventos que afectan el mercado
    // Esto lo trabajaran ustedes muchachos yo ya me frite el cerebro con esto, pero podrían hacer algo como:
    // Si hay guerra, reducir stock de lujo y tecnología
    // Si hay peste, reducir stock de comida y medicina
    // Si hay auge tecnológico, aumentar stock de tecnología
    // etc... De nuevo, esto es solo un ejemplo de cómo podrían implementar la influencia de eventos en el mercado,
    // no es algo que tengan que hacer sí o sí... Suerte, Angel.
}

bool Planet::canBuyItem(const Item& item) const {
    // Si yo soy Tech 10 y me vendes Tech vieja (Common/Rare)
    if (item.isTechnology() && this->techLevel > 8 && item.getRarity() < Rarity::Exotic) {
        return false; // "No me interesa tu basura antigua, viajero"
    }

    // Un planeta muy seguro no compra contrabando
    if (item.isIllegal() && this->securityLevel > 8) {
        return false; // "Aquí respetamos la ley"
    }

    return true; 
}