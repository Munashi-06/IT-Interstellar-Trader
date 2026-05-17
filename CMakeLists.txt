cmake_minimum_required(VERSION 3.14)
project(IT-Interstellar-Trader)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Forzar enlazado estático" FORCE)

find_package(SFML 3 COMPONENTS Graphics Window System Audio QUIET)

if(NOT SFML_FOUND)
    message(STATUS "SFML 3 no detectado en el sistema. Descargando mediante FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.0
    )
    FetchContent_MakeAvailable(SFML)
endif()

add_executable(IT-Interstellar-Trader 
    src/Core/main.cpp
    src/Core/Engine.cpp
    src/Core/AudioManager.cpp
    src/Core/Settings.cpp
    src/Core/WorldStatemanager.cpp
    src/Core/World.cpp
    
    src/Entities/Player.cpp
    src/Entities/Planet.cpp
    src/Entities/Item.cpp
    
    src/Systems/Economy.cpp
    src/Systems/TradeManager.cpp
    src/Systems/PlanetManager.cpp
    src/Systems/ItemFactory.cpp
    src/Systems/Inventory.cpp
    src/Systems/UpgradeManager.cpp
    src/Systems/SaveSystem.cpp
    src/Systems/AssetManager.cpp
    
    src/Interface/Menu.cpp
    src/Interface/ShipMenuUI.cpp
    src/Interface/TradeMenuUI.cpp
    src/Interface/BackgroundStars.cpp
    src/Interface/IntroAnimation.cpp
    src/Interface/UpgradeTreeUI.cpp
    src/Interface/DebugMenuUI.cpp
    src/Interface/PirateEncounter.cpp
    src/Interface/GameIntroAnimation.cpp
    src/Interface/GameOverScene.cpp
    src/Interface/PauseMenuUI.cpp
    src/Interface/Popup.cpp
    
    src/DataStructures/Heap.cpp
)

target_include_directories(IT-Interstellar-Trader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_command(TARGET IT-Interstellar-Trader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:IT-Interstellar-Trader>/assets"
)

target_link_libraries(IT-Interstellar-Trader PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)