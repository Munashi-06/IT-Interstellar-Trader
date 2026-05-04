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
    // 1. Asegurar que el vector tenga exactamente 100 slots (solo la primera vez)
    if (localStock.size() != 100) {
        localStock.resize(100, std::nullopt);
    }

    // 2. Paso de "Producción": Aumentar cantidad de items que YA están en los slots
    for (auto& slot : localStock) {
        if (slot.has_value()) {
            const auto& itemData = catalog.at(slot->itemID);
            if (rand() % 100 < 20) {
                int boost = (itemData->getRarity() == Rarity::Common) ? 5 : 1;
                slot->quantity = std::min(slot->quantity + boost, slot->maxStackSize);
            }
        }
    }

    // 3. Paso de "Nuevas Mercancías": Si hay slots vacíos, intentar llenarlos
    for (auto& slot : localStock) {
        if (!slot.has_value()) {
            // Elegimos un item al azar del catálogo global
            auto it = catalog.begin();
            std::advance(it, rand() % catalog.size());
            const auto& [id, itemPtr] = *it;

            // Verificar si este ítem ya existe en algún otro slot del mercado
            bool alreadyInStock = false;
            for (const auto& existingSlot : localStock) {
                if (existingSlot.has_value() && existingSlot->itemID == id) {
                    alreadyInStock = true;
                    break;
                }
            }

            if (!alreadyInStock && this->canBuyItem(*itemPtr)) {
                // --- CALCULO DINAMICO DE PROBABILIDAD ---
                int probability = 0;

                // A. Base por Rareza
                switch (itemPtr->getRarity()) {
                    case Rarity::Common:    probability = 45; break; // Muy frecuente
                    case Rarity::Rare:      probability = 20; break;
                    case Rarity::Exotic:    probability = 8;  break;
                    case Rarity::Legendary: probability = 2;  break; // Súper difícil de ver
                    case Rarity::Quest:     probability = 0;  break; // Nunca se venden en tiendas normales
                }

                // B. Modificadores por Categoría y Atributos del Planeta (Asumiendo atributos del 1 al 10)
                if (itemPtr->isTechnology()) {
                    probability += (this->techLevel * 2); // Planetas de alta tecnología venden más tecnología
                } 
                else if (itemPtr->isResource()) {
                    probability += (this->resourceAbundance * 2); // Mucho recurso natural = más oferta
                } 
                else if (itemPtr->isLuxury()) {
                    probability += (this->luxuryDemand * 2); // Si hay demanda, los mercaderes lo traen
                } 
                else if (itemPtr->isMedical()) {
                    probability += (this->medicalTech * 2); // Alta tecnología médica = más medicinas
                } 
                else if (itemPtr->isIllegal()) {
                    // La seguridad funciona al revés: Menos seguridad = Más chance de ver contrabando
                    probability += ((10 - this->securityLevel) * 3); 
                } 
                else if (itemPtr->isFood()) {
                    probability += 10; // La comida siempre tiene un pequeño bono, todos comen
                }

                // Asegurarnos de que ítems de rareza alta no se vuelvan comunes por los bonos
                if (itemPtr->getRarity() == Rarity::Legendary && probability > 5) probability = 5;
                if (itemPtr->getRarity() == Rarity::Exotic && probability > 15) probability = 15;

                // Cantidad de items que aparecen en el slot, influenciada por la rareza (items más raros suelen aparecer en menor cantidad)
                int quantity = 1;
                if (itemPtr->getRarity() == Rarity::Common) quantity = (rand() % 50) + 1; // Entre 1 y 50
                else if (itemPtr->getRarity() == Rarity::Rare) quantity = (rand() % 20) + 1; // Entre 1 y 20
                else if (itemPtr->getRarity() == Rarity::Exotic) quantity = (rand() % 11) + 2; // Entre 2 y 12
                else if (itemPtr->getRarity() == Rarity::Legendary) quantity = (rand() % 5) + 1; // Entre 1 y 5

                // --- TIRADA DE DADOS ---
                if (probability > 0 && (rand() % 100 < probability)) {
                    slot = ItemStack{ id, quantity, itemPtr->getMaxStackSize(), itemPtr->getBasePrice() }; 
                }
            }
        }
    }
}

float Planet::getItemPrice(const std::string& itemID, const std::unordered_map<std::string, std::unique_ptr<Item>>& globalCatalog) const {
    const auto& item = globalCatalog.at(itemID);
    float price = item->getBasePrice();
    float modifier = 1.0f;

    // --- Influencia de Atributos del Planeta ---
    if (item->isTechnology()) {
        // A más nivel tecnológico, más oferta, precio más bajo (-20% máximo)
        modifier -= (this->techLevel / 50.0f); 
    }
    if (item->isFood() && this->resourceAbundance > 7) {
        modifier -= 0.15f; // Comida barata en planetas agrícolas
    }
    if (item->isLuxury()) {
        // Alta demanda de lujo sube el precio
        modifier += (this->luxuryDemand / 20.0f);
    }

    // --- Influencia de Eventos ---
    switch (this->currentEvent) {
        case PlanetEvent::War:
            if (item->isMedical()) modifier += 1.5f;   // Medicina sube
            if (item->isTechnology()) modifier += 0.5f; // Componentes suben
            break;
        case PlanetEvent::Plague:
            if (item->isMedical()) modifier += 2.0f;
            if (item->isFood()) modifier += 0.4f;
            break;
        case PlanetEvent::Famine:
            if (item->isFood()) modifier += 2.5f;
            break;
        case PlanetEvent::TechBoom:
            if (item->isTechnology()) modifier -= 0.3f; // Exceso de oferta
            break;
        default: break;
    }

    return price * std::max(0.1f, modifier); // Nunca precio negativo o 0
}

bool Planet::addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice) {
    // Intentar apilar en slots existentes
    for (auto& slot : localStock) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity += qty;
            return true;
        }
    }
    // Si no se pudo apilar, buscar un slot vacío
    for (auto& slot : localStock) {
        if (!slot.has_value()) {
            slot = ItemStack{ itemID, qty, maxStackSize, buyPrice };
            return true;
        }
    }
    return false; // El mercado está lleno (100 slots)
}

void Planet::removeItem(const std::string& itemID, int qty) {
    for (auto& slot : localStock) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity -= qty;
            if (slot->quantity <= 0) {
                slot = std::nullopt; // Libera el slot si se queda en 0
            }
            return;
        }
    }
}

bool Planet::canBuyItem(const Item& item) const {
    // Si yo soy Tech 10 y me vendes Tech vieja (Common/Rare)
    if (item.isTechnology() && this->techLevel > 8 && item.getRarity() < Rarity::Exotic) {
        // Probabilidad minima de que un planeta avanzado quiera tecnología obsoleta, pero no imposible
        return (rand() % 100) < 9; // 9% de chance de que la compre
    }

    // Un planeta muy seguro no compra contrabando
    if (item.isIllegal() && this->securityLevel > 8) {
        // Probabilidad mínima de que un planeta con alta seguridad quiera items ilegales, pero no imposible
        return (rand() % 100) < 5; // 5% de chance de que la compre
    }

    return true; 
}

void Planet::setHighlighted(bool h) { 
        highlighted = h;
    }

bool Planet::isPointNear(const sf::Vector2f& point, const sf::Vector2f& planetPos) const{
    float distance = std::sqrt(std::pow(point.x - planetPos.x, 2)+(std::pow(point.y - planetPos.y, 2)));
    return distance < 50.0f;
}

void Planet::updateScale(float deltaTime){
    if (!sprite || !texture) return;

    sf::Vector2u texSize = texture->getSize();
    float targetScale = highlighted ? 60.f : 40.f;
    targetScale = targetScale / std::max(texSize.x, texSize.y);
    float currentScale = sprite->getScale().x;

    if(std::abs(currentScale - targetScale) < 0.001f){
        sprite->setScale({targetScale, targetScale});
        return;
    }

    float speed = 10.0f;
    float newScale = currentScale + (targetScale - currentScale) * speed * deltaTime;

    if (highlighted){
        newScale = std::min(newScale, targetScale);
    } else {
        newScale = std::max(newScale, targetScale);
    }
}