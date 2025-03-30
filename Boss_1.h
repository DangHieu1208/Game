#ifndef BOSS_1_H_INCLUDED
#define BOSS_1_H_INCLUDED

#include "Enemy.h"
#include "Player.h"
#include <vector>

struct Boss : public Enemy {
private:
    Uint32 Boss_1_MoveStartTime;
    Uint32 Boss_1_AttackStartTime;
    Uint32 Boss_1_lastUpdatetime;
    Uint32 Boss_2_randomAttackSpawnTime;
public:
    bool Boss_1_ReachDest = true, Boss_1_StartAttack = false, hasAttacked, isTeleporting;
    int dst_x, dst_y;
    vector<Enemy> Cloud;
    void updateHP(SDL_Renderer* ren, SDL_Rect camera, int OffSetX, int OffSetY);
    void Boss_1_Update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);
    void Boss_2_Update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);
    void Boss_3_Update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);
    void Boss_2_Cloud(Uint32 crTime, SDL_Renderer* ren);
    void Boss_2_CloudAttack(SDL_Renderer* ren, Player& player);
};


#endif // BOSS_1_H_INCLUDED
