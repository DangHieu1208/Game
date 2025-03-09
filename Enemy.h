#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "Player.h"

struct Enemy : public Entity {
private:
    int speed = 3;
    Uint32 lastUpdateTime = SDL_GetTicks();
    int animFrame = 0;
public:
    Enemy() {;}
    bool move_right = true;
    void update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32]);
    void renderEnemy(SDL_Renderer* ren, SDL_Rect& camera);
    bool isNearPlayer(Player& player, int range);
    void chasePlayer(Player& player);
};

#endif // ENEMY_H_INCLUDED
