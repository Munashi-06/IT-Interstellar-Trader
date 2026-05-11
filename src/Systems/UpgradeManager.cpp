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
        "Next-gen propulsion system. Unlocks Orbit 7 and increases travel speed by 50%.", 
        3500.0f, 
        false, 
        [&player]() {
            player.setMaxOrbit(7);
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
        }
    );

    R(R(R(R(propulsionTree)))) = std::make_shared<BinNode<Upgrade>>(std::move(warpDrive));
#pragma endregion

// ==========================================
// 2. LOGISTICS TREE INITIALIZATION
// ==========================================
#pragma region Logistics Tree Initialization

    // ROOT NODE: Advanced Radar
    // Effect: Unlocks the logistics tree. (You could tie this to revealing planet names on the UI)
    Upgrade advancedRadar(
        "log_1", 
        "Advanced Radar", 
        "Upgrades basic sensors. Shows info About planets events. Essential for interstellar commerce.", 
        800.0f, 
        false, // Root is never mutually exclusive
        [&player]() {
            player.setHasAdvancedRadar(true); // You will need to add this bool to Player.hpp
        }
    );
    
    logisticsTree = std::make_shared<BinNode<Upgrade>>(std::move(advancedRadar));
    K(logisticsTree).status = UpgradeStatus::AVAILABLE;


    // ================= BRANCH A: THE HAULER (LEFT) =================

    // LEFT CHILD: Heavy Freighter Hull
    // Effect: Massively increases inventory slots. Mutually exclusive with Market Predictor.
    Upgrade heavyFreighter(
        "log_cargo_1", 
        "Heavy Freighter Hull", 
        "Expands cargo bay. +10 Inventory Slots. Lacks space for advanced computers.", 
        2500.0f, 
        true, // MUTUALLY EXCLUSIVE! Choosing this blocks the Right Child
        [&player]() {
            player.addInventoryCapacity(10); 
        }
    );
    
    L(logisticsTree) = std::make_shared<BinNode<Upgrade>>(std::move(heavyFreighter));

    // LEFT-LEFT CHILD: Reinforced Cargo Holder
    // Effect: Increases inventory capacity. Mutually exclusive with Smuggler's Compartment (the other Left child).
    Upgrade reinforcedCargo(
        "log_cargo_3", 
        "Reinforced Cargo Holder", 
        "Strengthens cargo hold. +25 Slots and can carry heavier items without damage.", 
        4000.0f, 
        true, 
        [&player]() {
            player.addInventoryCapacity(30);
        }
    );

    L(L(logisticsTree)) = std::make_shared<BinNode<Upgrade>>(std::move(reinforcedCargo));

    // LEFT-RIGHT CHILD: Smuggler's Compartment
    // Effect: Further increases cargo slightly, but hides it. Mutually exclusive with Reinforced Cargo Holder.
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

    // LEFT-LEFT-LEFT & LEFT-LEFT-RIGHT CHILD: Quantum Storage
    // Effect: Uses quantum tech to expand cargo space. +50 inventory slots.
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

    // 1. Creamos una copia exacta manualmente en memoria
    Upgrade quantumStorage2 = quantumStorage;

    // 2. Ahora sí podemos usar std::move en ambas porque son variables distintas
    L(L(L(logisticsTree))) = std::make_shared<BinNode<Upgrade>>(std::move(quantumStorage));
    L(R(L(logisticsTree))) = std::make_shared<BinNode<Upgrade>>(std::move(quantumStorage2));


    // ================= BRANCH B: THE BROKER (RIGHT) =================

    // RIGHT CHILD: Insider Trading Link
    // Effect: Makes events last longer.
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

    R(logisticsTree) = std::make_shared<BinNode<Upgrade>>(std::move(insiderTrading));

    // RIGHT-LEFT CHILD: Market Predictor Algorithm
    // Effect: Increases the frequency of beneficial planet events. Mutually exclusive with Manipulator.
    Upgrade marketPredictor(
        "log_intel_1", 
        "Market Predictor", 
        "Advanced AI intercepts comms. Increases Planet Event frequency. No room for extra cargo.", 
        2500.0f, 
        true,
        [&player]() {
            player.setEventFrequencyBonus(true);
        }
    );

    L(R(logisticsTree)) = std::make_shared<BinNode<Upgrade>>(std::move(marketPredictor));

    // RIGHT-RIGHT CHILD: Manipulator Chip
    // Effect: Allows the player to trigger one event every 4 minutes. Mutually exclusive with Market Predictor.
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
        std::string("Breaks communication barriers. Grants a flat 5% discount on all purchases."), 
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
        "Official trading permits. 15% better prices for Tech and Medical goods.", 
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

#pragma endregion
}