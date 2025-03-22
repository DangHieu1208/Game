 #ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include <vector>
#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Message.h"

struct Tile : public Entity {
public:
    Tile() {;}
};

struct Map {
private:
    int tiles[18][32];
    SDL_Rect tile[18][32];
    Tile mapTile[20];
    Uint32 lastSpawnTime = 0;
    Uint32 intervalStartTime = 0;
public:
    bool hpUpgraded = false, attackUpgraded = false, alliesUpgraded = false, isInterval = false, enemyAttackUpgraded = false, pointUpgraded = false;
    int enemy_killed = 0;
    int enemy_left = ENEMY_NUMS_WAVE_1;
    int intervalTime = 15;
    int score_ = 0;
    int upgrade_points = 0;
    int wave = 1;
    Message Wave;
    Message Score;
    Message Time;
    Message UpgradePoints;
    vector<Enemy> enemies;
    void loadMap(SDL_Renderer* ren, const char* file_name, Player& player);
    void renderMap(SDL_Renderer* ren, Player& player, Uint32 crTime);
    void print();
    void updateScore(SDL_Renderer* ren, Enemy& enemy);
    void randomSpawnEnemy(Uint32 crTime, SDL_Renderer* ren);
    void intervalCount(Uint32 crTime, SDL_Renderer* ren);
    void renderWave(SDL_Renderer* ren);
    void playerUpgrade(SDL_Event& event, Player& player);
};

#endif // TILE_MAP_H_INCLUDED
