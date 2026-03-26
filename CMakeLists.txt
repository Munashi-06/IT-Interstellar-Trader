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

add_executable(IT-Interstellar-Trader src/main.cpp)

target_link_libraries(IT-Interstellar-Trader sfml-graphics sfml-window sfml-system)
