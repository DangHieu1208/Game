#include "Player.h"

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
        }
    }
}

void Player::update(Uint32 crTime) {
    static int attack_index = 0;
    static int move_index = 0;
    static int stand_index = 0;

    if (isAttacking) {
        if (crTime - AttackStartTime >= 50) {
            setSrc(attack_index*56, 1*56+22, 56-4, 56-22);
            attack_index++;
            AttackStartTime = crTime;
            if (attack_index > 7) {
                isAttacking = false;
                attack_index = 0;
            }
        }
    }

    else if (move_left || move_right || go_up || go_down) {
        if (crTime - MoveStartTime >= 80) {
            setSrc(move_index*56, 2*56+22, 56-4, 56-22);
            move_index++;
            MoveStartTime = crTime;
            if (move_index > 7) {
                move_index = 0;
            }
        }
        if (move_right) dst.x += 5;
        if (move_left) dst.x -= 5;
        if (go_up) dst.y -= 5;
        if (go_down) dst.y += 5;

        updateCamera();
    }

    else {
        if (crTime - StandStartTime >= 60) {
            setSrc(stand_index*56, 0+22, 56-4, 56-22);
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
