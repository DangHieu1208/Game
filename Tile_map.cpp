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

    for (int i = 0; i < 8; i++) {
        mapTile[i].loadTex("graphic/tiles01.png", ren);
    }

    mapTile[0].setSrc(16, 4*16, 16, 16);

    mapTile[1].setSrc(16, 16-2, 16, 16);

    mapTile[2].setSrc(0, 9*16+4, 16, 16);

    mapTile[3].setSrc(16, 9*16+4, 16, 16);

    mapTile[4].setSrc(16, 2*16-2, 16, 16);

    mapTile[5].setSrc(4*16, 12*16, 16, 16);

    mapTile[6].setSrc(4*16, 16-2, 16, 16);

    mapTile[7].setSrc(4*16, 4*16, 16, 16);


    player.loadHP(ren);

    Score.loadFont("font.ttf", 30, ren);
    Score.setPosition(1190, 0);

    Time.loadFont("font.ttf", 30, ren);
    Time.setPosition(1280/2-10, 0);

    Wave.loadFont("font.ttf", 30, ren);
    Wave.setPosition(1280/2 - 50, 0);

    UpgradePoints.loadFont("font.ttf", 30, ren);
    UpgradePoints.setPosition(990, 0);

    Upgrade.loadFont("font.ttf", 30, ren);
    Upgrade.setPosition(1280/2 - 430, 48);
    Upgrade.setText("Press 1 to increase HP by 2, Press 2 to upgrade attack, Press 3 to upgrade speed", {255, 255, 255 ,255}, ren);

    srand(time(NULL));
}

void Map::renderMap(SDL_Renderer* ren, Player& player, Uint32 crTime) {

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            SDL_Rect renderQuad = {tile[i][j].x - player.camera.x, tile[i][j].y - player.camera.y, 80, 80};
            SDL_Rect wallRect = {tile[i][j].x, tile[i][j].y, 80, 80};
            mapTile[tiles[i][j]].render(ren, renderQuad, player.camera);
            if (tiles[i][j] == 1 || tiles[i][j] == 2 || tiles[i][j] == 3 || tiles[i][j] == 4) {
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
            if (tiles[i][j] == 0) {
                randomSpawnEnemy(crTime, ren);
            }
            if (tiles[i][j] == 5) {
                if (crTime - trapStartTime >= 400) {
                    mapTile[tiles[i][j]].setSrc(trap_index*16, 12*16, 16, 16);
                    trap_index++;
                    trapStartTime = crTime;

                    if (trap_index > 3) {
                        trap_index = 1;
                    }
                }

                if (player.checkCollision(wallRect)) {
                    if (crTime - player.TrappedStartTime >= 1000) {
                        player.HP --;
                        player.TrappedStartTime = crTime;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < enemies.size();) {
        enemies[i].loadHP(ren);
        enemies[i].updateHP(ren, player);
        enemies[i].update(SDL_GetTicks(), player, tiles, tile, wave);
        enemies[i].renderEnemy(ren, player.camera);
        enemies[i].updateHP(ren, player);
        updateScore(ren, enemies[i]);
        if (enemies[i].isDied && enemies[i].animFrame > 5) {
            enemies.erase(enemies.begin() + i);
            enemy_killed++;
        }
        else {
            i++;
        }
    }

    char score[10];
    sprintf(score, "SCORE:%02d", score_);
    Score.setText(score, {255, 255, 255, 255}, ren);
    Score.render(ren);

    char upgradePoint[10];
    sprintf(upgradePoint, "Point:%01d", upgrade_points);
    UpgradePoints.setText(upgradePoint, {255, 255, 255, 255}, ren);
    UpgradePoints.render(ren);


    if (enemy_killed == ENEMY_NUMS_WAVE_1) {
        isInterval = true;
        hpUpgraded = false;
        attackUpgraded = false;
        pointUpgraded = false;
        Wave.destroy();
        if (!enemyAttackUpgraded && wave % 2 == 0) {
            enemyDamage += 1;
            enemy_base_speed += 1;
            enemyAttackUpgraded = true;
        }
        enemy_base_HP += 1;
        enemy_killed = 0;
    }

    if (isInterval) {
        intervalCount(crTime, ren);
        if (!pointUpgraded) {
            upgrade_points = upgrade_points + ENEMY_NUMS_WAVE_1/2 + 2;
            pointUpgraded = true;
        }
        Upgrade.render(ren);
    }
    else {
        renderWave(ren);
        enemyAttackUpgraded = false;
    }

    player.updateHP(ren);
}

void Map::updateScore(SDL_Renderer* ren, Enemy& enemy) {
    if (enemy.score_count) {
        score_++;
        enemy.score_count = false;
    }
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
    if (crTime - lastSpawnTime >= 2000) {
        if (enemy_left == 0) {
            return;
        }

        int x = rand() % 30;
        int y = rand() % 16;
        Enemy enemy0;
        enemy0.loadTex("graphic/enemy.png", ren);
        if (!enemy0.isRendered()) {
            return;
        }
        enemy0.setSrc(0,64, 64, 64);
        enemy0.setDst(x*80, y*80, 120, 100);
        enemy0.attackDamage = enemyDamage;
        enemies.push_back(enemy0);
        lastSpawnTime = crTime;
        enemy_left--;
    }
}

void Map::intervalCount(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - intervalStartTime >= 1000) {
        intervalTime--;
        intervalStartTime = crTime;
    }

    char time[10];
    sprintf(time, "%01d", intervalTime > 0 ? intervalTime : 0);
    Time.setText(time, {255, 255, 255, 255}, ren);
    Time.render(ren);


    if (intervalTime <= 0) {
        intervalTime = 15;
        ENEMY_NUMS_WAVE_1 += ENEMY_ADD_EACH_WAVE;
        ENEMY_ADD_EACH_WAVE = ENEMY_NUMS_WAVE_1/2;
        enemy_left = ENEMY_NUMS_WAVE_1;
        Time.destroy();
        wave++;
        isInterval = false;
    }
}

void Map::renderWave(SDL_Renderer* ren) {
    char wave_[10];
    sprintf(wave_, "WAVE:%01d", wave);
    Wave.setText(wave_, {255, 255, 255, 255}, ren);
    Wave.render(ren);
}

void Map::playerUpgrade(SDL_Event& event, Player& player) {
    if (event.type == SDL_KEYDOWN && isInterval) {
        switch(event.key.keysym.sym) {
            case SDLK_1:
                if (!hpUpgraded && upgrade_points >= 3) {
                    player.HP += 2;
                    if (player.HP >= player.max_HP) {
                        player.max_HP = player.HP;
                    }
                    upgrade_points -= 3;
                    hpUpgraded = true;
                }
                break;
            case SDLK_2:
                if (!attackUpgraded && upgrade_points >= 5) {
                    player.attackDamage += 1;
                    upgrade_points -= 3;
                    attackUpgraded = true;
                }
                break;
            case SDLK_3:
                if (!speedUpgraded && upgrade_points >= 3) {
                    player.speed += 1;
                    upgrade_points -= 4;
                    speedUpgraded = true;
                }
                break;
        }

    }
    else if (event.type == SDL_KEYUP && isInterval) {
        switch (event.key.keysym.sym) {
            case SDLK_1:
                if (upgrade_points >= 2) {
                    hpUpgraded = false;
                }
                break;
            case SDLK_2:
                if (upgrade_points >= 5) {
                    attackUpgraded = false;
                }
                break;
            case SDLK_3:
                if (upgrade_points >= 3) {
                    speedUpgraded = false;
                }
                break;
        }
    }
}
