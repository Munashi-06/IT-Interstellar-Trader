#include "Entities/Planet.hpp"
#include <cmath>

Planet::Planet() : name("Empty"), description("Empty"), localStock({}), EVENT_DURATION(0), orbit(0), techLevel(0), securityLevel(0), resourceAbundance(0), luxuryDemand(0), moonCount(0), medicalTech(0), currentEvent(PlanetEvent::None)
{
    // Default constructor
}

Planet::Planet(std::string n, std::string d, int orb, int tech, int sec, int res, int lux, int moon, int med)
    : name(n), description(d), EVENT_DURATION(0), orbit(orb), techLevel(tech), securityLevel(sec), resourceAbundance(res), luxuryDemand(lux), moonCount(moon), medicalTech(med)
{
    // Initialize the stock hash table
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
    // Copy constructor
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
        std::cerr <<"Could not load texture for " << name << ":" << path << std::endl;
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
        case PlanetEvent::None: return "None";
        case PlanetEvent::War: return "War";
        case PlanetEvent::Plague: return "Plague";
        case PlanetEvent::TechBoom: return "Technology Boom";
        case PlanetEvent::Famine: return "Famine";
        case PlanetEvent::Piracy: return "Piracy";
        default: return "Unknown";
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
            currentEvent = PlanetEvent::None; // The event ends
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
    // 1. Ensure the vector has exactly 100 slots (only the first time)
    if (localStock.size() != 100) {
        localStock.resize(100, std::nullopt);
    }

    // 2. "Production" Step: Increase the quantity of items ALREADY in the slots
    for (auto& slot : localStock) {
        if (slot.has_value()) {
            const auto& itemData = catalog.at(slot->itemID);
            if (rand() % 100 < 20) {
                int boost = (itemData->getRarity() == Rarity::Common) ? 5 : 1;
                slot->quantity = std::min(slot->quantity + boost, slot->maxStackSize);
            }
        }
    }

    std::vector<std::string> catalogKeys;
    for (const auto& pair : catalog) {
        catalogKeys.push_back(pair.first);
    }
    StockSorter::hybridSort(catalogKeys.begin(), catalogKeys.end(), [](const std::string& a, const std::string& b) {
        return a < b;
    });

    // 3. "New Merchandise" Step: If there are empty slots, try to fill them
    for (auto& slot : localStock) {
        if (!slot.has_value()) {
            // Elegimos un ID al azar de nuestra lista ordenada determinista
            std::string id = catalogKeys[rand() % catalogKeys.size()];
            const auto& itemPtr = catalog.at(id);

            // Check if this item already exists in any other slot of the market
            bool alreadyInStock = false;
            for (const auto& existingSlot : localStock) {
                if (existingSlot.has_value() && existingSlot->itemID == id) {
                    alreadyInStock = true;
                    break;
                }
            }

            if (!alreadyInStock && this->canBuyItem(*itemPtr)) {
                // --- DYNAMIC PROBABILITY CALCULATION ---
                int probability = 0;

                // A. Base by Rarity
                switch (itemPtr->getRarity()) {
                    case Rarity::Common:    probability = 45; break; // Very frequent
                    case Rarity::Rare:      probability = 20; break;
                    case Rarity::Exotic:    probability = 8;  break;
                    case Rarity::Legendary: probability = 2;  break; // Super hard to see
                    case Rarity::Quest:     probability = 0;  break; // Never sold in normal shops
                }

                // B. Modifiers by Category and Planet Attributes (Assuming attributes from 1 to 10)
                if (itemPtr->isTechnology()) {
                    probability += (this->techLevel * 2); // High-tech planets sell more technology
                } 
                else if (itemPtr->isResource()) {
                    probability += (this->resourceAbundance * 2); // High natural resources = more supply
                } 
                else if (itemPtr->isLuxury()) {
                    probability += (this->luxuryDemand * 2); // If there's demand, merchants bring it
                } 
                else if (itemPtr->isMedical()) {
                    probability += (this->medicalTech * 2); // High medical tech = more medicines
                } 
                else if (itemPtr->isIllegal()) {
                    // Security works in reverse: Less security = More chance to see contraband
                    probability += ((10 - this->securityLevel) * 3); 
                } 
                else if (itemPtr->isFood()) {
                    probability += 10; // Food always has a small bonus, everyone eats
                }

                // Ensure high rarity items don't become common due to bonuses
                if (itemPtr->getRarity() == Rarity::Legendary && probability > 5) probability = 5;
                if (itemPtr->getRarity() == Rarity::Exotic && probability > 15) probability = 15;

                // Quantity of items appearing in the slot, influenced by rarity (rarer items usually appear in smaller quantities)
                int quantity = 1;
                if (itemPtr->getRarity() == Rarity::Common) quantity = (rand() % 50) + 1; // Between 1 and 50
                else if (itemPtr->getRarity() == Rarity::Rare) quantity = (rand() % 20) + 1; // Between 1 and 20
                else if (itemPtr->getRarity() == Rarity::Exotic) quantity = (rand() % 11) + 2; // Between 2 and 12
                else if (itemPtr->getRarity() == Rarity::Legendary) quantity = (rand() % 5) + 1; // Between 1 and 5

                // --- DICE ROLL ---
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

    // --- Influence of Planet Attributes ---
    if (item->isTechnology()) {
        // Higher tech level means more supply, lower price (-20% maximum)
        modifier -= (this->techLevel / 50.0f); 
    }
    if (item->isFood() && this->resourceAbundance > 7) {
        modifier -= 0.15f; // Cheap food on agricultural planets
    }
    if (item->isLuxury()) {
        // High luxury demand raises the price
        modifier += (this->luxuryDemand / 20.0f);
    }

    // --- Influence of Events ---
    switch (this->currentEvent) {
        case PlanetEvent::War:
            if (item->isMedical()) modifier += 1.5f;   // Medicine goes up
            if (item->isTechnology()) modifier += 0.5f; // Components go up
            break;
        case PlanetEvent::Plague:
            if (item->isMedical()) modifier += 2.0f;
            if (item->isFood()) modifier += 0.4f;
            break;
        case PlanetEvent::Famine:
            if (item->isFood()) modifier += 2.5f;
            break;
        case PlanetEvent::TechBoom:
            if (item->isTechnology()) modifier -= 0.3f; // Excess supply
            break;
        default: break;
    }

    return price * std::max(0.1f, modifier); // Never a negative or 0 price
}

bool Planet::addItem(const std::string& itemID, int qty, int maxStackSize, float buyPrice) {
    // Try to stack in existing slots
    for (auto& slot : localStock) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity += qty;
            return true;
        }
    }
    // If it couldn't be stacked, look for an empty slot
    for (auto& slot : localStock) {
        if (!slot.has_value()) {
            slot = ItemStack{ itemID, qty, maxStackSize, buyPrice };
            return true;
        }
    }
    return false; // The market is full (100 slots)
}

void Planet::removeItem(const std::string& itemID, int qty) {
    for (auto& slot : localStock) {
        if (slot.has_value() && slot->itemID == itemID) {
            slot->quantity -= qty;
            if (slot->quantity <= 0) {
                slot = std::nullopt; // Free the slot if it reaches 0
            }
            return;
        }
    }
}

void Planet::sortByName(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByName(localStock, ascending, catalog);
}

void Planet::sortByCategory(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByCategory(localStock, ascending, catalog);
}

void Planet::sortByQuality(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByQuality(localStock, ascending, catalog);
}

void Planet::sortByPrice(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByPrice(localStock, ascending, catalog);
}

void Planet::sortByQuantity(bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
    StockSorter::sortByQuantity(localStock, ascending, catalog);
}

bool Planet::canBuyItem(const Item& item) const {
    // If I am Tech 10 and you sell me old Tech (Common/Rare)
    if (item.isTechnology() && this->techLevel > 8 && item.getRarity() < Rarity::Exotic) {
        // Minimum probability that an advanced planet wants obsolete technology, but not impossible
        return (rand() % 100) < 9; // 9% chance that it buys it
    }

    // A very secure planet does not buy contraband
    if (item.isIllegal() && this->securityLevel > 8) {
        // Minimum probability that a high-security planet wants illegal items, but not impossible
        return (rand() % 100) < 5; // 5% chance that it buys it
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
    }
    else {
        newScale = std::max(newScale, targetScale);
    }
    sprite->setScale({newScale, newScale});
}