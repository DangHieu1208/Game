#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <SDL_mixer.h>

struct Sound {
private:
    Mix_Chunk* sound;
    Mix_Music* music;
    int channel;
public:
    void loadSound(const char* file_name);
    void loadMusic(const char* file_name);
    void playSound();
    void playMusic(int loops = -1);
    void stopMusic();
    void destroy();
};

#endif // SOUND_H_INCLUDED
