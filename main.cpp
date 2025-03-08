#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Tile_map.h"


void Init() {
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
}

int frameDelay = 1000/60;


int main(int argc, char* argv[])
{
    Init();

    Player player;
    player.loadTex("graphic/player.png", renderer);
    player.setDst(0, 0, 160, 160);

    Map game_map;
    game_map.loadMap(renderer, "map.txt");

    game_map.print();

    Uint32 frameStart = SDL_GetTicks();
    while (running) {
        currentTime = SDL_GetTicks();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            player.handleEvent(event);
        }
        player.update(currentTime);
        SDL_RenderClear(renderer);

        game_map.createMap(renderer, player);
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

