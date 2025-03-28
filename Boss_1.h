#ifndef BOSS_1_H_INCLUDED
#define BOSS_1_H_INCLUDED

#include "Enemy.h"

struct Boss : public Enemy {
private:
    Uint32 Boss_1_MoveStartTime;
    Uint32 Boss_1_AttackStartTime;
    Uint32 Boss_1_lastUpdatetime;
public:
    bool Boss_1_ReachDest = true, Boss_1_StartAttack = false, hasAttacked;
    int dst_x, dst_y;
    void updateHP(SDL_Renderer* ren, SDL_Rect camera);
    void Boss_1_Update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);
};


#endif // BOSS_1_H_INCLUDED
