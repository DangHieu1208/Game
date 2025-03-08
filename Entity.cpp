#include "Entity.h"

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

void Entity::render(SDL_Renderer* ren, SDL_Rect dst_) {
    if (tex) {
        SDL_RenderCopy(ren, tex, &src, &dst_);
    }
}

void Entity::renderFlip(SDL_Renderer* ren, SDL_Rect dst_) {
    if (tex) {
        SDL_RenderCopyEx(ren, tex, &src, &dst_, 180, NULL, SDL_FLIP_VERTICAL);
    }
}

bool Entity::isCollided(Entity& A) {
    if (SDL_HasIntersection(&dst, &A.dst)) {
        return true;
    }
    return false;
}

