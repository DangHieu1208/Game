#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "Player.h"
#include "Message.h"

struct Enemy : public Entity {
private:
    int speed = 3;
    Uint32 lastUpdateTime = SDL_GetTicks();
    Uint32 attackCoolDown = 0;
    //int animFrame = 0;
    int deathFrame = 0;
public:
    Enemy() {;}
    Message EnemyHP;
    int HP = 20;
    int animFrame = 0;
    bool move_right = true, isAttacking = false, isDied = false, score_count = false;
    void attackPlayer(Uint32 crTime, Player& player);
    void update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32]);
    void renderEnemy(SDL_Renderer* ren, SDL_Rect& camera);
    bool isNearPlayer(Player& player, int range);
    void chasePlayer(Uint32 crTime, Player& player);
    void loadHP(SDL_Renderer* ren);
    void updateHP(SDL_Renderer* ren, Player& player);
};

#endif // ENEMY_H_INCLUDED
