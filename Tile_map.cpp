#include "Tile_map.h"
#include <cstdlib>

void Map::loadMap(SDL_Renderer* ren, const char* file_name, Player& player) {
    ifstream file(file_name);
    int n;

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            file >> tiles[i][j];
        }
    }

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j <32; j++) {
            tile[i][j].x = j*80;
            tile[i][j].y = i*80;
            tile[i][j].w = 80;
            tile[i][j].h = 80;
        }
    }

    mapTile[0].loadTex("graphic/tiles.png", ren);
    mapTile[0].setSrc(0, 16, 32, 32);

    mapTile[1].loadTex("graphic/tiles.png", ren);
    mapTile[1].setSrc(0, 160, 32, 32);

    /*Enemy enemy0;
    enemy0.loadTex("graphic/enemy.png", ren);
    enemy0.setSrc(0,64, 64, 64);
    enemy0.setDst(500, 500, 120, 100);
    enemies.push_back(enemy0);*/

    //enemies[0].loadHP(ren);
    player.loadHP(ren);

    Score.loadFont("font.ttf", 30, ren);
    Score.setPosition(1000, 0);

    srand(time(NULL));
}

void Map::renderMap(SDL_Renderer* ren, Player& player, Uint32 crTime) {
    static int score_ = 0;

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            SDL_Rect renderQuad = {tile[i][j].x - player.camera.x, tile[i][j].y - player.camera.y, 80, 80};
            SDL_Rect wallRect = {tile[i][j].x, tile[i][j].y, 80, 80};
            if (tiles[i + offSetY][j + offSetX] == 1) {
                mapTile[0].render(ren, renderQuad);
                if (player.move_left) {
                    player.LeftOffSet = 22;
                }
                if (player.go_down) {
                    player.TopOffSet = 22;
                }
                if (player.go_up) {
                    player.TopOffSet = 44;
                }
                if (player.checkCollision(wallRect)) {
                    player.solveCollision(wallRect);
                }
            }
            else {
                randomSpawnEnemy(crTime, ren);
                mapTile[1].render(ren, renderQuad);
            }
        }
    }

    for (size_t i = 0; i < enemies.size();) {
        enemies[i].loadHP(ren);
        enemies[i].updateHP(ren, player);
        enemies[i].update(SDL_GetTicks(), player, tiles, tile);
        enemies[i].renderEnemy(ren, player.camera);
        enemies[i].updateHP(ren, player);
        updateScore(ren, enemies[i]);
        if (enemies[i].isDied && enemies[i].animFrame > 5) {
            enemies.erase(enemies.begin() + i);
        }
        else {
            i++;
        }
    }

    player.updateHP(ren);
}

void Map::updateScore(SDL_Renderer* ren, Enemy& enemy) {
    if (enemy.score_count) {
        score_++;
        enemy.score_count = false;
    }
    char score[10];
    sprintf(score, "SCORE:%01d", score_);
    Score.setText(score, {255, 255, 255, 255}, ren);
    Score.render(ren);
}

void Map::print() {
 for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 32; j++) {
        cout << tiles[i][j] << " ";
    }
    cout << endl;
 }
}

void Map::randomSpawnEnemy(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - lastSpawnTime >= 5000) {
        int x = rand() % 30;
        int y = rand() % 16;
        Enemy enemy0;
        enemy0.loadTex("graphic/enemy.png", ren);
        if (!enemy0.isRendered()) {
            return;
        }
        enemy0.setSrc(0,64, 64, 64);
        enemy0.setDst(x*80, y*80, 120, 100);
        enemies.push_back(enemy0);
        lastSpawnTime = crTime;
    }
}
