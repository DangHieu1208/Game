#include "Tile_map.h"
#include <cstdlib>

void Map::loadMap(SDL_Renderer* ren, const char* file_name) {
    ifstream file(file_name);
    int n;

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            file >> tiles[i][j];
        }
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            tile[i][j].x = j * TILE_SIZE;
            tile[i][j].y = i * TILE_SIZE;
            tile[i][j].w = TILE_SIZE;
            tile[i][j].h = TILE_SIZE;
        }
    }
}

void Map::loadTexture(SDL_Renderer* ren) {
    for (int i = 0; i < MAP_TILES_NUMS - 1; i++) {
        mapTile[i].loadTex("graphic/tiles/tiles01.png", ren);
        if (!mapTile[i].isRendered()) {
            cout << "Failed to load texture for mapTile[" << i << "]" << endl;
        }
    }

    mapTile[0].setSrc(16, 4*16, 16, 16);

    mapTile[1].setSrc(16, 16-2, 16, 16);

    mapTile[2].setSrc(0, 9*16+4, 16, 16);

    mapTile[3].setSrc(16, 9*16+4, 16, 16);

    mapTile[4].setSrc(16, 2*16-2, 16, 16);

    mapTile[5].setSrc(4*16, 12*16, 16, 16);

    mapTile[6].loadTex("graphic/tiles/Magma.png", ren);
    mapTile[6].setSrc(0, 0, 96, 96);
}

void Map::loadEntities(SDL_Renderer* ren, Player& player) {
    player.loadHP(ren);

    boss_1.loadTex("graphic/boss/Boss_1.png", ren);
    boss_1.setSrc(0, 0, 160, 128);
    boss_1.setDst(16 * TILE_SIZE, 9 * TILE_SIZE, 320, 256);
    boss_1.attackDamage = BOSS_DAMAGE;
    boss_1.speed = BOSS_SPEED;
    boss_1.HP = BOSS_HP;
    boss_1.max_HP = boss_1.HP;
    boss_1.TopOffSet = 160;

    boss_2.loadTex("graphic/boss/Boss_2.png", ren);
    boss_2.setSrc(0, 0, 140, 93);
    boss_2.setDst(16 * TILE_SIZE, 9 * TILE_SIZE, 280, 186);
    boss_2.attackDamage = BOSS_DAMAGE;
    boss_2.speed = BOSS_SPEED;
    boss_2.HP = BOSS_HP + 100;
    boss_2.max_HP = boss_2.HP;
    boss_2.TopOffSet = 160;
    boss_2.LeftOffSet = 80;
    boss_2.move_right = true;

    boss_3.loadTex("graphic/boss/Boss_3.png", ren);
    boss_3.setSrc(0, 0, 288, 160);
    boss_3.setDst(12 * TILE_SIZE, 6 * TILE_SIZE, 576, 320);
    boss_3.attackDamage = BOSS_DAMAGE;
    boss_3.speed = BOSS_SPEED;
    boss_3.HP = boss_3.HP;
    boss_3.max_HP = BOSS_HP + 100;
    boss_3.TopOffSet = 130;
    boss_3.LeftOffSet = 250;

    Score.loadFont("font/font.ttf", 30, ren);
    Score.setPosition(1150, 0);

    Time.loadFont("font/font.ttf", 30, ren);
    Time.setPosition(1280 / 2 - 10, 0);

    Wave.loadFont("font/font.ttf", 30, ren);
    Wave.setPosition(1280 / 2 - 50, 0);

    UpgradePoints.loadFont("font/font.ttf", 30, ren);
    UpgradePoints.setPosition(990, 0);

    Upgrade.loadFont("font/font.ttf", 30, ren);
    Upgrade.setPosition(40, 48);
    Upgrade.setText("Press 1 to increase HP by 3 [2 points], Press 2 to upgrade attack [5 points], Press 3 to upgrade speed [3 points]", {255, 255, 255, 255}, ren);

    BossWaveWarn.loadFont("font/font.ttf", 30, ren);
    BossWaveWarn.setPosition (1280/2 - 90, 96);
    BossWaveWarn.setText("BOSS WAVE COMING", red, ren);
}

void Map::loadEnemyTexture(SDL_Renderer* ren) {
    SDL_Surface* surface;

    surface = IMG_Load("graphic/enemy/skeleton.png");
    if (surface) {
        skeleton = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }

    else {
        cout << "Failed to load skeleton texture: " << IMG_GetError() << endl;
    }

    surface = IMG_Load("graphic/enemy/rat.png");
    if (surface) {
        rat = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }

    else {
        cout << "Failed to load rat texture: " << IMG_GetError() << endl;
    }

    surface = IMG_Load("graphic/enemy/slime.png");
    if (surface) {
        slime = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }

    else {
        cout << "Failed to load slime texture: " << IMG_GetError() << endl;
    }

    surface = IMG_Load("graphic/enemy/slime_2.png");
    if (surface) {
        slime_boss = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }

    else {
        cout << "Failed to load slime_2 texture: " << IMG_GetError() << endl;
    }

    surface = IMG_Load("graphic/boss/Boss_2.png");
    if (surface) {
        cloud = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }

    else {
        cout << "Failed to load cloud texture: " << IMG_GetError() << endl;
    }

    surface = IMG_Load("graphic/Fire.png");
    if (surface) {
        fire = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }

    else {
        cout << "Failed to load fire texture: " << IMG_GetError() << endl;
    }
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
            if (tiles[i][j] == 5) {
                if (crTime - trapStartTime >= 300) {
                    mapTile[tiles[i][j]].setSrc(trap_index * 16, 12 * 16, 16, 16);
                    trap_index++;
                    trapStartTime = crTime;
                    if (trap_index > 4) {
                        trap_index = 1;
                    }
                }
                if (player.checkCollision(wallRect)) {
                    if (crTime - player.TrappedStartTime >= 1000) {
                        player.HP--;
                        player.TrappedStartTime = crTime;
                    }
                }
            }
            if (tiles[i][j] == 6) {
                if (player.checkCollision(wallRect)) {
                    if (crTime - player.TrappedStartTime >= 300) {
                        player.HP--;
                        player.TrappedStartTime = crTime;
                    }
                    player.solveCollision(wallRect);
                }
            }
        }
    }


    if (enemySpawn) {
        randomSpawnSkeleton(crTime, ren);
        randomSpawnRat(crTime, ren);
        randomSpawnSlime(crTime, ren);

        for (size_t i = 0; i < rats.size();) {
            rats[i].loadHP(ren);
            rats[i].update(crTime, player, tiles, tile, wave, 7, 8, 5, 1, 5, 7, 64, 64);
            rats[i].renderEnemy(ren, player.camera);
            rats[i].updateHP(ren, player);
            updateScore(ren, rats[i]);
            if ((rats[i].isDestroyed && rats[i].animFrame >= 5) || !rats[i].isRendered()) {
                rats.erase(rats.begin() + i);
                rat_killed++;
            } else {
                i++;
            }
        }

        for (size_t i = 0; i < skeletons.size();) {
            skeletons[i].loadHP(ren);
            skeletons[i].update(crTime, player, tiles, tile, wave, 7, 9, 5, 1, 2, 5, 64, 64);
            skeletons[i].renderEnemy(ren, player.camera);
            skeletons[i].updateHP(ren, player);
            updateScore(ren, skeletons[i]);
            if ((skeletons[i].isDestroyed && skeletons[i].animFrame >= 5) || !skeletons[i].isRendered()) {
                skeletons.erase(skeletons.begin() + i);
                skeleton_killed++;
            } else {
                i++;
            }
        }

        for (size_t i = 0; i < slimes.size();) {
            slimes[i].loadHP(ren);
            slimes[i].update(crTime, player, tiles, tile, wave, 11, 12, 12, 2, 0, 1, 64, 64);
            slimes[i].renderEnemy(ren, player.camera);
            slimes[i].updateHP(ren, player);
            updateScore(ren, slimes[i]);
            if ((slimes[i].isDestroyed && slimes[i].animFrame >= 12) || !slimes[i].isRendered()) {
                slimes.erase(slimes.begin() + i);
                slime_killed++;
            } else {
                i++;
            }
        }
    }

    if (isBossWave && currentBoss != NONE) {
        boss.loadHP(ren);
        switch (currentBoss) {
            case BOSS_1:
                killAllSlimes = false;

                boss.Boss_1_Update(SDL_GetTicks(), player, tiles, tile, wave, 7, 12, 8, 1, 2, 6, 160, 128);
                boss.renderEnemy(ren, player.camera);
                boss.updateHP(ren, player.camera, 140, 100);

                randomSpawnSlimeBoss(crTime, ren);

                for (size_t i = 0; i < slimes.size();) {
                    slimes[i].loadHP(ren);
                    slimes[i].update(SDL_GetTicks(), player, tiles, tile, wave, 11, 12, 12, 2, 0, 1, 64, 64);
                    slimes[i].renderEnemy(ren, player.camera);
                    slimes[i].updateHP(ren, player);
                    if ((slimes[i].isDestroyed && slimes[i].animFrame >= 12) || !slimes[i].isRendered()) {
                        slimes.erase(slimes.begin() + i);
                        if (player.HP < player.max_HP && !isBossKilled) {
                            player.HP += 2;
                        }

                    } else {
                        i++;
                    }
                }
                break;

            case BOSS_2:

                boss.Boss_2_Cloud(crTime, ren);
                for (size_t i = 0; i < boss.Cloud.size();) {
                    boss.Cloud[i].loadHP(ren);
                    boss.Cloud[i].update(SDL_GetTicks(), player, tiles, tile, wave, 7, 7, 7, 6, 6, 6, 140, 93);
                    boss.Cloud[i].renderEnemy(ren, player.camera);
                    boss.Cloud[i].updateHP(ren, player);
                    if ((boss.Cloud[i].isDestroyed && boss.Cloud[i].animFrame >= 7)) {
                        boss.Cloud.erase(boss.Cloud.begin() + i);
                    } else {
                        i++;
                    }
                }

                boss.Boss_2_Update(SDL_GetTicks(), player, tiles, tile, wave, 7, 7, 7, 0, 2, 7, 140, 93);
                boss.renderEnemy(ren, player.camera);
                boss.updateHP(ren, player.camera, boss.move_right ? 190 : 50, 50);
                break;

            case BOSS_3:
                killAllSlimes = false;

                boss.Boss_3_Update(SDL_GetTicks(), player, tiles, tile, wave, 11, 14, 14, 1, 2, 4, 288, 160);
                boss.renderEnemy(ren, player.camera);
                boss.updateHP(ren, player.camera, boss.move_right ? 240 : 280, 130);

                boss.Boss_3_FireAttack(crTime, ren, player);
                randomSpawnSlimeBoss_2(crTime, ren);

                for (size_t i = 0; i < boss.Fire.size();) {

                    boss.Fire[i].renderEnemy(ren, player.camera);

                    if (crTime - boss.Fire[i].lastingTime >= 500) {
                        if (player.checkCollision(boss.Fire[i].dst)) {
                            if(boss.Fire[i].attack_count != 0) {
                                player.HP--;
                            }
                        }
                        boss.Fire[i].attack_count++;
                        boss.Fire[i].lastingTime = crTime;
                    }


                    if (boss.Fire[i].attack_count == 6) {
                        boss.Fire[i].destroy();
                    }


                    if (boss.Fire[i].isDestroyed) {
                        boss.Fire.erase(boss.Fire.begin() + i);
                    } else {
                        i++;
                    }
                }

                for (size_t i = 0; i < slimes_boss.size();) {
                    slimes_boss[i].loadHP(ren);
                    slimes_boss[i].update(SDL_GetTicks(), player, tiles, tile, wave, 7, 7, 4, 0, 1, 2, 32, 25);
                    slimes_boss[i].renderEnemy(ren, player.camera);
                    slimes_boss[i].updateHP(ren, player);
                    if ((slimes_boss[i].isDestroyed && slimes_boss[i].animFrame >= 12) || !slimes_boss[i].isRendered()) {
                        slimes_boss.erase(slimes_boss.begin() + i);
                        if (player.HP < player.max_HP && !isBossKilled) {
                            player.HP += 1;
                        }

                    } else {
                        i++;
                    }
                }
                break;
            default:
                break;
        }

        updateScore(ren, boss);

        if (boss.isDestroyed && !isBossKilled) {
            isBossKilled = true;
            switch (currentBoss) {
                case BOSS_1:
                    killAllSlimes = true;
                    currentMap = MAP_2;
                    break;
                case BOSS_2:
                    currentMap = MAP_3;
                    isBossWave = false;
                    break;
                case BOSS_3:
                    killAllSlimes = true;
                    currentMap = MAP_1;
                    break;
                default: break;
            }
        }
    }

    if (killAllSlimes) {
        if (currentBoss == BOSS_1) {
            for (size_t i = 0; i < slimes.size(); i++) {
                slimes[i].isDied = true;
                slimes[i].animFrame = 0;
            }
        }
        else if (currentBoss == BOSS_3) {
            for (size_t i = 0; i < slimes_boss.size(); i++) {
                slimes_boss[i].isDied = true;
                slimes_boss[i].animFrame = 0;
            }
        }
    }

    char score[10];
    sprintf(score, "SCORE:%02d", score_);
    Score.setText(score, white, ren);
    Score.render(ren);

    char upgradePoint[10];
    sprintf(upgradePoint, "Point:%01d", upgrade_points);
    UpgradePoints.setText(upgradePoint, white, ren);
    UpgradePoints.render(ren);

    if ((skeleton_killed == skeleton_wave_nums && rat_killed == rat_wave_nums && slime_killed == slime_wave_nums) || isBossKilled) {
        isInterval = true;
        hpUpgraded = false;
        attackUpgraded = false;
        pointUpgraded = false;
        Wave.destroy();
    }

    if (isInterval) {
        intervalCount(crTime, ren, player);
        Upgrade.render(ren);
    } else {
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
    if (crTime - lastSkeletonSpawnTime >= 1500) {
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
                y1 = 14;
                break;
            case 2:
                y1 = 15;
                break;
        }

        if (tiles[x1][y1] == 0 || tiles[x1][y1] == 5) {
            Enemy skeleton;
            skeleton.loadTex("graphic/enemy/skeleton.png", ren);

            if (!skeleton.isRendered()) {
                return;
            }
            skeleton.setSrc(0, 64, 64, 64);
            skeleton.setDst(x1 * 80, y1 * 80, 100, 100);
            skeleton.attackDamage = SKELETON_DAMAGE + wave/4;
            skeleton.speed = rand() % (this->skeleton_base_speed - (this->skeleton_base_speed - 1) + 1)+(this->skeleton_base_speed - 1);
            skeleton.HP = this->skeleton_base_HP;
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
                x1 = 2;
                break;
            case 2:
                x1 = 3;
                break;

        }

        switch(y) {
            case 1:
                y1 = 2;
                break;
            case 2:
                y1 = 3;
                break;
        }

        if (tiles[x1][y1] == 0 || tiles[x1][y1] == 5) {
            Enemy rat;
            rat.loadTex("graphic/enemy/rat.png", ren);
            if (!rat.isRendered()) {
                return;
            }
            rat.setSrc(0,64, 64, 64);
            rat.setDst(x1 * 80, y1 * 80, 100, 100);
            rat.attackDamage = RAT_DAMAGE + wave/4;
            rat.speed = rand() % (this->rat_base_speed - (this->rat_base_speed - 1) + 1)+(this->rat_base_speed - 1);
            rat.HP = this->rat_base_HP;
            rat.max_HP = rat.HP;
            rats.push_back(rat);
            lastRatSpawnTime = crTime;
            rats_left--;
        }
        else return;
    }
}

void Map::randomSpawnSlime(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - lastSlimeSpawnTime >= 2000) {
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
                y1 = 3;
                break;
        }

        if (tiles[x1][y1] == 0 || tiles[x1][y1] == 5) {
            Enemy slime_temp;
            slime_temp.setTexture(slime);
            if (!slime_temp.isRendered()) {
                return;
            }
            slime_temp.setSrc(0, 64, 64, 64);
            slime_temp.setDst(x1 * 80, y1 * 80, 120, 120);
            slime_temp.attackDamage = SLIME_DAMAGE + wave/4;
            slime_temp.speed = rand() % (this->slime_base_speed-(this->slime_base_speed - 1) + 1)+(this->slime_base_speed - 1);
            slime_temp.HP = this->slime_base_HP;
            slime_temp.max_HP = slime_temp.HP;
            slimes.push_back(slime_temp);
            lastSlimeSpawnTime = crTime;
            slime_left--;
        }
        else return;
    }
}

void Map::randomSpawnSlimeBoss(Uint32 crTime, SDL_Renderer* ren) {
     if (crTime - lastSlimeSpawnTime >= 5000) {
        if (isBossKilled) {
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

        if (tiles[x][y] == 0 || tiles[x][y] == 5) {
            Enemy slime_temp;
            slime_temp.setTexture(slime);
            if (!slime_temp.isRendered()) {
                return;
            }
            slime_temp.setSrc(0,64, 64, 64);
            slime_temp.setDst(x1*80, y1*80, 120, 120);
            slime_temp.attackDamage = SLIME_DAMAGE + wave/4;
            slime_temp.speed = rand() % (this->slime_base_speed-(this->slime_base_speed - 1) + 1)+(this->slime_base_speed - 1);
            slime_temp.HP = this->slime_base_HP;
            slime_temp.max_HP = slime_temp.HP;
            slimes.push_back(slime_temp);
            lastSlimeSpawnTime = crTime;
        }
        else return;
    }
}

void Map::randomSpawnSlimeBoss_2(Uint32 crTime, SDL_Renderer* ren) {
    if (crTime - lastSlimeSpawnTime_2 >= 5000) {
        if (isBossKilled) {
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

        if (tiles[x1][y1] == 0 || tiles[x1][y1] == 5 || tiles[x1][y1] == 6) {
            Enemy slime_temp;
            slime_temp.setTexture(slime_boss);
            if (!slime_temp.isRendered()) {
                return;
            }
            slime_temp.setSrc(0 , 0, 32, 25);
            slime_temp.setDst(x1 * 80, y1 * 80, 120, 120);
            slime_temp.attackDamage = SLIME_DAMAGE + wave/4;
            slime_temp.speed = rand() % (this->slime_base_speed-(this->slime_base_speed - 1) + 1)+(this->slime_base_speed - 1);
            slime_temp.HP = this->slime_base_HP;
            slime_temp.max_HP = slime_temp.HP;
            slimes_boss.push_back(slime_temp);
            lastSlimeSpawnTime_2 = crTime;
        }
        else return;
    }
}

void Map::intervalCount(Uint32 crTime, SDL_Renderer* ren, Player& player) {
    if (intervalTime == INTERVAL_TIME) {
        if (!pointUpgraded) {
            upgrade_points_add = (skeleton_wave_nums + rat_wave_nums + slime_wave_nums) / 3 + 2;
            if (upgrade_points_add >= 8) {
                upgrade_points_add = 8;
            }
            if (isBossWave) {
                upgrade_points_add = 12;
            }
            upgrade_points += upgrade_points_add;
            pointUpgraded = true;
        }

        if (!enemyUpgraded) {
            if (wave % 5 == 1) {
                this->skeleton_base_speed += 1;
                this->rat_base_speed += 1;
                if (this->skeleton_base_speed > 8) {
                    this->skeleton_base_speed = 8;
                }
                if (this->rat_base_speed > 10) {
                    this->skeleton_base_speed = 10;
                }
            }
            if (wave % 5 == 1) {
                this->slime_base_speed += 1;
                if (this->slime_base_speed > 6) {
                    this->slime_base_speed = 6;
                }
            }
            this->skeleton_base_HP += 2;
            this->rat_base_HP += 2;
            this->slime_base_HP += 2;
            skeleton_killed = 0;
            rat_killed = 0;
            slime_killed = 0;
            enemyUpgraded = true;
        }
    }

    if (crTime - intervalStartTime >= 1000) {
        intervalTime--;
        intervalStartTime = crTime;
    }

    if (wave == boss_1_wave_index * 5 - 1 || wave == boss_2_wave_index * 5 - 1 || wave == boss_3_wave_index * 5 - 1) {
        BossWaveWarn.render(ren);
    }

    char time[10];
    sprintf(time, "%01d", intervalTime > 0 ? intervalTime : 0);
    Time.setText(time, white, ren);
    Time.render(ren);

    if (intervalTime <= 0) {
        if (slimes.size() == 0 || slimes_boss.size() == 0) {
            isBossWave = false;
        }

        skeletons.clear();
        rats.clear();
        slimes.clear();

        intervalTime = INTERVAL_TIME;

        if (wave % 2 == 0) {
            skeleton_wave_nums += skeleton_add;
            rat_wave_nums += rat_add;
            slime_wave_nums += slime_add;
        }

        if (skeleton_wave_nums > 8) skeleton_wave_nums = 8;
        if (rat_wave_nums > 6) rat_wave_nums = 6;
        if (slime_wave_nums > 6) slime_wave_nums = 6;
        skeleton_left = skeleton_wave_nums;
        rats_left = rat_wave_nums;
        slime_left = slime_wave_nums;

        Time.destroy();

        wave++;

        if (!isBossWave) {
            if (currentMap == MAP_1) {
                loadMap(ren, "map/map1.txt");
            }
            else if (currentMap == MAP_2) {
                loadMap(ren, "map/map2.txt");
            }
            else loadMap(ren, "map/map3.txt");
        }

        if (wave == boss_1_wave_index * 5) {
            player.dst.x = 9 * TILE_SIZE;
            player.dst.y = 8 * TILE_SIZE;
            boss = boss_1;
            boss.HP += (boss_1_wave_index - 1) * 100;
            boss.max_HP = boss.HP;
            boss.attackDamage += (boss_1_wave_index - 1) * 4;
            boss.isDied = false;
            boss.isAttacking = false;
            boss.animFrame = 0;
            loadMap(ren, "map/map_boss_1.txt");
            isBossWave = true;
            currentBoss = BOSS_1;
            enemySpawn = false;
            boss_1_wave_index += 3;
        }

        else if (wave == boss_2_wave_index * 5) {
            player.dst.x = 7 * TILE_SIZE;
            player.dst.y = 8 * TILE_SIZE;
            boss = boss_2;
            boss.loadEnemyBossTexture(cloud);
            boss.HP += (boss_2_wave_index - 2) * 100;
            boss.max_HP = boss.HP;
            boss.attackDamage += (boss_2_wave_index - 2) * 4;
            boss.isDied = false;
            boss.isAttacking = false;
            boss.animFrame = 0;
            loadMap(ren, "map/map_boss_2.txt");
            isBossWave = true;
            currentBoss = BOSS_2;
            enemySpawn = false;
            boss_2_wave_index += 3;
        }

        else if (wave == boss_3_wave_index * 5) {
            player.dst.x = 7 * TILE_SIZE;
            player.dst.y = 8 * TILE_SIZE;
            boss = boss_3;
            boss.loadEnemyBossTexture(fire);
            boss.HP += (boss_3_wave_index - 3) * 100;
            boss.max_HP = boss.HP;
            boss.attackDamage += (boss_3_wave_index - 3) * 4;
            boss.isDied = false;
            boss.isAttacking = false;
            boss.animFrame = 0;
            loadMap(ren, "map/map_boss_3.txt");
            isBossWave = true;
            currentBoss = BOSS_3;
            enemySpawn = false;
            boss_3_wave_index += 3;
        }

        else {
            isBossWave = false;
            currentBoss = NONE;
            enemySpawn = true;
        }

        isInterval = false;
        pointUpgraded = false;
        enemyUpgraded = false;
        isBossKilled = false;
    }
}

void Map::renderWave(SDL_Renderer* ren) {
    char wave_[10];
    sprintf(wave_, "WAVE:%01d", wave);
    Wave.setText(wave_, white, ren);
    Wave.render(ren);
}

void Map::playerUpgrade(SDL_Event& event, Player& player) {
    if (event.type == SDL_KEYDOWN && isInterval) {
        switch(event.key.keysym.sym) {
            case SDLK_1:
                if (!hpUpgraded && upgrade_points >= 2) {
                    player.HP += 3;
                    if (player.HP >= player.max_HP) {
                        player.max_HP = player.HP;
                    }
                    upgrade_points -= 2;
                    hpUpgraded = true;
                }
                break;
            case SDLK_2:
                if (!attackUpgraded && upgrade_points >= 5) {
                    player.attackDamage += 1;
                    upgrade_points -= 5;
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
