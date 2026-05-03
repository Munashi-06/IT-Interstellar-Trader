cmake_minimum_required(VERSION 3.14)
project(IT-Interstellar-Trader)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
)

FetchContent_MakeAvailable(SFML)

add_executable(IT-Interstellar-Trader 
    src/main.cpp
    src/Menu.cpp
    src/Settings.cpp
    src/Player.cpp
    src/Item.cpp
    src/Inventory.cpp
    src/ItemFactory.cpp
    src/Planet.cpp
    src/Economy.cpp
    src/State.hpp
    src/PlanetManager.cpp
    src/WorldStatemanager.cpp
    src/World.cpp
    src/Heap.cpp
    src/BinaryNode.hpp
    src/RadarUI.hpp
    src/ShipMenuUI.cpp
    src/TradeMenuUI.cpp
    src/AudioManager.cpp
)

add_custom_command(TARGET IT-Interstellar-Trader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:IT-Interstellar-Trader>/assets"
)

target_link_libraries(IT-Interstellar-Trader sfml-graphics sfml-window sfml-system sfml-audio)