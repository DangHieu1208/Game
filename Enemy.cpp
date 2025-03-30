#include "Enemy.h"
#include <math.h>

void Enemy::update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32], int& wave, int walk_frames, int attack_frames, int die_frames, int walk_y, int attack_y, int die_y, int tile_size_width, int tile_size_height) {
    if (player.attack_index == 0) {
        isAttacked = false;
    }

    if (crTime - lastUpdateTime >= 80 && !isAttacking && (crTime - attackCoolDown >= 500) && !isDied) {
        setSrc(animFrame*tile_size_width, walk_y*tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > walk_frames) {
            animFrame = 0;
        }
        lastUpdateTime = crTime;
    }

    else if (isAttacking && !isDied) {
        if (crTime - lastUpdateTime >= 80) {
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
            attackCoolDown = crTime;
        }
        lastUpdateTime = crTime;
        }
    }

    else if (isDied && crTime - lastUpdateTime >= 120) {
        setSrc(animFrame*tile_size_width, die_y*tile_size_height, tile_size_width, tile_size_height);
        animFrame++;
        if (animFrame > die_frames) {
            //animFrame = die_frames;
            destroy();
            EnemyHP.destroy();
            return;
        }
        lastUpdateTime = crTime;
    }

    if (isNearPlayer(player, 1470) && !isDied) {
        chasePlayer(crTime, player);
    }

    else if (!isDied) {
        isAttacking = false;
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

    if (!isDied && !isAttacked && player.isAttacking && isCollided(player) && player.attack_index == 5) {
        HP -= player.attackDamage;
        isAttacked = true;
        if (HP <= 0) {
            isDied = true;
            animFrame = 0;
            lastUpdateTime = crTime;
            score_count = true;
        }
    }
}

bool Enemy::isNearPlayer(Player& player, int range) {
    int dx = dst.x - player.dst.x;
    int dy = dst.y - player.dst.y;
    float distance = sqrt(dx*dx + dy*dy);
    return distance < range;
}

/*void Enemy::chasePlayer(Uint32 crTime, Player& player) {
    if (!isAttacking && (crTime - attackCoolDown >= 500)) {
        if (dst.x < player.dst.x - 30 - 5) {
            dst.x += speed;
            move_right = true;
        }
        else if (dst.x > player.dst.x - 30 + 5) {
            dst.x -= speed;
            move_right = false;
        }

        if (dst.y < player.dst.y + 20 - 10) {
                dst.y += speed;
        }
        else if (dst.y > player.dst.y + 20 + 10) {
            dst.y -= speed;
        }
    }

    if (abs(dst.x - (player.dst.x - 30)) < 20 && abs(dst.y - player.dst.y - 20) < 20 && crTime - attackCoolDown >= 500) {
        isAttacking = true;
    } else if (abs(dst.x - (player.dst.x - 30)) > 20 || abs(dst.y - player.dst.y - 20) > 20) {
        isAttacking = false;
    }

    if (isAttacking) {
        move_right = true;
    }
}*/

void Enemy::chasePlayer(Uint32 crTime, Player& player) {
    if (!isAttacking && (crTime - attackCoolDown >= 500)) {

        int dx = dst.x - player.dst.x;
        int dy = dst.y - player.dst.y;
        int distance = sqrt(dx * dx + dy * dy);

        if (distance > stoppingZoneRadius) {
            if (dst.x < player.dst.x - 5) {
                dst.x += speed;
                move_right = true;
            } else if (dst.x > player.dst.x + 5) {
                dst.x -= speed;
                move_right = false;
            }

            if (dst.y < player.dst.y - 10) {
                dst.y += speed;
            } else if (dst.y > player.dst.y + 10) {
                dst.y -= speed;
            }
        }

        if (distance <= stoppingZoneRadius && crTime - attackCoolDown >= 500) {
            isAttacking = true;
        }
    }

    float distanceToPlayer = sqrt(pow(dst.x - player.dst.x, 2) + pow(dst.y - player.dst.y, 2));
    if (distanceToPlayer > attackRange) {
        isAttacking = false;
    }

    if (isAttacking) {
        move_right = (player.dst.x > dst.x);
    }
}

void Enemy::renderEnemy(SDL_Renderer* ren, SDL_Rect& camera) {
    if (isDestroyed) return;
    SDL_Rect renderQuad = {dst.x - camera.x, dst.y - camera.y, dst.w, dst.h};
    if (!move_right) {
       renderFlip(ren, renderQuad, camera);
    }
    else {
        render(ren, renderQuad, camera);
    }

    if (dst.x + dst.w < camera.x || dst.x > camera.x || dst.y + dst.h < camera.y || dst.y > camera.y) return;
}

void Enemy::loadHP(SDL_Renderer* ren) {
    EnemyHP.loadFont("font.ttf", 16, ren);
    enemyAttack.loadSound("sfx/enemyAttack.wav");
}

void Enemy::updateHP(SDL_Renderer* ren, Player& player) {
    char enemyHP[10];
    sprintf(enemyHP, "%01d/%01d", HP, max_HP);
    if (!isDied) {
        EnemyHP.setPosition(dst.x - player.camera.x + 50, dst.y - player.camera.y);
        EnemyHP.setText(enemyHP, {255, 255, 255, 255}, ren);
        EnemyHP.render(ren);
    }
    if (!isRendered()) {
        EnemyHP.destroy();
        return;
    }
}

void Enemy::solveCollision(SDL_Rect& wall, Player& player) {
    SDL_Rect entityRect = dst;
    SDL_Rect wallRect = wall;

    SDL_Rect hitbox = {
        entityRect.x + LeftOffSet,
        entityRect.y + TopOffSet,
        entityRect.w - abs(LeftOffSet),
        entityRect.h - TopOffSet
    };

    int leftOverlap = (hitbox.x + hitbox.w) - wallRect.x;
    int rightOverlap = (wallRect.x + wallRect.w) - hitbox.x;
    int topOverlap = (hitbox.y + hitbox.h) - wallRect.y;
    int bottomOverlap = (wallRect.y + wallRect.h) - hitbox.y;

    int minOverlap = leftOverlap;
    if (rightOverlap < minOverlap) minOverlap = rightOverlap;
    if (topOverlap < minOverlap) minOverlap = topOverlap;
    if (bottomOverlap < minOverlap) minOverlap = bottomOverlap;

    int dx = player.dst.x - dst.x;
    int dy = player.dst.y - dst.y;

    if (minOverlap == leftOverlap && leftOverlap > 0) {
        dst.x -= minOverlap + 1;
        if (dy > 0) dst.y += speed;
        else dst.y -= speed;
    }
    else if (minOverlap == rightOverlap && rightOverlap > 0) {
        dst.x += minOverlap + 1;
        if (dy > 0) dst.y += speed;
        else dst.y -= speed;
    }
    else if (minOverlap == topOverlap && topOverlap > 0) {
        dst.y -= minOverlap + 1;
        if (dx > 0) {
            dst.x += speed;
            move_right = true;
        } else {
            dst.x -= speed;
            move_right = false;
        }
    }
    else if (minOverlap == bottomOverlap && bottomOverlap > 0) {
        dst.y += minOverlap + 1;
        if (dx > 0) {
            dst.x += speed;
            move_right = true;
        } else {
            dst.x -= speed;
            move_right = false;
        }
    }
}
