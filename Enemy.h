#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "Player.h"
#include "Message.h"
#include <cstdlib>

struct Enemy : public Entity {
private:
    int speed = rand() % (4-2+1)+2;
    Uint32 lastUpdateTime = SDL_GetTicks();
    Uint32 attackCoolDown = 0;
    //int animFrame = 0;
    int deathFrame = 0;
public:
    Enemy() {;}
    Message EnemyHP;
    int HP = rand() % (enemy_base_HP-(enemy_base_HP-5)+1)+(enemy_base_HP-5);
    int max_HP = HP;
    int attackDamage;
    int animFrame = 0;
    bool move_right = true, isAttacking = false, isDied = false, score_count = false, blocked = false, isAttacked = false;
    void attackPlayer(Uint32 crTime, Player& player);
    void update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave);
    void renderEnemy(SDL_Renderer* ren, SDL_Rect& camera);
    bool isNearPlayer(Player& player, int range);
    void chasePlayer(Uint32 crTime, Player& player);
    void loadHP(SDL_Renderer* ren);
    void updateHP(SDL_Renderer* ren, Player& player);
    void solveCollision(SDL_Rect& wall, Player& player);
};

#endif // ENEMY_H_INCLUDED
