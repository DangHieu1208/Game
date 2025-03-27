#include "Sound.h"
#include <iostream>

void Sound::loadSound(const char* file_name) {
    sound = Mix_LoadWAV(file_name);
    if (!sound) {
        std::cout << "Failed to load sound '" << file_name << "': " << Mix_GetError() << std::endl;
    }
}

void Sound::loadMusic(const char* file_name) {
    music = Mix_LoadMUS(file_name);
    if (!music) {
        std::cout << "Failed to load music '" << file_name << "': " << Mix_GetError() << std::endl;
    }
}

void Sound::playSound() {
    if (sound) {
        channel = Mix_PlayChannel(-1, sound, 0);
        if (channel == -1) {
            std::cout << "Failed to play sound: " << Mix_GetError() << std::endl;
        }
    }
}

void Sound::playMusic(int loops) {
    if (music) {
        if (Mix_PlayMusic(music, loops) == -1) {
            std::cout << "Failed to play music: " << Mix_GetError() << std::endl;
        }
    }
}

void Sound::stopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}

void Sound::destroy() {
    if (music) {
        Mix_FreeMusic(music);
    }
    if (sound) {
        Mix_FreeChunk(sound);
    }
}
