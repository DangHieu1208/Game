#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include "Message.h"
#include <math.h>

struct Player : public Entity {
private:
    //bool move_right = false, move_left = false, facing_right = true, facing_left = false, isAttacking = false, go_up = false, go_down = false, defence = false;;
    Uint32 AttackStartTime;
    Uint32 MoveStartTime;
    Uint32 DefenceStartTime;
    Uint32 DefenceBeginTime;
    Uint32 StandStartTime = SDL_GetTicks();
public:
    Message PlayerHP;
    Uint32 DieStartTime;
    Uint32 TrappedStartTime;
    int attack_index = 0;
    int attackDamage = 4 ;
    int die_index = 0;
    int defence_abusing = 0;
    bool move_right = false, move_left = false, facing_right = true, facing_left = false, isAttacking = false, go_up = false, go_down = false, defence = false, defenceCooldown = false, isDied = false;
    SDL_Rect camera;
    int HP = 10;
    int max_HP = HP;
    int speed = 6;
    void renderPlayer(SDL_Renderer* ren);
    void update(Uint32 crTime);
    void updateCamera();
    void handleEvent(SDL_Event& e);
    void loadHP(SDL_Renderer* ren);
    void updateHP(SDL_Renderer* ren);
};

#endif // PLAYER_H_INCLUDED
