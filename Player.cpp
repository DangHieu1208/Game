#include "Player.h"


void Player::loadHP(SDL_Renderer* ren) {
    PlayerHP.loadFont("font.ttf", 30, ren);
    PlayerHP.setPosition(0, 0);
}

void Player::updateHP(SDL_Renderer* ren) {
    char playerHP[10];
    sprintf(playerHP, "HP:%01d/10", HP);
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

/*void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_d:
            if (!move_right) {
                move_right = true;
                facing_right = true;
                facing_left = false;
                MoveStartTime = SDL_GetTicks();
            }
            break;
        case SDLK_a:
            if (!move_left) {
                move_left = true;
                facing_left = true;
                facing_right = false;
                MoveStartTime = SDL_GetTicks();
            }
            break;
        case SDLK_j:
            if (!isAttacking) {
                isAttacking = true;
                AttackStartTime = SDL_GetTicks();
            }
            break;
        case SDLK_w:
            if (!go_up) {
                go_up = true;
            }
            break;
        case SDLK_s:
            if (!go_down) {
                go_down = true;
            }
            break;
        case SDLK_k:
            if (!defence) {
                defence = true;
            }
            DefenceStartTime = SDL_GetTicks();
            break;
        }
    }
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        case SDLK_d:
            move_right = false;
            break;
        case SDLK_a:
            move_left = false;
            break;
        case SDLK_w:
            go_up = false;
            break;
        case SDLK_s:
            go_down = false;
            break;
        case SDLK_k:
            defence = false;
            break;
        }
    }
}*/

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

    if (isAttacking) {
        if (crTime - AttackStartTime >= 50) {
            setSrc(attack_index*56, 56, 56, 56);
            attack_index++;
            AttackStartTime = crTime;
            if (attack_index > 5) {
                isAttacking = false;
                attack_index = 0;
            }
        }
    }

    else if (move_left || move_right || go_up || go_down) {
        if (crTime - MoveStartTime >= 80) {
            setSrc(move_index*56, 2*56, 56, 56);
            move_index++;
            MoveStartTime = crTime;
            if (move_index > 5) {
                move_index = 0;
            }
        }
        if (move_right) dst.x += 6;
        if (move_left) dst.x -= 6;
        if (go_up) dst.y -= 6;
        if (go_down) dst.y += 6;

        updateCamera();
    }

    else if (defence) {
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
       renderFlip(ren, renderQuad);
    }
    else {
        render(ren, renderQuad);
    }
}
