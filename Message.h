#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include "Const.h"
#include <SDL_ttf.h>
#include <string>
#include <sstream>

struct Message {
private:
    TTF_Font* font;
    SDL_Texture* textTexture;
    const char* text;
public:
    SDL_Rect dst;
    Message();
    ~Message();
    void loadFont(const char* fontFile, int fontSize, SDL_Renderer* ren);
    void setText(const char* text, SDL_Color color, SDL_Renderer* ren);
    void setPosition(int x, int y);
    void render(SDL_Renderer* ren);
    void destroy(){if (textTexture) SDL_DestroyTexture(textTexture);}
    void multiLinesRender(const char* test, SDL_Color color, SDL_Renderer* ren);
};

#endif
