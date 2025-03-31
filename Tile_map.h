#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include <vector>
#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Message.h"
#include "Boss_1.h"

struct Tile : public Entity {
public:
    Tile() {;}
};

struct Map {
private:
    int tiles[MAP_HEIGHT][MAP_WIDTH];
    SDL_Rect tile[MAP_HEIGHT][MAP_WIDTH];
    Tile mapTile[MAP_TILES_NUMS];
    Uint32 lastSkeletonSpawnTime = 0;
    Uint32 lastRatSpawnTime = 0;
    Uint32 lastSlimeSpawnTime = 0;
    Uint32 lastSlimeSpawnTime_2 = 0;
    Uint32 intervalStartTime = 0;
    Uint32 trapStartTime = 0;
    SDL_Texture* skeleton;
    SDL_Texture* rat;
    SDL_Texture* slime;
    SDL_Texture* slime_boss;
    SDL_Texture* cloud;
    SDL_Texture* fire;
    Uint32 portalSrartTime = 0;
    int boss_1_wave_index = 1;
    int boss_2_wave_index = 2;
    int boss_3_wave_index = 3;
    int skeleton_base_HP = 20;
    int skeleton_base_speed = 4;
    int rat_base_HP = 1;
    int rat_base_speed = 7;
    int slime_base_HP = 30;
    int slime_base_speed = 2;
public:
    enum BossType { NONE, BOSS_1, BOSS_2, BOSS_3 };
    enum MapType {MAP_1, MAP_2, MAP_3};
    BossType currentBoss = NONE;
    MapType currentMap = MAP_1;
    bool hpUpgraded = false, attackUpgraded = false, alliesUpgraded = false, isInterval = false, pointUpgraded = false,
        speedUpgraded = false, enemySpawn = true, isBossKilled = false, isBossWave = false, enemyUpgraded = false, killAllSlimes;
    int skeleton_killed = 0, rat_killed = 0, slime_killed = 0,
        skeleton_left = 3, rats_left = 2, slime_left = 1,
        skeleton_wave_nums = 3, rat_wave_nums = 2, slime_wave_nums = 1,
        skeleton_add = 1, rat_add = 1, slime_add = 1;
    int intervalTime = INTERVAL_TIME, score_ = 0, upgrade_points = 0, upgrade_points_add, wave = 1;
    int trap_index = 1;
    Boss boss, boss_1, boss_2, boss_3;
    Message Wave, Score, Time, UpgradePoints, Upgrade, BossWaveWarn;
    vector<Enemy> skeletons, rats, slimes, slimes_boss;
    void loadMap(SDL_Renderer* ren, const char* file_name);
    void loadTexture(SDL_Renderer* ren);
    void renderMap(SDL_Renderer* ren, Player& player, Uint32 crTime);
    void loadEntities(SDL_Renderer* ren, Player& player);
    void loadEnemyTexture(SDL_Renderer* ren);
    void print();
    void updateScore(SDL_Renderer* ren, Enemy& enemy);
    void randomSpawnSkeleton(Uint32 crTime, SDL_Renderer* ren);
    void randomSpawnRat(Uint32 crTime, SDL_Renderer* ren);
    void randomSpawnSlime(Uint32 crTime, SDL_Renderer* ren);
    void randomSpawnSlimeBoss(Uint32 crTime, SDL_Renderer* ren);
    void randomSpawnSlimeBoss_2(Uint32 crTime, SDL_Renderer* ren);
    void intervalCount(Uint32 crTime, SDL_Renderer* ren, Player& player);
    void renderWave(SDL_Renderer* ren);
    void playerUpgrade(SDL_Event& event, Player& player);
};

#endif // TILE_MAP_H_INCLUDED
