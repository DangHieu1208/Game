#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "Const.h"
#include <cmath>

struct Entity {
private:
    SDL_Texture* tex;
public:
    bool isDestroyed = false;
    int LeftOffSet = 4, TopOffSet = 22;
    SDL_Rect hitbox = {dst.x + LeftOffSet, dst.y + TopOffSet, dst.w - abs(LeftOffSet), dst.h - TopOffSet};
    Entity() {;}
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* getTex() {return tex;}
    void setSrc(int x, int y, int w, int h);
    void setDst(int x, int y, int w, int h);
    void loadTex(const char* file_name, SDL_Renderer* ren);
    void render(SDL_Renderer* ren, SDL_Rect dst_, SDL_Rect camera);
    bool isRendered();
    void renderFlip(SDL_Renderer* ren, SDL_Rect dst_, SDL_Rect camera);
    bool isCollided(Entity& A);
    bool checkCollision(SDL_Rect& wall);
    void solveCollision(SDL_Rect& wall);
    void destroy() {if (tex) SDL_DestroyTexture(tex); tex = NULL; isDestroyed = true;}

};

#endif // ENTITY_H_INCLUDED
