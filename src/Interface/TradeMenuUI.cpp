#include "Interface/TradeMenuUI.hpp"
#include <iomanip> // To format decimals (std::fixed, std::setprecision)
#include <sstream>

/*
Menu visual configuration:
    1. Background: 
    (20, 20, 40, 200) - Very dark blue with transparency to see the background planet
    2. Text:
    Cyan (0, 255, 255) for titles and headers
    Yellow (255, 255, 0) for money and highlights
    3. Borders and Lines:
    Cyan (0, 255, 255) for technological consistency
*/

TradeMenuUI::TradeMenuUI(const sf::Font& f) 
    : font(f),
      titleText(f, ""),
      playerSectionTitle(f, ""),
      playerMoneyText(f, ""),
      headerPlayerName(f, ""),
      headerPlayerQty(f, ""),
      headerPlayerSellPrice(f, ""),
      planetNameText(f, ""),
      headerPlanetName(f, ""),
      headerPlanetQty(f, ""),
      headerPlanetBuyPrice(f, ""),
      ctxBtnInfo(f, "Info."),
      ctxBtnAction(f, ""),
      ctxBtnCancel(f, "Cancel"),
      infoPopupText(f, ""),
      infoPopupEscText(f, "Press Esc. to close"),
      infoInputText(f, "Use Mouse Scroll to navigate objects, RMB for more info, LMB to select, Esc. to exit."),
      playerActionBtnText(f, "SELL"),
      planetActionBtnText(f, "BUY"),
      headerPlayerCategory(f, "CATEGORY"),
      headerPlayerQuality(f, "QUALITY"),
      headerPlanetCategory(f, "CATEGORY"),
      headerPlanetQuality(f, "QUALITY")
{
// --- LAYOUT CONSTANTS (Visual adjustments) ---
    sf::Vector2f panelSize(560.f, 550.f);
    sf::Color panelFill(20, 20, 40, 200); // Dark with a hint of blue (200 alpha)
    sf::Color cyanOutline(sf::Color::Cyan); // Cyan Borders
    int headerSize = 20;

    // General Title
    titleText.setFont(font);
    titleText.setString("TRADE MENU");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Cyan); // Cyan Text
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
    titleText.setPosition({ 640.f, 50.f });

// --- LEFT SECTION: PLAYER ---
    float leftPanelX = 50.f;
    float topY = 100.f;
    float headerY = topY + 60.f;

    playerTableBg.setSize(panelSize);
    playerTableBg.setFillColor(panelFill);
    playerTableBg.setOutlineThickness(3);
    playerTableBg.setOutlineColor(cyanOutline);
    playerTableBg.setPosition({ leftPanelX, topY });

    playerSectionTitle.setFont(font);
    playerSectionTitle.setString("YOUR INVENTORY");
    playerSectionTitle.setCharacterSize(25);
    playerSectionTitle.setFillColor(sf::Color::Cyan); // Cyan section title
    playerSectionTitle.setPosition({ leftPanelX + 20.f, topY + 15.f });

    playerMoneyText.setFont(font);
    playerMoneyText.setCharacterSize(18);
    playerMoneyText.setFillColor(sf::Color::Yellow); // Money in Yellow
    playerMoneyText.setPosition({ leftPanelX + 380.f, topY + 22.f });

    // Adjust X positions relative to leftPanelX to accommodate more columns
    headerPlayerName.setFont(font);
    headerPlayerName.setString("ITEM");
    headerPlayerName.setCharacterSize(16);
    headerPlayerName.setFillColor(cyanOutline);
    headerPlayerName.setPosition({ leftPanelX + 10.f, headerY });

    headerPlayerCategory.setFont(font);
    headerPlayerCategory.setString("CAT.");
    headerPlayerCategory.setCharacterSize(16);
    headerPlayerCategory.setFillColor(cyanOutline);
    headerPlayerCategory.setPosition({ leftPanelX + 225.f, headerY });

    headerPlayerQuality.setFont(font);
    headerPlayerQuality.setString("QUAL.");
    headerPlayerQuality.setCharacterSize(16);
    headerPlayerQuality.setFillColor(cyanOutline);
    headerPlayerQuality.setPosition({ leftPanelX + 330.f, headerY });

    headerPlayerQty.setFont(font);
    headerPlayerQty.setString("QTY.");
    headerPlayerQty.setCharacterSize(16);
    headerPlayerQty.setFillColor(cyanOutline);
    headerPlayerQty.setPosition({ leftPanelX + 400.f, headerY });

    headerPlayerSellPrice.setFont(font);
    headerPlayerSellPrice.setString("SELL");
    headerPlayerSellPrice.setCharacterSize(16);
    headerPlayerSellPrice.setFillColor(cyanOutline);
    headerPlayerSellPrice.setPosition({ leftPanelX + 470.f, headerY });

// --- RIGHT SECTION: PLANET ---
    float rightPanelX = 670.f;

    planetTableBg.setSize(panelSize);
    planetTableBg.setFillColor(panelFill);
    planetTableBg.setOutlineThickness(3);
    planetTableBg.setOutlineColor(cyanOutline);
    planetTableBg.setPosition({ rightPanelX, topY });

    planetNameText.setFont(font);
    // Planet name is dynamically updated in draw()
    planetNameText.setCharacterSize(25);
    planetNameText.setFillColor(sf::Color::Cyan); // Cyan planet name
    planetNameText.setPosition({ rightPanelX + 20.f, topY + 15.f });

    // Adjust X positions relative to rightPanelX
    headerPlanetName.setFont(font);
    headerPlanetName.setString("ITEM");
    headerPlanetName.setCharacterSize(16);
    headerPlanetName.setFillColor(cyanOutline);
    headerPlanetName.setPosition({ rightPanelX + 10.f, headerY });

    headerPlanetCategory.setFont(font);
    headerPlanetCategory.setString("CAT.");
    headerPlanetCategory.setCharacterSize(16);
    headerPlanetCategory.setFillColor(cyanOutline);
    headerPlanetCategory.setPosition({ rightPanelX + 225.f, headerY });

    headerPlanetQuality.setFont(font);
    headerPlanetQuality.setString("QUAL.");
    headerPlanetQuality.setCharacterSize(16);
    headerPlanetQuality.setFillColor(cyanOutline);
    headerPlanetQuality.setPosition({ rightPanelX + 330.f, headerY });

    headerPlanetQty.setFont(font);
    headerPlanetQty.setString("QTY.");
    headerPlanetQty.setCharacterSize(16);
    headerPlanetQty.setFillColor(cyanOutline);
    headerPlanetQty.setPosition({ rightPanelX + 400.f, headerY });

    headerPlanetBuyPrice.setFont(font);
    headerPlanetBuyPrice.setString("BUY");
    headerPlanetBuyPrice.setCharacterSize(16);
    headerPlanetBuyPrice.setFillColor(cyanOutline);
    headerPlanetBuyPrice.setPosition({ rightPanelX + 470.f, headerY });
    
// --- CONTEXT MENU CONFIGURATION ---
    contextMenuBg.setSize({ 150.f, 100.f });
    contextMenuBg.setFillColor(sf::Color(20, 20, 40, 235)); // Very dark blue
    contextMenuBg.setOutlineThickness(2);
    contextMenuBg.setOutlineColor(sf::Color::Yellow); // Yellow highlight

    int ctxFontSize = 16;
    ctxBtnInfo.setCharacterSize(ctxFontSize);   ctxBtnInfo.setFillColor(sf::Color::White);
    ctxBtnAction.setCharacterSize(ctxFontSize); ctxBtnAction.setFillColor(sf::Color::White);
    ctxBtnCancel.setCharacterSize(ctxFontSize); ctxBtnCancel.setFillColor(sf::Color::Red);

// --- INFO POPUP CONFIGURATION ---
    infoPopupBg.setFillColor(sf::Color(10, 10, 20, 245)); // Almost bluish black
    infoPopupBg.setOutlineThickness(2);
    infoPopupBg.setOutlineColor(sf::Color::Cyan);

    infoPopupText.setCharacterSize(18);
    infoPopupText.setFillColor(sf::Color::White);

    // Set up ESC text
    infoPopupEscText.setCharacterSize(14);
    infoPopupEscText.setFillColor(sf::Color(150, 150, 150)); // Discreet gray

// --- INSTRUCTION TEXT CONFIGURATION ---
    infoInputText.setCharacterSize(16);
    infoInputText.setFillColor(sf::Color::White);
    sf::FloatRect inputBounds = infoInputText.getLocalBounds();
    infoInputText.setOrigin({ inputBounds.size.x / 2.f, inputBounds.size.y / 2.f });
    infoInputText.setPosition({ 640.f, 685.f });

// --- SELECTION HIGHLIGHT CONFIGURATION ---
    selectionHighlight.setSize({540.f, 35.f}); 
    selectionHighlight.setFillColor(sf::Color(0, 255, 255, 60)); // Very transparent Cyan

// --- ACTION BUTTONS CONFIGURATION ---
    sf::Vector2f btnSize(150.f, 40.f);
    float btnY = topY + 500.f; 

    // Player Button (Sell)
    playerActionBtnBg.setSize(btnSize);
    playerActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Disabled
    playerActionBtnBg.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    playerActionBtnBg.setPosition({leftPanelX + (panelSize.x / 2.f), btnY});
    
    playerActionBtnText.setCharacterSize(16);
    sf::FloatRect pBounds = playerActionBtnText.getLocalBounds();
    playerActionBtnText.setOrigin({pBounds.size.x / 2.f, pBounds.size.y / 2.f});
    playerActionBtnText.setPosition({leftPanelX + (panelSize.x / 2.f), btnY});

    // Planet Button (Buy)
    planetActionBtnBg.setSize(btnSize);
    planetActionBtnBg.setFillColor(sf::Color(100, 100, 100)); // Disabled
    planetActionBtnBg.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    planetActionBtnBg.setPosition({rightPanelX + (panelSize.x / 2.f), btnY});
    
    planetActionBtnText.setCharacterSize(16);
    sf::FloatRect mBounds = planetActionBtnText.getLocalBounds();
    planetActionBtnText.setOrigin({mBounds.size.x / 2.f, mBounds.size.y / 2.f});
    planetActionBtnText.setPosition({rightPanelX + (panelSize.x / 2.f), btnY});
}