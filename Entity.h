#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "Const.h"
#include <cmath>

struct Entity {
private:
    SDL_Texture* tex;
public:
    SDL_Rect hitbox = {dst.x + LeftOffSet, dst.y + TopOffSet, dst.w - abs(LeftOffSet), dst.h - TopOffSet};
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* getTex() {return tex;}
    bool isDestroyed = false;
    int LeftOffSet = 4, TopOffSet = 22;
    void setSrc(int x, int y, int w, int h);
    void setDst(int x, int y, int w, int h);
    void loadTex(const char* file_name, SDL_Renderer* ren);
    void render(SDL_Renderer* ren, SDL_Rect dst_, SDL_Rect camera);
    bool isRendered();
    void renderFlip(SDL_Renderer* ren, SDL_Rect dst_, SDL_Rect camera);
    bool checkCollision(SDL_Rect& wall);
    void solveCollision(SDL_Rect& wall);
    void setTexture(SDL_Texture* tex_);
    void destroy() {if (tex) tex = NULL; isDestroyed = true;}

};

#endif // ENTITY_H_INCLUDED
