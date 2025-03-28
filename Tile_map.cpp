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

    for (int i = 0; i < 9; i++) {
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
                /*if (wave == 3) {
                    enemySpawn = false;
                }
                else {
                    enemySpawn = true;
                }
                if (enemySpawn) {
                    randomSpawnSkeleton(crTime, ren);
                    randomSpawnRat(crTime, ren);
                    randomSpawnSlime(crTime, ren);
                }*/
            }
            if (tiles[i][j] == 5) {
                if (crTime - trapStartTime >= 300) {
                    mapTile[tiles[i][j]].setSrc(trap_index*16, 12*16, 16, 16);
                    trap_index++;
                    trapStartTime = crTime;

                    if (trap_index > 4) {
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

    randomSpawnSkeleton(crTime, ren);
    randomSpawnRat(crTime, ren);
    randomSpawnSlime(crTime, ren);

    for (size_t i = 0; i < rats.size();) {
        rats[i].loadHP(ren);
        rats[i].update(SDL_GetTicks(), player, tiles, tile, wave, 7, 8, 5, 1, 5, 7, 64);
        rats[i].renderEnemy(ren, player.camera);
        rats[i].updateHP(ren, player);
        updateScore(ren, rats[i]);
        if (rats[i].isDestroyed && rats[i].animFrame >= 5) {
            rats.erase(rats.begin() + i);
            rat_killed++;
        }
        else {
            i++;
        }
    }

    for (size_t i = 0; i < skeletons.size();) {
        skeletons[i].loadHP(ren);
        skeletons[i].update(SDL_GetTicks(), player, tiles, tile, wave, 7, 9, 5, 1, 2, 5, 64);
        skeletons[i].renderEnemy(ren, player.camera);
        skeletons[i].updateHP(ren, player);
        updateScore(ren, skeletons[i]);
        if (skeletons[i].isDestroyed && skeletons[i].animFrame >= 5) {
            skeletons.erase(skeletons.begin() + i);
            skeleton_killed++;
        }
        else {
            i++;
        }
    }

    for (size_t i = 0; i < slimes.size();) {
        slimes[i].loadHP(ren);
        slimes[i].update(SDL_GetTicks(), player, tiles, tile, wave, 11, 12, 12, 2, 0, 1, 64);
        slimes[i].renderEnemy(ren, player.camera);
        slimes[i].updateHP(ren, player);
        updateScore(ren, slimes[i]);
        if (slimes[i].isDestroyed && slimes[i].animFrame >= 12) {
            slimes.erase(slimes.begin() + i);
            slime_killed++;
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

    if ((skeleton_killed == skeleton_wave_nums && rat_killed == rat_wave_nums && slime_killed == slime_wave_nums) || isBossKilled) {
        isInterval = true;
        hpUpgraded = false;
        attackUpgraded = false;
        pointUpgraded = false;
        isBossKilled = false;
        Wave.destroy();
        if (wave % 6 == 0) {
            skeleton_base_speed += 1;
            rat_base_speed += 1;
        }
        if (wave % 7 == 0) {
            slime_base_speed += 1;
        }
        skeleton_base_HP += 1;
        rat_base_HP += 1;
        slime_base_HP += 1;
        skeleton_killed = 0;
        rat_killed = 0;
        slime_killed = 0;
    }

    if (isInterval) {
        intervalCount(crTime, ren);
        if (!pointUpgraded) {
            upgrade_points_add = (skeleton_wave_nums + rat_wave_nums + slime_wave_nums)/3 + 2;
            if (upgrade_points_add >= 8) {
                upgrade_points_add = 8;
            }
            upgrade_points += upgrade_points_add;
            pointUpgraded = true;
        }
        Upgrade.render(ren);
    }
    else {
        renderWave(ren);
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

void Map::randomSpawnSkeleton(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - lastSkeletonSpawnTime >= 1000) {
        if (skeleton_left == 0) {
            return;
        }

        int x1, y1;

        int x = rand() % 2 + 1;
        int y = rand() % 2 + 1;

        switch(x) {
            case 1:
                x1 = 15;
                break;
            case 2:
                x1 = 16;
                break;

        }

        switch(y) {
            case 1:
                y1 = 2;
                break;
            case 2:
                y1 = 15;
                break;
        }

        if (tiles[x][y] == 0) {
            Enemy skeleton;
            skeleton.loadTex("graphic/enemy.png", ren);

            if (!skeleton.isRendered()) {
                return;
            }
            skeleton.setSrc(0,64, 64, 64);
            skeleton.setDst(x1*80, y1*80, 100, 100);
            skeleton.attackDamage = SKELETON_DAMAGE + wave/4;
            skeleton.speed = rand() % (skeleton_base_speed-(skeleton_base_speed-1)+1)+(skeleton_base_speed - 1);
            skeleton.HP = rand() % (skeleton_base_HP-(skeleton_base_HP-5)+1)+(skeleton_base_HP-5);
            skeleton.max_HP = skeleton.HP;
            skeletons.push_back(skeleton);
            lastSkeletonSpawnTime = crTime;
            skeleton_left--;
        }
        else return;
    }
}

void Map::randomSpawnRat(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - lastRatSpawnTime >= 1000) {
        if (rats_left == 0) {
            return;
        }

        int x1, y1;

        int x = rand() % 2 + 1;
        int y = rand() % 2 + 1;

        switch(x) {
            case 1:
                x1 = 15;
                break;
            case 2:
                x1 = 16;
                break;

        }

        switch(y) {
            case 1:
                y1 = 2;
                break;
            case 2:
                y1 = 15;
                break;
        }

        if (tiles[x][y] == 0) {
            Enemy rat;
            rat.loadTex("graphic/rat.png", ren);
            if (!rat.isRendered()) {
                return;
            }
            rat.setSrc(0,64, 64, 64);
            rat.setDst(x1*80, y1*80, 100, 100);
            rat.attackDamage = RAT_DAMAGE + wave/4;
            rat.speed = rand() % (rat_base_speed-(rat_base_speed-1)+1)+(rat_base_speed - 1);
            rat.HP = rat_base_HP;
            rat.max_HP = rat.HP;
            rats.push_back(rat);
            lastRatSpawnTime = crTime;
            rats_left--;
        }
        else return;
    }
}

void Map::randomSpawnSlime(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - lastSlimeSpawnTime >= 1000) {
        if (slime_left == 0) {
            return;
        }

        int x1, y1;

        int x = rand() % 2 + 1;
        int y = rand() % 2 + 1;

        switch(x) {
            case 1:
                x1 = 15;
                break;
            case 2:
                x1 = 16;
                break;

        }

        switch(y) {
            case 1:
                y1 = 2;
                break;
            case 2:
                y1 = 15;
                break;
        }

        if (tiles[x][y] == 0) {
            Enemy slime;
            slime.loadTex("graphic/slime.png", ren);
            if (!slime.isRendered()) {
                return;
            }
            slime.setSrc(0,64, 64, 64);
            slime.setDst(x1*80, y1*80, 120, 120);
            slime.attackDamage = SLIME_DAMAGE + wave/4;
            slime.speed = rand() % (slime_base_speed-(slime_base_speed-1)+1)+(slime_base_speed - 1);
            slime.HP = slime_base_HP;
            slime.max_HP = slime.HP;
            slimes.push_back(slime);
            lastSlimeSpawnTime = crTime;
            slime_left--;
        }
        else return;
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
        intervalTime = INTERVAL_TIME;
        if (wave % 2 == 0) {
            skeleton_wave_nums += skeleton_add;
            rat_wave_nums += rat_add;
            slime_wave_nums += slime_add;
        }
        if (skeleton_wave_nums >= 8) skeleton_wave_nums = 10;
        if (rat_wave_nums >= 6) rat_wave_nums = 8;
        if (slime_wave_nums >= 6) slime_wave_nums = 8;
        skeleton_left = skeleton_wave_nums;
        rats_left = rat_wave_nums;
        slime_left = slime_wave_nums;
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
                if (!hpUpgraded && upgrade_points >= 2) {
                    player.HP += 2;
                    if (player.HP >= player.max_HP) {
                        player.max_HP = player.HP;
                    }
                    upgrade_points -= 2;
                    hpUpgraded = true;
                }
                break;
            case SDLK_2:
                if (!attackUpgraded && upgrade_points >= 4) {
                    player.attackDamage += 1;
                    upgrade_points -= 4;
                    attackUpgraded = true;
                }
                break;
            case SDLK_3:
                if (!speedUpgraded && upgrade_points >= 3) {
                    player.speed += 1;
                    upgrade_points -= 3;
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
                if (upgrade_points >= 4) {
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
