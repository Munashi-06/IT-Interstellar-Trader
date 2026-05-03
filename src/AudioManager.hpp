#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <iostream>

class AudioManager {
    private:
        sf::Music music;
        std::string currentMusicPath;
        bool musicPlaying = false;

        sf::SoundBuffer hoverBuffer;
        sf::SoundBuffer clickBuffer;
        sf::Sound hoverSound;
        sf::Sound clickSound;

        int musicVolume = 50;
        int sfxVolume = 50;

    public:
        AudioManager();

        bool loadMusic(const std::string& path);
        void playMusic();
        void stopMusic();
        void pauseMusic();
        void setMusicVolume(int volume);
        int getMusicVolume() const;
        bool isMusicPlaying() const;

        bool loadSFX(const std::string& hoverPath, const std::string& clickPath);
        void playHover();
        void playClick();
        void setSFXVolume(int volume);
        int getSFXVolume() const;

        void setAllVolumes(int musicVol, int sfxVol);
        void updateVolumesFromConfig(int musicVol, int sfxVol);
};
