#include "Entity.h"
#include <cmath>

bool Entity::isRendered() {
    if (tex) {
        return true;
    }
    return false;
}
void Entity::setSrc(int x, int y, int w, int h) {
    src.x = x;
    src.y = y;
    src.w = w;
    src.h = h;
}

void Entity::setDst(int x, int y, int w, int h) {
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
}

void Entity::loadTex(const char* file_name, SDL_Renderer* ren) {
    SDL_Surface* load_surface = IMG_Load(file_name);
    if (load_surface == NULL) {
        cout << "Failed to load surface: " << SDL_GetError() << endl;
        return;
    }

    tex = SDL_CreateTextureFromSurface(ren, load_surface);
    if (tex == NULL) {
        cout << "Failed to load texture: " << SDL_GetError() << endl;
        return;
    }

    SDL_FreeSurface(load_surface);
}

void Entity::render(SDL_Renderer* ren, SDL_Rect dst_, SDL_Rect camera) {
    if (tex) {
        SDL_RenderCopy(ren, tex, &src, &dst_);
    }

    if (dst.x + dst.w < camera.x || dst.x > camera.x || dst.y + dst.h < camera.y || dst.y > camera.y) return;
}

void Entity::renderFlip(SDL_Renderer* ren, SDL_Rect dst_, SDL_Rect camera) {
    if (tex) {
        SDL_RenderCopyEx(ren, tex, &src, &dst_, 180, NULL, SDL_FLIP_VERTICAL);
    }

    if (dst.x + dst.w < camera.x || dst.x > camera.x || dst.y + dst.h < camera.y || dst.y > camera.y) return;
}

bool Entity::isCollided(Entity& A) {
    if (SDL_HasIntersection(&dst, &A.dst)) {
        return true;
    }
    return false;
}


bool Entity::checkCollision(SDL_Rect& wall) {
    SDL_Rect entityRect = dst;
    SDL_Rect wallRect = wall;

    SDL_Rect hitbox = {
        entityRect.x + LeftOffSet,
        entityRect.y + TopOffSet,
        entityRect.w - abs(LeftOffSet),
        entityRect.h - TopOffSet
    };

    return (hitbox.x < wallRect.x + wallRect.w &&
            hitbox.x + hitbox.w > wallRect.x &&
            hitbox.y < wallRect.y + wallRect.h &&
            hitbox.y + hitbox.h > wallRect.y);
}

void Entity::solveCollision(SDL_Rect& wall) {
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

    if (minOverlap == leftOverlap && leftOverlap > 0) {
        dst.x -= minOverlap;
    }
    else if (minOverlap == rightOverlap && rightOverlap > 0) {
        dst.x += minOverlap;
    }
    else if (minOverlap == topOverlap && topOverlap > 0) {
        dst.y -= minOverlap;
    }
    else if (minOverlap == bottomOverlap && bottomOverlap > 0) {
        dst.y += minOverlap;
    }
}
