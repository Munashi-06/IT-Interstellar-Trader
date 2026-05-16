#include "Systems/UpgradeManager.hpp"

// Default constructor (we will implement initTrees later)
UpgradeManager::UpgradeManager() {}

bool UpgradeManager::purchaseUpgrade(std::shared_ptr<BinNode<Upgrade>> node, std::shared_ptr<BinNode<Upgrade>> siblingNode, float& playerMoney) {
    if (!node || K(node).status != UpgradeStatus::AVAILABLE) {
        return false; // Cannot purchase a locked, already purchased, or null node
    }

    if (playerMoney >= K(node).cost) {
        // 1. Deduct money
        playerMoney -= K(node).cost;
        
        // 2. Change status and apply the effect
        K(node).status = UpgradeStatus::PURCHASED;
        K(node).applyEffect();
        
        // 3. Handle mutually exclusive sibling
        if (K(node).isMutuallyExclusive && siblingNode != nullptr) {
            K(siblingNode).status = UpgradeStatus::BLOCKED_BY_CHOICE;
        }

        // 4. Unlock children so they become AVAILABLE
        if (L(node) && K(L(node)).status == UpgradeStatus::LOCKED) {
            K(L(node)).status = UpgradeStatus::AVAILABLE;
        }
        if (R(node) && K(R(node)).status == UpgradeStatus::LOCKED) {
            K(R(node)).status = UpgradeStatus::AVAILABLE;
        }

        return true;
    }
    
    return false; // Not enough money to purchase this upgrade
}

void UpgradeManager::initTrees(Player& player) {
// ==========================================
// 1. PROPULSION TREE INITIALIZATION
// ==========================================
#pragma region Propulsion Tree Initialization

    // ROOT NODE: Ion Thrusters
    // Effect: Increases the max orbit reach by 1
    Upgrade ionThruster(
        "prop_1", // ID
        "Ion Thrusters", // Name 
        "Improves thrust efficiency. Unlocks Orbit 5 & 2.", // Description
        1000.0f, // Cost
        false, // Not mutually exclusive (you need all engines)
        [&player]() {
            player.setMaxOrbit(5);
            player.setMinOrbit(2);
            // player.setTravelSpeed(player.getTravelSpeed() * 1.2f);
        }
    );
    
    propulsionTree = std::make_shared<BinNode<Upgrade>>(std::move(ionThruster));
    K(propulsionTree).status = UpgradeStatus::AVAILABLE; 


    // LEFT CHILD: Thermal Shielding
    // Effect: Allows travel to Orbit 2
    Upgrade thermalShield(
        "prop_heat", 
        "Thermal Shielding", 
        "Resists intense solar radiation. Allows travel to Orbit 2.", 
        3000.0f, 
        false, 
        [&player]() {
            player.setMinOrbitReach(2); // Unlocks the inner solar system
        }
    );
    
    L(propulsionTree) = std::make_shared<BinNode<Upgrade>>(std::move(thermalShield));


    // RIGHT CHILD OF THERMAL SHIELD: Cryogenic Heaters
    // Effect: Prevents ship systems from freezing. Allows travel to outer orbits (6 nd 7)
    Upgrade cryoHeater(
        "prop_cold", 
        "Cryogenic Heaters", 
        "Prevents freezing in deep space. Allows travel to Orbits 6 & 7.", 
        4000.0f, 
        false, 
        [&player]() {
            player.setMaxOrbitReach(7);
        }
    );
    
    R(L(propulsionTree)) = std::make_shared<BinNode<Upgrade>>(std::move(cryoHeater));

    // LEFT CHILD OF THERMAL SHIELD: Advanced Heat Dispersion
    // Effect: Allows travel to Orbit 1
    Upgrade heatDispersion(
        "prop_heat_adv", 
        "Advanced Heat Dispersion", 
        "Further improves thermal management. Allows travel to Orbit 1.", 
        5000.0f, 
        false,
        [&player]() {
            player.setMinOrbitReach(1); // Unlocks the innermost orbit
        }
    );

    L(L(propulsionTree)) = std::make_shared<BinNode<Upgrade>>(std::move(heatDispersion));

    // LEFT CHILD OF CRYO HEATER: Advanced Cold Resistance
    // Effect: Allows travel to Orbits 8 to 10
    Upgrade coldResistance(
        "prop_cold_adv", 
        "Advanced Cold Resistance", 
        "Further improves cold resistance. Allows travel to Orbits 8 to MAX.", 
        6500.0f, 
        false,
        [&player]() {
            player.setMaxOrbitReach(10);
        }
    );

    L(R(L(propulsionTree))) = std::make_shared<BinNode<Upgrade>>(std::move(coldResistance));

    // RIGHT CHILD OF ION THRUSTER: Nuclear Propulsion
    // Effect: Unlocks Orbit 7 and increases travel speed by 50%
    Upgrade nuclearPropulsion(
        "prop_2", 
        "Nuclear Propulsion", 
        "Next-gen propulsion system. Unlocks Orbit 7 and 1. Increases travel speed by 50%.", 
        3500.0f, 
        false, 
        [&player]() {
            player.setMaxOrbit(7);
            player.setMinOrbit(1);
            // player.setTravelSpeed(player.getTravelSpeed() * 1.5f);
        }
    );

    R(propulsionTree) = std::make_shared<BinNode<Upgrade>>(std::move(nuclearPropulsion));

    // RIGHT CHILD OF NUCLEAR PROPULSION: Fusion Propulsion
    // Effect: Unlocks Orbits 8 & 9 and increases travel speed by an extra 25%
    Upgrade fusionPropulsion(
        "prop_3", 
        "Fusion Propulsion", 
        "Enhanced nuclear propulsion. Unlocks Orbits 8 & 9 and increases travel speed by an extra 25%.", 
        5000.0f, 
        false, 
        [&player]() {
            player.setMaxOrbit(9);
            // player.setTravelSpeed((player.getTravelSpeed()/(1*5f))*1.75f);
        }
    );

    R(R(propulsionTree)) = std::make_shared<BinNode<Upgrade>>(std::move(fusionPropulsion));

    // RIGHT CHILD OF FUSION PROPULSION: Antimatter Propulsion
    // Effect: Unlocks Orbit 10 and increases travel speed by an extra 50%
    Upgrade antimatterPropulsion(
        "prop_4", 
        "Antimatter Propulsion", 
        "State-of-the-art propulsion. Unlocks Orbit 10 and increases travel speed by an extra 50%.", 
        8000.0f, 
        false, 
        [&player]() {
            player.setMaxOrbit(10);
            // player.setTravelSpeed((player.getTravelSpeed()/(1*5f))*2.25f);
        }
    );

    R(R(R(propulsionTree))) = std::make_shared<BinNode<Upgrade>>(std::move(antimatterPropulsion));

    // RIGHT CHILD OF ANTIMATTER PROPULSION: Warp Drive
    // Effect: Allows Player to return home and travel speed becomes irrelevant
    Upgrade warpDrive(
        "prop_5", 
        "Warp Drive", 
        "Revolutionary propulsion. Allows instant travel between any orbits. YOU ARE FREE TO GO HOME.", 
        15000.0f, 
        false, 
        [&player]() {
            player.setMinOrbit(1);
            player.setMaxOrbit(10);
            // player.setTravelSpeed(9999.f);
            player.setHasWarpDrive(true);
        }
    );

    R(R(R(R(propulsionTree)))) = std::make_shared<BinNode<Upgrade>>(std::move(warpDrive));
#pragma endregion

// ==========================================
// 2. LOGISTICS TREE INITIALIZATION
// ==========================================
#pragma region Logistics Tree Initialization

    // ROOT NODE: Advanced Radar
    Upgrade advancedRadar(
        "log_1", 
        "Advanced Radar", 
        "Upgrades basic sensors. Shows info About planets events. Essential for interstellar commerce.", 
        800.0f, 
        false, 
        [&player]() {
            player.setHasAdvancedRadar(true); 
        }
    );
    
    logisticsTree = std::make_shared<BinNode<Upgrade>>(std::move(advancedRadar));
    K(logisticsTree).status = UpgradeStatus::AVAILABLE;

    // ================= BRANCH A: THE HAULER (LEFT) - ORIGINAL =================

    Upgrade heavyFreighter(
        "log_cargo_1", 
        "Heavy Freighter Hull", 
        "Expands cargo bay. +10 Inventory Slots. Lacks space for advanced computers.", 
        2500.0f, 
        true, 
        [&player]() {
            player.addInventoryCapacity(10); 
        }
    );
    
    L(logisticsTree) = std::make_shared<BinNode<Upgrade>>(std::move(heavyFreighter));

    Upgrade reinforcedCargo(
        "log_cargo_3", 
        "Reinforced Cargo Holder", 
        "Strengthens cargo hold. +25 Slots.", 
        4000.0f, 
        true, 
        [&player]() {
            player.addInventoryCapacity(30);
        }
    );

    L(L(logisticsTree)) = std::make_shared<BinNode<Upgrade>>(std::move(reinforcedCargo));

    Upgrade smugglerCompartment(
        "log_cargo_2", 
        "Smuggler's Compartment", 
        "Adds hidden compartments. +10 Slots.", 
        4500.0f, 
        true, 
        [&player]() {
            player.addInventoryCapacity(10);
            player.setIsSmuggler(true);
        }
    );

    R(L(logisticsTree))= std::make_shared<BinNode<Upgrade>>(std::move(smugglerCompartment));

    Upgrade quantumStorage(
        "log_cargo_4", 
        "Quantum Storage", 
        "State-of-the-art storage. +50 Slots. Uses quantum tech to maximize space.", 
        10000.0f, 
        false, 
        [&player]() {
            player.addInventoryCapacity(40);
        }
    );

    Upgrade quantumStorage2 = quantumStorage;

    L(L(L(logisticsTree))) = std::make_shared<BinNode<Upgrade>>(std::move(quantumStorage));
    L(R(L(logisticsTree))) = std::make_shared<BinNode<Upgrade>>(std::move(quantumStorage2));

    // ================= BRANCH B: THE BROKER (RIGHT) - NUEVA ESTRUCTURA =================

    // RIGHT CHILD (NUEVO PADRE): Deep Market Scanners
    Upgrade marketScanners(
        "log_scanners",
        "Deep Market Scanners", 
        "Upgrades sensors to detect restricted and exotic goods in local markets. No room for more cargo", 
        2000.0f, 
        true, 
        [&player]() {
            player.levelUpShip();
        }
    );

    R(logisticsTree) = std::make_shared<BinNode<Upgrade>>(std::move(marketScanners));

    // RIGHT-LEFT CHILD: Insider Trading Link (Desplazado hacia abajo a la izquierda)
    Upgrade insiderTrading(
        "log_intel_2", 
        "Insider Trading Link", 
        "Tap into corporate networks. Planet events last 30 seconds longer.", 
        2000.0f, 
        true, 
        [&player]() {
            player.setEventDurationBonus(true);
        }
    );

    L(R(logisticsTree)) = std::make_shared<BinNode<Upgrade>>(std::move(insiderTrading));

    // RIGHT-LEFT-LEFT CHILD: Market Predictor Algorithm (Hijo de Insider Trading)
    Upgrade marketPredictor(
        "log_intel_1", 
        "Market Predictor", 
        "Advanced AI intercepts comms. Increases Planet Event frequency. No room for extra cargo.", 
        2500.0f, 
        false,
        [&player]() {
            player.setEventFrequencyBonus(true);
        }
    );

    R(L(R(logisticsTree))) = std::make_shared<BinNode<Upgrade>>(std::move(marketPredictor));

    // RIGHT-LEFT-RIGHT CHILD: Manipulator Chip (Hijo de Insider Trading)
    Upgrade manipulatorChip(
        "log_intel_3", 
        "Manipulator Chip", 
        "Illicit tech manipulates local conditions. Allows triggering one event every 4 minutes. Only can trigger War, Plague or Tech Boom events. Costs 5000 credits per use.", 
        3000.0f, 
        true,
        [&player]() {
            player.setIsManipulator(true);
        }
    );

    R(R(logisticsTree)) = std::make_shared<BinNode<Upgrade>>(std::move(manipulatorChip));

#pragma endregion

// ==========================================
// 3. TRADING TREE INITIALIZATION
// ==========================================
#pragma region Trading Tree Initialization

    // ROOT NODE: Universal Translator
    // Effect: Unlocks the trading tree and gives a basic global discount.
    Upgrade universalTranslator(
        "trade_1", 
        "Universal Translator", 
        "Breaks communication barriers. Grants a flat 5% discount on all purchases.", 
        1000.0f, 
        false, // Root is never mutually exclusive
        [&player]() {
            player.setDiscount(0.05f); // 5% discount
        }
    );
    
    tradingTree = std::make_shared<BinNode<Upgrade>>(std::move(universalTranslator));
    K(tradingTree).status = UpgradeStatus::AVAILABLE;


    // ================= BRANCH A: THE CORPORATE (LEFT) =================

    // LEFT CHILD: Corporate License
    Upgrade corporateLicense(
        "trade_corp_1", 
        "Corporate License", 
        "Official trading permits. 15% better prices for Tech and Medical goods. Blocks Black Market Contacts", 
        3500.0f, 
        true, // Choosing this blocks the Underworld branch
        [&player]() {
            player.setCorporateLicense(true); 
        }
    );
    
    L(tradingTree) = std::make_shared<BinNode<Upgrade>>(std::move(corporateLicense));

    // LEFT-LEFT CHILD: Monopoly Charter
    Upgrade monopolyCharter(
        "trade_corp_2", 
        "Monopoly Charter", 
        "Total market control. Sell prices for ALL legal goods are increased by 20%.", 
        8000.0f, 
        false, 
        [&player]() {
            player.setMonopolyCharter(true);
        }
    );

    L(L(tradingTree)) = std::make_shared<BinNode<Upgrade>>(std::move(monopolyCharter));


    // ================= BRANCH B: THE UNDERWORLD (RIGHT) =================

    // RIGHT CHILD: Black Market Contacts
    Upgrade blackMarketContacts(
        "trade_under_1", 
        "Black Market Contacts", 
        "Access to the underworld. 25% better prices for Illegal goods. Blocks Corporate License.", 
        3500.0f, 
        true, // Choosing this blocks the Corporate branch
        [&player]() {
            player.setBlackMarketContacts(true);
        }
    );

    R(tradingTree) = std::make_shared<BinNode<Upgrade>>(std::move(blackMarketContacts));

    // RIGHT-RIGHT CHILD: Syndicate Boss
    Upgrade syndicateBoss(
        "trade_under_2", 
        "Syndicate Boss", 
        "You rule the black market. Illegal goods can be sold at a massive 50% premium.", 
        8000.0f, 
        false, 
        [&player]() {
            player.setSyndicateBoss(true);
        }
    );

    R(R(tradingTree)) = std::make_shared<BinNode<Upgrade>>(std::move(syndicateBoss));

    // RIGHT-LEFT & LEFT-RIGHT CHILD: VIP Vendor License (Visibility Upgrade 2)
    // Effect: Grants exclusive access to Legendary items and hidden syndicate stashes.
    Upgrade vipLicense(
        "trade_vip_2", 
        "VIP Vendor License", 
        "Grants exclusive access to Legendary items and hidden syndicate stashes.", 
        5000.0f, 
        false, 
        [&player]() {
            player.levelUpShip();
        }
    );

    Upgrade vipLicense2 = vipLicense;

    R(L(tradingTree)) = std::make_shared<BinNode<Upgrade>>(std::move(vipLicense));
    L(R(tradingTree)) = std::make_shared<BinNode<Upgrade>>(std::move(vipLicense2));

#pragma endregion
}

// ==========================================
// SAVING AND LOADING LOGIC
// ==========================================

void UpgradeManager::collectPurchased(std::shared_ptr<BinNode<Upgrade>> node, std::vector<std::string>& list) const {
    if (!node) return;
    if (K(node).status == UpgradeStatus::PURCHASED) {
        list.push_back(K(node).id);
    }
    // Pre-order traversal ensures that the parent is stored before the children
    collectPurchased(L(node), list);
    collectPurchased(R(node), list);
}

std::vector<std::string> UpgradeManager::getPurchasedUpgrades() const {
    std::vector<std::string> list;
    collectPurchased(propulsionTree, list);
    collectPurchased(logisticsTree, list);
    collectPurchased(tradingTree, list);
    return list;
}

bool UpgradeManager::findNodeAndSibling(std::shared_ptr<BinNode<Upgrade>> current, std::shared_ptr<BinNode<Upgrade>> sibling, const std::string& targetID, std::shared_ptr<BinNode<Upgrade>>& outNode, std::shared_ptr<BinNode<Upgrade>>& outSibling) const {
    if (!current) return false;
    
    if (K(current).id == targetID) {
        outNode = current;
        outSibling = sibling;
        return true;
    }
    
    // When you look to the left, the right is its counterpart (and vice versa)
    if (findNodeAndSibling(L(current), R(current), targetID, outNode, outSibling)) return true;
    if (findNodeAndSibling(R(current), L(current), targetID, outNode, outSibling)) return true;
    
    return false;
}

void UpgradeManager::loadPurchasedUpgrades(const std::vector<std::string>& purchasedIDs) {
    for (const std::string& id : purchasedIDs) {
        std::shared_ptr<BinNode<Upgrade>> node = nullptr;
        std::shared_ptr<BinNode<Upgrade>> sibling = nullptr;
        
        if (findNodeAndSibling(propulsionTree, nullptr, id, node, sibling) ||
            findNodeAndSibling(logisticsTree, nullptr, id, node, sibling) ||
            findNodeAndSibling(tradingTree, nullptr, id, node, sibling)) 
        {
            // 1. Simulate the purchase visually
            K(node).status = UpgradeStatus::PURCHASED;
            
            // 2. Lock mutually exclusive nodes
            if (K(node).isMutuallyExclusive && sibling != nullptr) {
                K(sibling).status = UpgradeStatus::BLOCKED_BY_CHOICE;
            }
            
            // 3. Unlock the children
            if (L(node) && K(L(node)).status == UpgradeStatus::LOCKED) K(L(node)).status = UpgradeStatus::AVAILABLE;
            if (R(node) && K(R(node)).status == UpgradeStatus::LOCKED) K(R(node)).status = UpgradeStatus::AVAILABLE;
            
            // 4. Apply the lambda effect to the player WITHOUT charging them anything
            K(node).applyEffect();
        }
    }
}

void UpgradeManager::resetNodeStatus(std::shared_ptr<BinNode<Upgrade>> node, bool isRoot) {
    if (!node) return;
    
    // La raíz siempre vuelve a estar AVAILABLE, los hijos se bloquean por defecto
    K(node).status = isRoot ? UpgradeStatus::AVAILABLE : UpgradeStatus::LOCKED;
    
    // Repetimos el proceso para sus hijos
    resetNodeStatus(L(node), false);
    resetNodeStatus(R(node), false);
}

void UpgradeManager::resetTrees() {
    resetNodeStatus(propulsionTree, true);
    resetNodeStatus(logisticsTree, true);
    resetNodeStatus(tradingTree, true);
}

// ==========================================
// FUNCIÓN ORIGINAL: Encuentra la mejora más profunda (última hoja comprada)
// ==========================================
std::string UpgradeManager::findDeepestPurchased(std::shared_ptr<BinNode<Upgrade>> node) const {
    if (!node) return "";
    
    std::string deepestId = "";
    int maxDepth = -1;
    
    // Función recursiva interna para buscar la mayor profundidad
    std::function<void(std::shared_ptr<BinNode<Upgrade>>, int)> searchDeepest = 
        [&](std::shared_ptr<BinNode<Upgrade>> currentNode, int currentDepth) {
            if (!currentNode) return;
            
            // Si este nodo está comprado, verificar si es más profundo
            if (K(currentNode).status == UpgradeStatus::PURCHASED) {
                if (currentDepth > maxDepth) {
                    maxDepth = currentDepth;
                    deepestId = K(currentNode).id;
                }
            }
            
            // Buscar en hijos (incrementando la profundidad)
            searchDeepest(L(currentNode), currentDepth + 1);
            searchDeepest(R(currentNode), currentDepth + 1);
        };
    
    searchDeepest(node, 0);
    
    if (deepestId.empty()) {
        std::cout << "[SYSTEM] No purchased upgrades found in this branch." << std::endl;
    } else {
        std::cout << "[SYSTEM] Deepest upgrade found: " << deepestId << " at depth " << maxDepth << std::endl;
    }
    
    return deepestId;
}

bool UpgradeManager::deactivateDeepestUpgrade(Player& player, int treeType, int branch) {
    // 1. Elegir el árbol raíz
    std::shared_ptr<BinNode<Upgrade>> rootToSearch = nullptr;
    if (treeType == 1) rootToSearch = propulsionTree;
    else if (treeType == 2) rootToSearch = logisticsTree;
    else if (treeType == 3) rootToSearch = tradingTree;

    if (!rootToSearch) {
        std::cout << "[SYSTEM] Invalid tree type." << std::endl;
        return false;
    }

    // 2. Definir en qué rama vamos a buscar inicialmente
    std::shared_ptr<BinNode<Upgrade>> startNode = rootToSearch;
    if (branch == 1 && L(rootToSearch)) {
        startNode = L(rootToSearch);
    } 
    else if (branch == 2 && R(rootToSearch)) {
        startNode = R(rootToSearch);
    } 

    // 3. Encontrar la última mejora de esa zona
    std::string idToRemove = findDeepestPurchased(startNode);
    
    // Si no encontró nada en la rama específica, el daño rebota y busca en TODO el árbol
    if (idToRemove.empty()) {
        std::cout << "[SYSTEM] Rama vacia. Redirigiendo el daño al resto del sistema..." << std::endl;
        // Al buscar desde rootToSearch, revisará automáticamente la rama contraria y,
        // si la contraria también está vacía, ¡seleccionará al mismísimo Nodo Padre!
        idToRemove = findDeepestPurchased(rootToSearch);
    }

    // Si después de buscar en todo el árbol sigue vacío, es que no hay absolutamente nada comprado
    if (idToRemove.empty()) {
        std::cout << "[SYSTEM] No hay mejoras compradas en este arbol.\n";
        return false; 
    }

    std::cout << "[SYSTEM] Eliminando la mejora: " << idToRemove << std::endl;

    // 4. Obtener TODAS las mejoras globales y borrar la víctima de la lista
    std::vector<std::string> currentUpgrades = getPurchasedUpgrades();
    
    // Verificar que el upgrade existe en la lista
    auto it = std::find(currentUpgrades.begin(), currentUpgrades.end(), idToRemove);
    if (it == currentUpgrades.end()) {
        std::cout << "[SYSTEM] Error: Mejora no encontrada en la lista!" << std::endl;
        return false;
    }
    
    currentUpgrades.erase(it);

    // 5. Guardar el estado actual que NO queremos perder
    float savedMoney = player.getMoney();
    short savedOrbit = player.getCurrentOrbit();
    auto savedItems = player.getInventory().getSlots();

    // 6. ¡FORMATEO DE FÁBRICA!
    player.resetToDefaults();
    resetTrees();

    // 7. Re-aplicar el resto de mejoras
    loadPurchasedUpgrades(currentUpgrades);

    // 8. Restaurar el estado guardado
    player.setMoney(savedMoney);

    // Seguridad de Órbita
    short minLegal = player.getMinOrbitReach();
    short maxLegal = player.getMaxOrbitReach();
    if (savedOrbit < minLegal) savedOrbit = minLegal;
    if (savedOrbit > maxLegal) savedOrbit = maxLegal;
    player.setCurrentOrbit(savedOrbit);

    // Restaurar los items en el inventario
    player.getInventory().clearAll();
    for (const auto& slot : savedItems) {
        if (slot.has_value()) {
            player.getInventory().addItem(slot->itemID, slot->quantity, slot->maxStackSize, slot->buyPrice);
        }
    }

    std::cout << "[SYSTEM] La mejora " << idToRemove << " ha sido destruida.\n";
    return true;
}