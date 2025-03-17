#include "Message.h"
#include <iostream>

Message::Message() : font(NULL), textTexture(NULL) {
    dst = {0, 0, 0, 0};
}

Message::~Message() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = NULL;
    }
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
}

void Message::loadFont(const char* fontFile, int fontSize, SDL_Renderer* ren) {

    font = TTF_OpenFont(fontFile, fontSize);
    if (!font) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }
}

void Message::setText(const char* text, SDL_Color color, SDL_Renderer* ren) {
    if (!font) {
        std::cout << "Font not loaded!" << std::endl;
        return;
    }

    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = NULL;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) {
        std::cout << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    if (!textTexture) {
        std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }

    dst.w = textSurface->w;
    dst.h = textSurface->h;

    SDL_FreeSurface(textSurface);
}

void Message::setPosition(int x, int y) {
    dst.x = x;
    dst.y = y;
}

void Message::render(SDL_Renderer* ren) {
    if (textTexture) {
        SDL_RenderCopy(ren, textTexture, NULL, &dst);
    }
}
