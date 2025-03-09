#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "Const.h"

struct Entity {
private:
    SDL_Texture* tex;
public:
    Entity() {;}
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* getTex() {return tex;}
    void setSrc(int x, int y, int w, int h);
    void setDst(int x, int y, int w, int h);
    void loadTex(const char* file_name, SDL_Renderer* ren);
    void render(SDL_Renderer* ren, SDL_Rect dst_);
    void renderFlip(SDL_Renderer* ren, SDL_Rect dst_);
    bool isCollided(Entity& A);
    bool checkCollision(SDL_Rect& wall);
    void solveCollision(SDL_Rect& wall);

};

#endif // ENTITY_H_INCLUDED
