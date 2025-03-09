#include "Enemy.h"
#include <math.h>

void Enemy::update(Uint32 crTime, Player& player, int mapTiles[18][32], SDL_Rect mapTileRects[18][32] ) {
    /*if (crTime - lastUpdateTime >= 80) {
        setSrc(animFrame*0, 0, 56, 56);
        animFrame++;
        if (animFrame > 5) {
            animFrame = 0;
        }
        lastUpdateTime = crTime;
    }*/

    if (isNearPlayer(player, 300)) {
        chasePlayer(player);
    }

    else {
        if (move_right) {
            dst.x += speed;
            if (dst.x > 1000) move_right = false;
        }
        else {
            dst.x -= speed;
            if (dst.x < 0) move_right = true;
        }
    }

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            if (mapTiles[i][j] == 1) {
                if (checkCollision(mapTileRects[i][j])) {
                    solveCollision(mapTileRects[i][j]);
                    if (!isNearPlayer(player, 300)) {
                        move_right = !move_right;
                    }
                }
            }
        }
    }
}

bool Enemy::isNearPlayer(Player& player, int range) {
    int dx = dst.x - player.dst.x;
    int dy = dst.y - player.dst.y;
    float distance = sqrt(dx*dx + dy*dy);
    return distance < range;
}

void Enemy::chasePlayer(Player& player) {
    if (dst.x < player.dst.x) {
        dst.x += speed;
        move_right = true;
    }
    else {
        dst.x -= speed;
        move_right = false;
    }

    if (dst.y < player.dst.y) {
        dst.y += speed;
    }
    else {
        dst.y -= speed;
    }
}

void Enemy::renderEnemy(SDL_Renderer* ren, SDL_Rect& camera) {
    SDL_Rect renderQuad = {dst.x - camera.x, dst.y - camera.y, dst.w, dst.h};
    if (move_right) {
       renderFlip(ren, renderQuad);
    }
    else {
        render(ren, renderQuad);
    }
}
