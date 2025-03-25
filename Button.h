#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "Entity.h"
#include "Const.h"

struct Button : public Entity {
    bool isPaused = false;
    bool isClicked(int MouseX, int MouseY);
    void renderButton(SDL_Renderer* ren, SDL_Rect camera);
    void resume(SDL_Event& event, int mouseX, int mouseY, bool isPaused);
};

#endif // BUTTON_H_INCLUDED
