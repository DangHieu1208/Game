#include "Tile_map.h"
#include <cstdlib>

void Map::loadMap(SDL_Renderer* ren, const char* file_name) {
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
}

void Map::loadTexture(SDL_Renderer* ren) {
    for (int i = 0; i < 6; i++) {
        mapTile[i].loadTex("graphic/tiles01.png", ren);
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

    mapTile[6].loadTex("graphic/Magma.png", ren);
    mapTile[6].setSrc(0, 0, 96, 96);

}

void Map::loadEntities(SDL_Renderer* ren, Player& player) {
    player.loadHP(ren);

    boss_1.loadTex("graphic/Boss_1.png", ren);
    boss_1.setSrc(0, 0, 160, 128);
    boss_1.setDst(16 * TILE_SIZE, 9 * TILE_SIZE, 320, 256);
    boss_1.attackDamage = BOSS_DAMAGE;
    boss_1.speed = BOSS_SPEED;
    boss_1.HP = BOSS_HP;
    boss_1.max_HP = BOSS_HP;
    boss_1.TopOffSet = 160;

    boss_2.loadTex("graphic/Boss_2.png", ren);
    boss_2.setSrc(0, 0, 140, 93);
    boss_2.setDst(16 * TILE_SIZE, 9 * TILE_SIZE, 280, 186);
    boss_2.attackDamage = BOSS_DAMAGE;
    boss_2.speed = BOSS_SPEED + 100;
    boss_2.HP = BOSS_HP;
    boss_2.max_HP = BOSS_HP;
    boss_2.TopOffSet = 160;
    boss_2.LeftOffSet = 80;
    boss_2.move_right = true;

    boss_3.loadTex("graphic/Boss_3.png", ren);
    boss_3.setSrc(0, 0, 288, 160);
    boss_3.setDst(16 * TILE_SIZE, 9 * TILE_SIZE, 576, 320);
    boss_3.attackDamage = BOSS_DAMAGE;
    boss_3.speed = BOSS_SPEED + 200;
    boss_3.HP = BOSS_HP;
    boss_3.max_HP = BOSS_HP;
    boss_3.TopOffSet = 130;
    boss_3.LeftOffSet = 250;

    Score.loadFont("font.ttf", 30, ren);
    Score.setPosition(1190, 0);

    Time.loadFont("font.ttf", 30, ren);
    Time.setPosition(1280 / 2 - 10, 0);

    Wave.loadFont("font.ttf", 30, ren);
    Wave.setPosition(1280 / 2 - 50, 0);

    UpgradePoints.loadFont("font.ttf", 30, ren);
    UpgradePoints.setPosition(990, 0);

    Upgrade.loadFont("font.ttf", 30, ren);
    Upgrade.setPosition(1280 / 2 - 430, 48);
    Upgrade.setText("Press 1 to increase HP by 2, Press 2 to upgrade attack, Press 3 to upgrade speed", {255, 255, 255, 255}, ren);
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
            rats[i].update(SDL_GetTicks(), player, tiles, tile, wave, 7, 8, 5, 1, 5, 7, 64, 64);
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
            skeletons[i].update(SDL_GetTicks(), player, tiles, tile, wave, 7, 9, 5, 1, 2, 5, 64, 64);
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
            slimes[i].update(SDL_GetTicks(), player, tiles, tile, wave, 11, 12, 12, 2, 0, 1, 64, 64);
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
                    if ((boss.Cloud[i].isDestroyed && boss.Cloud[i].animFrame >= 7) || !boss.Cloud[i].isRendered()) {
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
                boss.Boss_3_Update(SDL_GetTicks(), player, tiles, tile, wave, 11, 14, 14, 1, 2, 4, 288, 160);
                boss.Boss_3_FireAttack(crTime, ren, player);
                for (size_t i = 0; i < boss.Fire.size();) {

                    boss.Fire[i].renderEnemy(ren, player.camera);


                    static Uint32 lastDamageTime = SDL_GetTicks();
                    if (crTime - lastDamageTime >= 500) {
                        if (player.checkCollision(boss.Fire[i].dst)) {
                            if(boss.Fire[i].attack_count != 0) {
                                player.HP--;
                            }
                        }
                        boss.Fire[i].attack_count++;
                        lastDamageTime = crTime;
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
                boss.renderEnemy(ren, player.camera);
                boss.updateHP(ren, player.camera, boss.move_right ? 240 : 280, 130);
                break;
            default:
                break;
        }
        updateScore(ren, boss);
        if (boss.isDestroyed && !isBossKilled) {
            isBossKilled = true;
            switch (currentBoss) {
                case BOSS_1:
                    boss_1.attackDamage += 10;
                    boss_1.HP += 200;
                    killAllSlimes = true;
                    break;
                case BOSS_2:
                    boss_2.attackDamage += 10;
                    boss_2.HP += 200;
                    break;
                case BOSS_3:
                    boss_3.attackDamage += 10;
                    boss_3.HP += 200;
                    break;
                default: break;
            }
        }
    }

    if (killAllSlimes && currentBoss == BOSS_1) {
        for (size_t i = 0; i < slimes.size(); i++) {
            slimes[i].isDied = true;
            slimes[i].animFrame = 0;
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
            if (wave % 5 == 0) {
                skeleton_base_speed += 1;
                rat_base_speed += 1;
            }
            if (wave % 5 == 0) {
                slime_base_speed += 1;
            }
            skeleton_base_HP += 2;
            rat_base_HP += 2;
            slime_base_HP += 2;
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
        if (skeleton_wave_nums > 8) skeleton_wave_nums = 8;
        if (rat_wave_nums > 6) rat_wave_nums = 6;
        if (slime_wave_nums > 6) slime_wave_nums = 6;
        skeleton_left = skeleton_wave_nums;
        rats_left = rat_wave_nums;
        slime_left = slime_wave_nums;
        Time.destroy();
        wave++;
        if (wave == 5 && !mapChange) {
            boss = boss_1;
            boss.HP = boss_1.max_HP;
            boss.isDied = false;
            boss.isAttacking = false;
            boss.animFrame = 0;
            loadMap(ren, "map2.txt");
            isBossWave = true;
            currentBoss = BOSS_1;
            enemySpawn = false;
            mapChange = true;
            //isMap2 = true;
            //isMap1 = false;
        } else if (wave == 10 && !mapChange) {
            boss = boss_2;
            boss.HP = boss_2.max_HP;
            boss.isDied = false;
            boss.isAttacking = false;
            boss.animFrame = 0;
            loadMap(ren, "map.txt");
            isBossWave = true;
            currentBoss = BOSS_2;
            enemySpawn = false;
            mapChange = true;
            //isMap1 = true;
            //isMap2 = false;
        } else if (wave == 15 && !mapChange) {
            boss = boss_3;
            boss.HP = boss_3.max_HP;
            boss.isDied = false;
            boss.isAttacking = false;
            boss.animFrame = 0;
            loadMap(ren, "map3.txt");
            isBossWave = true;
            currentBoss = BOSS_3;
            enemySpawn = false;
            mapChange = true;
            //isMap2 = false;
            //isMap1 = true;
        } else {
            mapChange = false;
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
    Wave.setText(wave_, {255, 255, 255, 255}, ren);
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
