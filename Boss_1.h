#ifndef BOSS_1_H_INCLUDED
#define BOSS_1_H_INCLUDED

#include "Enemy.h"
#include "Player.h"
#include <vector>

struct Boss : public Enemy {
private:
    Uint32 Boss_MoveStartTime;
    Uint32 Boss_AttackStartTime;
    Uint32 Boss_lastUpdatetime;
    Uint32 Boss_2_randomAttackSpawnTime;
    Uint32 Boss_3_randomFireSpawnTime;
    SDL_Texture* enemyBoss;
public:
    bool Boss_ReachDest = true, isTeleporting;
    int dst_x, dst_y;
    vector<Enemy> Cloud;
    vector<Enemy> Fire;
    void updateHP(SDL_Renderer* ren, SDL_Rect camera, int OffSetX, int OffSetY);

    void Boss_1_Update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH],
                        int& wave, int walk_frames, int attack_frames, int die_frames,
                        int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);

    void Boss_2_Update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH],
                        int& wave, int walk_frames, int attack_frames, int die_frames,
                        int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);

    void Boss_3_Update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH],
                        int& wave, int walk_frames, int attack_frames, int die_frames,
                        int walk_y, int attack_y, int die_y, int tile_size_with, int tile_size_height);

    void Boss_2_Cloud(Uint32 crTime, SDL_Renderer* ren);

    void Boss_3_FireAttack(Uint32 crTime, SDL_Renderer* ren, Player& player);

    void loadEnemyBossTexture(SDL_Texture* tex);
};


#endif // BOSS_1_H_INCLUDED
