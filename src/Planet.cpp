#include "Planet.hpp"

Planet::Planet() : name("Empty"), description("Empty"), localStock({}), EVENT_DURATION(0), orbit(0), techLevel(0), securityLevel(0), resourceAbundance(0), luxuryDemand(0), moonCount(0), medicalTech(0), currentEvent(PlanetEvent::None)
{
    // Constructor por defecto
}

Planet::Planet(std::string n, std::string d, int orb, int tech, int sec, int res, int lux, int moon, int med)
    : name(n), description(d), EVENT_DURATION(0), orbit(orb), techLevel(tech), securityLevel(sec), resourceAbundance(res), luxuryDemand(lux), moonCount(moon), medicalTech(med)
{
    // Inicializamos la tabla hash de stock
    loadTexture();
}

Planet& Planet::operator=(const Planet& other) {
    if (this != &other) {
        name = other.name;
        description = other.description;
        currentEvent = other.currentEvent;
        EVENT_DURATION = other.EVENT_DURATION;
        localStock = other.localStock;
        orbit = other.orbit;
        techLevel = other.techLevel;
        securityLevel = other.securityLevel;
        resourceAbundance = other.resourceAbundance;
        luxuryDemand = other.luxuryDemand;
        moonCount = other.moonCount;
        medicalTech = other.medicalTech;
        baseScale = other.baseScale;
        highlighted = other.highlighted;

        if (other.texture && other.sprite){
            texture = std::make_unique<sf::Texture>(*other.texture);
            sprite = std::make_unique<sf::Sprite>(*texture);
            sprite->setTextureRect(other.sprite->getTextureRect());
            sprite->setScale(other.sprite->getScale());
            sprite->setOrigin(other.sprite->getOrigin());
        } else {
            texture.reset();
            sprite.reset();
        }
            
    }
    return *this;
}

Planet::Planet(const Planet& other)
    : name(other.name), description(other.description), currentEvent(other.currentEvent), EVENT_DURATION(other.EVENT_DURATION),
      localStock(other.localStock), orbit(other.orbit), techLevel(other.techLevel), securityLevel(other.securityLevel),
      resourceAbundance(other.resourceAbundance), luxuryDemand(other.luxuryDemand), moonCount(other.moonCount),
      medicalTech(other.medicalTech), baseScale(other.baseScale), highlighted(other.highlighted)
{
    // Constructor de copia
    if (other.texture && other.sprite){
        texture = std::make_unique<sf::Texture>(*other.texture);
        sprite = std::make_unique<sf::Sprite>(*texture);
        sprite->setTextureRect(other.sprite->getTextureRect());
        sprite->setScale(other.sprite->getScale());
        sprite->setOrigin(other.sprite->getOrigin());
    }
}

void Planet::loadTexture() {
    std::string path = "assets/planets/" + name + ".png";
    texture = std::make_unique<sf::Texture>();

    if(texture->loadFromFile(path)){
        sprite = std::make_unique<sf::Sprite>(*texture);
        sf::Vector2u texSize = texture->getSize();
        baseScale = 40.f / std::max(texSize.x, texSize.y);
        sprite->setScale({baseScale, baseScale});
        sprite->setOrigin({static_cast<float>(texSize.x)/2.f, static_cast<float>(texSize.y)/2.f});
    } else {
        std::cerr <<"No se pudo cargar textura para " << name << ":" << path << std::endl;
        texture.reset();
        sprite.reset();
    }
}

std::string Planet::getName() const {
    return name;
}

std::string Planet::getDescription() const {
    return description;
}

PlanetEvent Planet::getEvent() const {
    return currentEvent;
}

std::string Planet::getEventName() const {
    switch (currentEvent) {
        case PlanetEvent::None: return "Ninguno";
        case PlanetEvent::War: return "Guerra";
        case PlanetEvent::Plague: return "Plaga";
        case PlanetEvent::TechBoom: return "Auge Tecnologico";
        case PlanetEvent::Famine: return "Hambruna";
        case PlanetEvent::Piracy: return "Pirateria";
        default: return "Desconocido";
    }
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