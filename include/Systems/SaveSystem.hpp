#pragma once
#include <string>
#include <functional>
#include <ctime>
#include <cstdlib>

class SaveSystem {
public:
    // Converts text into a seed number and configures C++ randomization
    // If you pass an empty string “”, it will generate a random seed based on the current time.
    static unsigned int setGameSeed(const std::string& seedText);
};