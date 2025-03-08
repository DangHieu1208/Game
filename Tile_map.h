#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include <vector>
#include "Const.h"
#include "Entity.h"
#include "Player.h"

struct Tile : public Entity {
public:
    Tile() {;}
};

struct Map {
private:
    int tiles[18][32];
    SDL_Rect tile[18][32];
    Tile wall;
    Tile floor;
public:
    int offSetX = 0, offSetY = 0;
    void loadMap(SDL_Renderer* ren, const char* file_name);
    void createMap(SDL_Renderer* ren, Player& player);
    void scrollMap(Player& player, SDL_Event& e);
    void print();
};

#endif // TILE_MAP_H_INCLUDED
