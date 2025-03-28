#include "Boss_1.h"
#include <cmath>

void Boss::Boss_1_Update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_width, int tile_size_height) {
    if (move_right) {
        LeftOffSet = 80;
    }
    else {
        LeftOffSet = -80;
    }

    if (player.attack_index == 0) {
        isAttacked = false;
    }

    if (crTime - Boss_1_lastUpdatetime >= 80 && !isAttacking && !isDied) {
        setSrc(animFrame*tile_size_width, walk_y*tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > walk_frames) {
            animFrame = 0;
        }
        Boss_1_lastUpdatetime = crTime;
    }

    else if (isAttacking && !isDied) {
        if (crTime - Boss_1_lastUpdatetime >= 80) {
        setSrc(animFrame*tile_size_width, attack_y*tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame == attack_frames-1) {
            enemyAttack.playSound();
        }
        if (animFrame == attack_frames-1) {
            if (!player.defence) {
                player.HP -= attackDamage;
            }
        }
        if (animFrame > attack_frames) {
            animFrame = 0;
            isAttacking = false;
        }
        Boss_1_lastUpdatetime = crTime;
        }
    }

    else if (isDied && crTime - Boss_1_lastUpdatetime >= 120) {
        setSrc(animFrame*tile_size_width, die_y*tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > die_frames) {
            destroy();
            EnemyHP.destroy();
            return;
        }
        Boss_1_lastUpdatetime = crTime;
    }

    if (!Boss_1_ReachDest && !isDied && !isAttacking) {
        if (dst.x < dst_x - 5) {
            dst.x += speed;
            move_right = true;
        }
        else if (dst.x > dst_x + 5) {
            dst.x -= speed;
            move_right = false;
        }
        if (dst.y < dst_y - 10) {
            dst.y += speed;
        }
        else if (dst.y > dst_y + 10) {
            dst.y -= speed;
        }

        if (abs(dst.x - dst_x) <= 10 && abs(dst.y - dst_y) <= 10) {
            dst.x = dst_x;
            dst.y = dst_y;
            Boss_1_ReachDest = true;
            Boss_1_AttackStartTime = crTime;
            Boss_1_MoveStartTime = crTime;
        }
    }

    if (crTime - Boss_1_MoveStartTime >= 4000 && Boss_1_ReachDest) {
        Boss_1_ReachDest = false;

        int x = rand() % 20 + 4;
        int y = rand() % 12 + 1;

        dst_x = x*80;
        dst_y = y*80;
    }


    if (crTime - Boss_1_AttackStartTime >= 3000 && Boss_1_ReachDest) {
        animFrame = 0;
        isAttacking = true;
        Boss_1_AttackStartTime = crTime;
    }

    else if (!isDied && isAttacking && animFrame >= 12) {
        isAttacking = false;
        animFrame = 0;
    }

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            if (mapTiles[i][j] == 1 || mapTiles[i][j] == 2 || mapTiles[i][j] == 3 || mapTiles[i][j] == 4) {
                if (checkCollision(mapTileRects[i][j])) {
                    solveCollision(mapTileRects[i][j], player);
                }
            }
        }
    }

    if (!isDied && !isAttacked && player.isAttacking && checkCollision(player.dst) && player.attack_index == 5) {
        HP -= player.attackDamage;
        isAttacked = true;
        if (HP <= 0) {
            isDied = true;
            animFrame = 0;
            Boss_1_lastUpdatetime = crTime;
            score_count = true;
        }
    }
}

void Boss::updateHP(SDL_Renderer* ren, SDL_Rect camera) {
    char enemyHP[10];
    sprintf(enemyHP, "%01d/%01d", HP, max_HP);
    if (!isDied) {
        EnemyHP.setPosition(dst.x - camera.x + 140, dst.y - camera.y + 100);
        EnemyHP.setText(enemyHP, {255, 255, 255, 255}, ren);
        EnemyHP.render(ren);
    }
    if (!isRendered()) {
        EnemyHP.destroy();
        return;
    }
}
