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
public:
    Message Score;
    int score_ = 0;
    vector<Enemy> enemies;
    int offSetX = 0, offSetY = 0;
    void loadMap(SDL_Renderer* ren, const char* file_name, Player& player);
    void renderMap(SDL_Renderer* ren, Player& player, Uint32 crTime);
    void print();
    void updateScore(SDL_Renderer* ren, Enemy& enemy);
    void randomSpawnEnemy(Uint32 crTime, SDL_Renderer* ren);
};

#endif // TILE_MAP_H_INCLUDED
