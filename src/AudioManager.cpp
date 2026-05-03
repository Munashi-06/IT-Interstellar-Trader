#include "AudioManager.hpp"

AudioManager::AudioManager() : hoverSound(hoverBuffer), clickSound(clickBuffer) 
{

}

bool AudioManager::loadMusic(const std::string&path){
    if(!music.openFromFile(path)){
        std::cerr << "Error cargando música: " << path << std::endl;
        return false;
    }

    currentMusicPath = path;
    music.setLooping(true);
    music.setVolume(static_cast<float>(musicVolume));
    std::cout << "Música cargada: " << path << std::endl;
    return true;
}

void AudioManager::playMusic() {
    if (!musicPlaying){
        music.play();
        musicPlaying = true;
    }
}

void AudioManager::stopMusic(){
    if(musicPlaying){
        music.stop();
        musicPlaying = false;
    }
}

void AudioManager::pauseMusic(){
    if(musicPlaying){
        music.pause();
        musicPlaying = false;
    }
}

void AudioManager::setMusicVolume(int volume){
    musicVolume = std::clamp(volume, 0, 100);
    music.setVolume(static_cast<float>(musicVolume));
}

int AudioManager::getMusicVolume() const {
    return musicVolume;
}

bool AudioManager::isMusicPlaying() const {
    return musicPlaying;
}

bool AudioManager::loadSFX(const std::string& hoverPath, const std::string& clickPath){
    bool success = true;

    if (!hoverBuffer.loadFromFile(hoverPath)){
        std::cerr << "Error cargando efecto de sonido: " << hoverPath <<std::endl;
        success = false;
    }

    if(!clickBuffer.loadFromFile(clickPath)){
        std::cerr << "Error cargando efecto de sonido: " << clickPath << std::endl;
        success = false;
    }

    if (success){
        std::cout << "Efectos de sonido cargados: " << hoverPath << ", " << clickPath << std::endl;
    }

    return success;
}

void AudioManager::playHover(){
    hoverSound.stop();
    hoverSound.play();
}

void AudioManager::playClick(){
    clickSound.play();
}

void AudioManager::setSFXVolume(int volume){
    sfxVolume = std::clamp(volume, 0, 100);
    hoverSound.setVolume(static_cast<float>(sfxVolume));
    clickSound.setVolume(static_cast<float>(sfxVolume));
}

int AudioManager::getSFXVolume() const{
    return sfxVolume;
}

void AudioManager::setAllVolumes(int musicVol, int sfxVol){
    setMusicVolume(musicVol);
    setSFXVolume(sfxVol);
}

void AudioManager::updateVolumesFromConfig(int musicVol, int sfxVol){
    setAllVolumes(musicVol, sfxVol);
}