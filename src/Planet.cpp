#include "Planet.hpp"

Planet::Planet(std::string name, std::string description) : name(name), description(description) {
    // Aquí podrías generar aleatoriamente los atributos del planeta o pasarlos como parámetros
    orbit = rand() % 10 + 1; // Orbita entre 1 y 10
    techLevel = rand() % 10 + 1; // Nivel tecnológico entre 1 y 10
    securityLevel = rand() % 10 + 1; // Nivel de seguridad entre 1 y 10
    resourceAbundance = rand() % 10 + 1; // Abundancia de recursos entre 1 y 10ç
    luxuryDemand = rand() % 10 + 1; // Demanda de lujo entre 1 y 10
    moonCount = rand() % 5; // Entre 0 y 4 lunas
}

std::string Planet::getName() const {
    return name;
}

std::string Planet::getDescription() const {
    return description;
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