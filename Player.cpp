#include "Player.h"


void Player::loadHP(SDL_Renderer* ren) {
    PlayerHP.loadFont("font.ttf", 30, ren);
    PlayerHP.setPosition(0, 0);
    attack.loadSound("sfx/playerAttack.wav");
    walk.loadSound("sfx/playerWalk.wav");
}

void Player::loadSound() {
    attack.loadSound("sfx/playerAttack.wav");
    walk.loadSound("sfx/playerWalk.wav");
}

void Player::updateHP(SDL_Renderer* ren) {
    if (HP <= 0) {
        return;
    }
    char playerHP[10];
    sprintf(playerHP, "HP:%01d/%01d", HP, max_HP);
    PlayerHP.setText(playerHP, {255, 255, 255, 255}, ren);
    PlayerHP.render(ren);
}

void Player::updateCamera() {
    int mapWidth = 32*80;
    int mapHeight = 18*80;

    camera.x = dst.x - WINDOW_WIDTH/2 + dst.w/2;
    camera.y = dst.y - WINDOW_HEIGHT/2 + dst.h/2;


    if (camera.x < 0) {
        camera.x = 0;
    }
    if (camera.y < 0) {
        camera.y = 0;
    }
    if (camera.x > mapWidth - WINDOW_WIDTH) {
        camera.x = mapWidth - WINDOW_WIDTH;
    }
    if (camera.y > mapHeight - WINDOW_HEIGHT) {
        camera.y = mapHeight - WINDOW_HEIGHT;
    }
}


void Player::handleEvent(SDL_Event& e) {

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_D]) {
        if (!move_right) {
            move_right = true;
            facing_right = true;
            facing_left = false;
            MoveStartTime = SDL_GetTicks();
        }
    } else {
        move_right = false;
    }

    if (keystate[SDL_SCANCODE_A]) {
        if (!move_left) {
            move_left = true;
            facing_left = true;
            facing_right = false;
            MoveStartTime = SDL_GetTicks();
        }
    } else {
        move_left = false;
    }

    if (keystate[SDL_SCANCODE_W]) {
        if (!go_up) {
            go_up = true;
        }
    } else {
        go_up = false;
    }

    if (keystate[SDL_SCANCODE_S]) {
        if (!go_down) {
            go_down = true;
        }
    } else {
        go_down = false;
    }

    if (keystate[SDL_SCANCODE_J]) {
        if (!isAttacking) {
            isAttacking = true;
            AttackStartTime = SDL_GetTicks();
            attack.playSound();
        }
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_k && !defence && !defenceCooldown) {
            defence = true;
            defenceCooldown = true;
            DefenceStartTime = SDL_GetTicks();
            DefenceBeginTime = SDL_GetTicks();
        }
    }

    if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_k) {
            defence = false;
            defenceCooldown = false;
        }
    }
}

void Player::update(Uint32 crTime) {
    static int move_index = 0;
    static int stand_index = 0;
    static int defence_index = 0;

    if (HP <= 0) {
        isDied = true;
    }

    if (isAttacking && !isDied) {
        if (crTime - AttackStartTime >= 50) {
            setSrc(attack_index*56, 56, 56, 56);
            attack_index++;
            AttackStartTime = crTime;
            if (attack_index > 7) {
                isAttacking = false;
                attack_index = 0;
            }
        }
    }

    else if ((move_left || move_right || go_up || go_down) && !isDied) {
        if (crTime - MoveStartTime >= 80) {
            setSrc(move_index*56, 2*56, 56, 56);
            move_index++;
            MoveStartTime = crTime;
            if (move_index > 7) {
                move_index = 0;
            }
        }
        if (crTime - WalkSoundStartTime >= 300) {
            walk.playSound();
            WalkSoundStartTime = crTime;
        }
        if (move_right) dst.x += speed;
        if (move_left) dst.x -= speed;
        if (go_up) dst.y -= speed;
        if (go_down) dst.y += speed;


        updateCamera();
    }

    else if (defence && !isDied) {
        if (crTime - DefenceStartTime >= 80) {
            setSrc(defence_index*56, 10*56, 56, 56);
            defence_index++;
            DefenceStartTime = crTime;
            if (defence_index > 2) {
                defence_index = 2;
            }
        }
        if (crTime - DefenceBeginTime >= 3000) {
            defence = false;
            defence_index = 0;
        }
    }

    else if (isDied) {
        move_left = false;
        move_left = false;
        isAttacking = false;
        defence = false;
        facing_left = false;
        facing_right = false;
        go_down = false;
        go_up = false;
        if (crTime - DieStartTime >= 100) {
            setSrc(die_index*56, 6*56, 56, 56);
            die_index++;
            DieStartTime = crTime;
            if (die_index > 7) {
                die_index = 7;
            }

        }
    }

    else {
        if (crTime - StandStartTime >= 60) {
            setSrc(stand_index*56, 0, 56, 56);
            stand_index++;
            StandStartTime = crTime;
            if (stand_index > 5) {
                stand_index = 0;
            }
        }
        move_index = 0;

        updateCamera();
    }
}

void Player::renderPlayer(SDL_Renderer* ren) {
    SDL_Rect renderQuad = {dst.x - camera.x, dst.y - camera.y, dst.w, dst.h};
    if (move_left || facing_left) {
       renderFlip(ren, renderQuad, camera);
    }
    else {
        render(ren, renderQuad, camera);
    }
}
