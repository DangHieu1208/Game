#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Tile_map.h"
#include "Enemy.h"
#include <cstdlib>


void Init() {
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "Failed to initiate SDL: " << SDL_GetError() << endl;
    }

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "Failed to create window: " << SDL_GetError() << endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        cout << "Failed to create renderer: " << SDL_GetError() << endl;
        return;
    }

    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
        cout << "Failed to initiate IMG!" << endl;
        return;
    }

    if (TTF_Init() < 0) {
        cout << "Failed to initiate TTF!" << endl;
        return;
    }
}

int frameDelay = 1000/60;


int main(int argc, char* argv[])
{
    Init();

    Player player;
    player.loadTex("graphic/player.png", renderer);
    player.setDst(100, 100, 120, 120);

    Map game_map;
    game_map.loadMap(renderer, "map.txt", player);

    game_map.print();

    Uint32 frameStart = SDL_GetTicks();
    while (running) {
        currentTime = SDL_GetTicks();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            game_map.playerUpgrade(event, player);
            player.handleEvent(event);
        }
        player.update(currentTime);

        SDL_RenderClear(renderer);

        game_map.renderMap(renderer, player,currentTime);
        player.renderPlayer(renderer);
        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
        frameStart = SDL_GetTicks();

    }

    return 0;
}
