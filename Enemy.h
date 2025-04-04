#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "Player.h"
#include "Sound.h"
#include "Message.h"
#include <cstdlib>

struct Enemy : public Entity {
private:
    Uint32 lastUpdateTime = SDL_GetTicks();
    Uint32 attackCoolDown = 0;
    int deathFrame = 0;
    int attackRange = 70;
    int stoppingZoneRadius = 60;
public:
    Uint32 lastingTime;
    Sound enemyAttack;
    Message EnemyHP;
    int attack_count = 0;
    int speed, max_HP, HP;
    int attackDamage = ENEMY_DAMAGE;
    int animFrame = 0;
    bool move_right = true, isAttacking = false, isDied = false, score_count = false, blocked = false, isAttacked = false;
    void attackPlayer(Uint32 crTime, Player& player);
    void update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);
    void renderEnemy(SDL_Renderer* ren, SDL_Rect& camera);
    bool isNearPlayer(Player& player, int range);
    void chasePlayer(Uint32 crTime, Player& player);
    void loadHP(SDL_Renderer* ren);
    void updateHP(SDL_Renderer* ren, Player& player);
    void solveCollision(SDL_Rect& wall, Player& player);
};

#endif // ENEMY_H_INCLUDED
