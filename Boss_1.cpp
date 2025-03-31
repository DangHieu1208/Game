#include "Boss_1.h"
#include <cmath>

void Boss::Boss_1_Update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH],
                        int& wave, int walk_frames, int attack_frames, int die_frames,
                        int walk_y, int attack_y, int die_y, int tile_size_width, int tile_size_height) {
    if (move_right) {
        LeftOffSet = -80;
    }
    else {
        LeftOffSet = 80;
    }

    if (dst.x < player.dst.x) {
        move_right = true;
    }
    else if (dst.x > player.dst.x) {
        move_right = false;
    }

    if (player.attack_index == 0) {
        isAttacked = false;
    }

    if (crTime - Boss_lastUpdatetime >= 80 && !isAttacking && !isDied) {
        setSrc(animFrame * tile_size_width, walk_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > walk_frames) {
            animFrame = 0;
        }
        Boss_lastUpdatetime = crTime;
    }

    else if (isAttacking && !isDied) {
        if (crTime - Boss_lastUpdatetime >= 80) {
        setSrc(animFrame * tile_size_width, attack_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame == attack_frames-1 && isNearPlayer(player, 400)) {
            enemyAttack.playSound();
        }
        if (animFrame == attack_frames-1 && isNearPlayer(player, 400)) {
            if (!player.defence) {
                player.HP -= attackDamage;
            }
        }
        if (animFrame > attack_frames) {
            animFrame = 0;
            isAttacking = false;
        }
        Boss_lastUpdatetime = crTime;
        }
    }

    else if (isDied && crTime - Boss_lastUpdatetime >= 120) {
        setSrc(animFrame * tile_size_width, die_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > die_frames) {
            destroy();
            EnemyHP.destroy();
            return;
        }
        Boss_lastUpdatetime = crTime;
    }

    if (!Boss_ReachDest && !isDied && !isAttacking) {
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
            Boss_ReachDest = true;
            Boss_AttackStartTime = crTime;
            Boss_MoveStartTime = crTime;
        }
    }

    if (crTime - Boss_MoveStartTime >= 3000 && Boss_ReachDest) {
        Boss_ReachDest = false;

        int x = rand() % 20 + 4;
        int y = rand() % 12 + 1;

        dst_x = x * TILE_SIZE;
        dst_y = y * TILE_SIZE;
    }

    if (crTime - Boss_AttackStartTime >= 900 && Boss_ReachDest) {
        animFrame = 0;
        isAttacking = true;
        Boss_AttackStartTime = crTime;
    }

    else if (!isDied && isAttacking && animFrame >= 12) {
        isAttacking = false;
        animFrame = 0;
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
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
            Boss_lastUpdatetime = crTime;
            score_count = true;
        }
    }
}

void Boss::updateHP(SDL_Renderer* ren, SDL_Rect camera, int OffSetX, int OffSetY) {
    char enemyHP[10];
    sprintf(enemyHP, "%01d/%01d", HP, max_HP);
    if (!isDied) {
        EnemyHP.setPosition(dst.x - camera.x + OffSetX, dst.y - camera.y + OffSetY);
        EnemyHP.setText(enemyHP, white, ren);
        EnemyHP.render(ren);
    }
    if (!isRendered()) {
        EnemyHP.destroy();
        return;
    }
}

void Boss::Boss_2_Update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH],
                        int& wave, int walk_frames, int attack_frames, int die_frames,
                        int walk_y, int attack_y, int die_y, int tile_size_width, int tile_size_height) {

    if (player.attack_index == 0) {
        isAttacked = false;
    }

    if (!isTeleporting && !isAttacking && !isDied && crTime - Boss_MoveStartTime >= 2500) {
        isTeleporting = true;
        animFrame = 0;
        Boss_MoveStartTime = crTime;

        int x = rand() % (26 - 5 + 1) + 5;
        int y = rand() % (13 - 4 + 1) + 4;
        dst_x = x * 80;
        dst_y = y * 80;

        if (!isNearPlayer(player, 500)) {
            dst_x = player.dst.x - 100;
            dst_y = player.dst.y - 30;
        }
    }

    if (!isTeleporting && !isAttacking && !isDied && crTime - Boss_lastUpdatetime >= 80) {
        setSrc(animFrame * tile_size_width, walk_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > walk_frames) {
            animFrame = 0;
        }
        Boss_lastUpdatetime = crTime;
    }

    else if (isAttacking && !isDied) {
        if (crTime - Boss_lastUpdatetime >= 80) {
            setSrc(animFrame * tile_size_width, attack_y * tile_size_height, tile_size_width, tile_size_height);
            animFrame++;
            if (animFrame == attack_frames - 1) {
                enemyAttack.playSound();
                if (!player.defence && checkCollision(player.dst)) {
                    player.HP -= attackDamage;
                    int x = rand() % 29 + 1;
                    int y = rand() % 13 + 1;
                    player.dst.x = x * TILE_SIZE;
                    player.dst.y = y * TILE_SIZE;
                }
            }
            if (animFrame > attack_frames) {
                animFrame = 0;
                isAttacking = false;
            }
            Boss_lastUpdatetime = crTime;
        }
    }

    else if (isDied && crTime - Boss_lastUpdatetime >= 120) {
        setSrc(animFrame * tile_size_width, die_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > die_frames) {
            destroy();
            EnemyHP.destroy();
            return;
        }
        Boss_lastUpdatetime = crTime;
    }

    else if (isTeleporting && crTime - Boss_lastUpdatetime >= 80) {
        setSrc(animFrame * tile_size_width, 4 * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > 7) {
            animFrame = 0;
            dst.x = dst_x;
            dst.y = dst_y;
            isTeleporting = false;
            Boss_ReachDest = true;
        }
        Boss_lastUpdatetime = crTime;
    }


    if (Boss_ReachDest && !isTeleporting && !isAttacking && !isDied && crTime - Boss_AttackStartTime >= 1000) {
        animFrame = 0;
        isAttacking = true;
        Boss_AttackStartTime = crTime;
    }

    if (!isDied && !isAttacked && player.isAttacking && checkCollision(player.dst) && player.attack_index == 5) {
        HP -= player.attackDamage;
        isAttacked = true;
        if (HP <= 0) {
            isDied = true;
            animFrame = 0;
            Boss_lastUpdatetime = crTime;
            score_count = true;
        }
    }
}

void Boss::Boss_3_Update(Uint32 crTime, Player& player, int mapTiles[MAP_HEIGHT][MAP_WIDTH], SDL_Rect mapTileRects[MAP_HEIGHT][MAP_WIDTH],
                        int& wave, int walk_frames, int attack_frames, int die_frames,
                        int walk_y, int attack_y, int die_y, int tile_size_width, int tile_size_height) {
    if (!move_right) {
        LeftOffSet = 30;
    }
    else {
        LeftOffSet = 240;
    }

    if (dst.x + 200 > player.dst.x) {
        move_right = true;
    }
    else if (dst.x + 200 < player.dst.x) {
        move_right = false;
    }

    if (player.attack_index == 0) {
        isAttacked = false;
    }

    if (crTime - Boss_lastUpdatetime >= 80 && !isAttacking && !isDied) {
        setSrc(animFrame * tile_size_width, walk_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > walk_frames) {
            animFrame = 0;
        }
        Boss_lastUpdatetime = crTime;
    }

    else if (isAttacking && !isDied) {
        if (crTime - Boss_lastUpdatetime >= 60) {
        setSrc(animFrame * tile_size_width, attack_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame == attack_frames-1 && ((player.dst.x < dst.x + dst.w && player.dst.x > dst.x) || (player.dst.x > dst.x && player.dst.x < dst.x + dst.w)) && (player.dst.y > dst.y && player.dst.y < dst.y + dst.h)) {
            enemyAttack.playSound();
            if (!player.defence) {
                player.HP -= attackDamage;
            }
        }
        if (animFrame > attack_frames) {
            animFrame = 0;
            isAttacking = false;
        }
        Boss_lastUpdatetime = crTime;
        }
    }

    else if (isDied && crTime - Boss_lastUpdatetime >= 80) {
        setSrc(animFrame * tile_size_width, die_y * tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > die_frames) {
            destroy();
            EnemyHP.destroy();
            return;
        }
        Boss_lastUpdatetime = crTime;
    }

    if (!Boss_ReachDest && !isDied && !isAttacking) {
        if (dst.x < dst_x - 5) {
            dst.x += speed;
            move_right = false;
        }
        else if (dst.x > dst_x + 5) {
            dst.x -= speed;
            move_right = true;
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
            Boss_ReachDest = true;
            Boss_AttackStartTime = crTime;
            Boss_MoveStartTime = crTime;
        }
    }

    if (crTime - Boss_MoveStartTime >= 2000 && Boss_ReachDest) {
        Boss_ReachDest = false;


        if (dst.x + dst.w < player.dst.x) {
            dst_x = player.dst.x - dst.w + 200;
        }
        else if (dst.x > player.dst.x + player.dst.w) {
            dst_x = player.dst.x + player.dst.w - 200;
        }
        dst_y = player.dst.y - (dst.h - player.dst.h);
    }


    if (crTime - Boss_AttackStartTime >= 900 && Boss_ReachDest) {
        animFrame = 0;
        isAttacking = true;
        Boss_AttackStartTime = crTime;
    }

    else if (!isDied && isAttacking && animFrame >= 14) {
        isAttacking = false;
        animFrame = 0;
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
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
            Boss_lastUpdatetime = crTime;
            score_count = true;
        }
    }
}

void Boss::Boss_2_Cloud(Uint32 crTime, SDL_Renderer* ren) {
    if (isDestroyed) {
        return;
    }
    if (crTime - Boss_2_randomAttackSpawnTime >= 3000) {
        Enemy cloud;
        cloud.setTexture(enemyBoss);
        if (!cloud.isRendered()) {
            return;
        }
        cloud.setSrc(0 * 140, 6 * 93, 140, 93);
        cloud.setDst(dst_x, dst_y, 140, 93);
        cloud.attackDamage = 2;
        cloud.speed = 7;
        cloud.HP = 1;
        cloud.max_HP = 1;
        Cloud.push_back(cloud);
        Boss_2_randomAttackSpawnTime = crTime;
    }
}

void Boss::Boss_3_FireAttack(Uint32 crTime, SDL_Renderer* ren, Player& player) {
    if (isDestroyed) {
        return;
    }
    if (crTime - Boss_3_randomFireSpawnTime >= 4000) {
        Enemy fire;
        fire.setTexture(enemyBoss);
        if (!fire.isRendered()) {
            return;
        }
        fire.setSrc(0, 0, 96, 96);
        fire.setDst(player.dst.x, player.dst.y, 192, 192);
        Fire.push_back(fire);
        Boss_3_randomFireSpawnTime = crTime;
        fire.lastingTime = crTime;
    }
}

void Boss::loadEnemyBossTexture(SDL_Texture* tex) {
    enemyBoss = tex;
}


