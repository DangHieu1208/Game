#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include <vector>
#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Message.h"
#include "Boost.h"
#include "Boss_1.h"

struct Tile : public Entity {
public:
    Tile() {;}
};

struct Map {
private:
    int tiles[18][32];
    SDL_Rect tile[18][32];
    Tile mapTile[20];
    Uint32 lastSkeletonSpawnTime = 0;
    Uint32 lastRatSpawnTime = 0;
    Uint32 lastSlimeSpawnTime = 0;
    Uint32 intervalStartTime = 0;
    Uint32 trapStartTime = 0;
    Uint32 portalSrartTime = 0;
public:
    enum BossType { NONE, BOSS_1, BOSS_2, BOSS_3 };
    BossType currentBoss = NONE;
    bool hpUpgraded = false, attackUpgraded = false, alliesUpgraded = false, isInterval = false, pointUpgraded = false, speedUpgraded = false, enemySpawn = true, isBossKilled = false, isBossWave = false, enemyUpgraded = false;
    bool isRatSpawned = false, mapChange = false, isMap1 = false, isMap2 = false;
    int skeleton_killed = 0, rat_killed = 0, slime_killed = 0;
    int skeleton_left = 3, rats_left = 2, slime_left = 1;
    int skeleton_wave_nums = 3, rat_wave_nums = 2, slime_wave_nums = 1;
    int skeleton_add = 1, rat_add = 1, slime_add = 1;
    int intervalTime = INTERVAL_TIME;
    int score_ = 0;
    int upgrade_points = 0, upgrade_points_add;
    int wave = 1;
    int trap_index = 1;
    int fire_index = 0;
    Boss boss;
    Boss boss_1;
    Boss boss_2;
    Boss boss_3;
    Message Wave;
    Message Score;
    Message Time;
    Message UpgradePoints;
    Message Upgrade;
    vector<Enemy> skeletons, rats, slimes;
    void loadMap(SDL_Renderer* ren, const char* file_name);
    void loadTexture(SDL_Renderer* ren);
    void renderMap(SDL_Renderer* ren, Player& player, Uint32 crTime);
    void loadEntities(SDL_Renderer* ren, Player& player);
    void print();
    void updateScore(SDL_Renderer* ren, Enemy& enemy);
    void randomSpawnSkeleton(Uint32 crTime, SDL_Renderer* ren);
    void randomSpawnRat(Uint32 crTime, SDL_Renderer* ren);
    void randomSpawnSlime(Uint32 crTime, SDL_Renderer* ren);
    void intervalCount(Uint32 crTime, SDL_Renderer* ren, Player& player);
    void renderWave(SDL_Renderer* ren);
    void playerUpgrade(SDL_Event& event, Player& player);
};

#endif // TILE_MAP_H_INCLUDED
