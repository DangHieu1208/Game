#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include <vector>
#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

struct Tile : public Entity {
public:
    Tile() {;}
};

struct Map {
private:
    int tiles[18][32];
    SDL_Rect tile[18][32];
    Tile mapTile[20];
public:
    vector<Enemy> enemies;
    int offSetX = 0, offSetY = 0;
    void loadMap(SDL_Renderer* ren, const char* file_name);
    void renderMap(SDL_Renderer* ren, Player& player);
    void print();
};

#endif // TILE_MAP_H_INCLUDED
