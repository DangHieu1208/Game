#include "Message.h"
#include <iostream>
#include <vector>
using namespace std;

void Message::loadFont(const char* fontFile, int fontSize, SDL_Renderer* ren) {
    font = TTF_OpenFont(fontFile, fontSize);
}

void Message::setText(const char* text_, SDL_Color color, SDL_Renderer* ren) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text_, color);

    textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    dst.w = textSurface->w;
    dst.h = textSurface->h;
    SDL_FreeSurface(textSurface);
}

void Message::setPosition(int x, int y) {
    dst.x = x;
    dst.y = y;
}

void Message::render(SDL_Renderer* ren) {
    SDL_RenderCopy(ren, textTexture, NULL, &dst);
}

void Message::multiLinesRender(const char* text, SDL_Color color, SDL_Renderer* ren) {
    if (!font) {
        cout << "Font not loaded for multiLinesRender!" << endl;
        return;
    }

    vector<string> lines;
    stringstream ss(text);
    string line;

    while (getline(ss, line, '\n')) {
        lines.push_back(line);
    }

    int Height = TTF_FontHeight(font);

    for (size_t i = 0; i < lines.size(); i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, lines[i].c_str(), color);
        if (!surface) {
            cout << "Failed to render line " << i << ": " << TTF_GetError() << endl;
            continue;
        }

        SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(ren, surface);
        if (!tempTexture) {
            cout << "Failed to create texture for line " << i << ": " << SDL_GetError() << endl;
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect tempDst = {dst.x, dst.y + (int)i * Height, surface->w, surface->h};
        SDL_RenderCopy(ren, tempTexture, NULL, &tempDst);

        SDL_DestroyTexture(tempTexture);
        SDL_FreeSurface(surface);
    }
}


