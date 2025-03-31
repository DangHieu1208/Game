#include "Sound.h"
#include <iostream>
using namespace std;

void Sound::loadSound(const char* file_name) {
    sound = Mix_LoadWAV(file_name);
}

void Sound::loadMusic(const char* file_name) {
    music = Mix_LoadMUS(file_name);
}

void Sound::playSound() {
    if (sound) {
        channel = Mix_PlayChannel(-1, sound, 0);
    }
}

void Sound::playMusic(int loops) {
    if (music) {
        Mix_PlayMusic(music, loops);
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
