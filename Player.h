#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include <math.h>

struct Player : public Entity {
private:
    bool move_right = false, move_left = false, facing_right = true, facing_left = false, isAttacking = false, go_up = false, go_down = false;
    Uint32 AttackStartTime;
    Uint32 MoveStartTime;
    Uint32 StandStartTime = SDL_GetTicks();
public:
    SDL_Rect camera;
    void renderPlayer(SDL_Renderer* ren);
    void update(Uint32 crTime);
    void updateCamera();
    void handleEvent(SDL_Event& e);
};

#endif // PLAYER_H_INCLUDED
