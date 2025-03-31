#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Tile_map.h"
#include "Enemy.h"
#include "Button.h"
#include "Sound.h"
#include <cstdlib>


enum GameState {menu, playing, quit, pause, instruct, over};


void Init() {
    srand(time(0));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Failed to initiate SDL: " << SDL_GetError() << endl;
    }

    window = SDL_CreateWindow("Dungeon Warrior", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return;
    }
}


void close() {
    TTF_Quit();
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int frameDelay = 1000/60;


int main(int argc, char* argv[])
{
    Init();

    Sound playingMusic;
    playingMusic.loadMusic("sfx/playing.mp3");
    playingMusic.playMusic(-1);

    Message Title;
    Title.loadFont("font/font.ttf", 200, renderer);
    Title.setPosition(80, 0);
    Title.setText("Dungeon Warrior", white, renderer);


    Message Instruction;
    Instruction.loadFont("font/font.ttf", 64, renderer);
    Instruction.setPosition(55, 75);


    Message GameOver;
    GameOver.loadFont("font/font.ttf", 200, renderer);
    GameOver.setPosition(300, 0);
    GameOver.setText("Game Over", white, renderer);


    Message Score;
    Score.loadFont("font/font.ttf", 100, renderer);
    Score.setPosition(480, 200);


    Entity background;
    background.loadTex("graphic/Background.png", renderer);
    background.setSrc(0, 0, 1920, 1080);
    background.setDst(0, 0, 1280, 720);

    GameState state = menu;

    Button playButton;
    Button quitButton;
    Button resumeButton;
    Button startButton;
    Button newGameButton;


    playButton.loadTex("graphic/button/Play Button.png", renderer);
    resumeButton.loadTex("graphic/button/Resume Button.png", renderer);
    quitButton.loadTex("graphic/button/Quit Button.png", renderer);
    startButton.loadTex("graphic/button/Start Button.png", renderer);
    newGameButton.loadTex("graphic/button/New Game Button.png", renderer);


    playButton.setSrc(0, 0, 600, 200);
    resumeButton.setSrc(0, 0, 600, 200);
    quitButton.setSrc(0, 0, 600, 200);
    startButton.setSrc(0, 0, 600, 200);
    newGameButton.setSrc(0, 0, 600, 200);

    startButton.setDst(1280/2 - 150, 720/2 - 75, 300, 100);
    resumeButton.setDst(1280/2 - 150, 720/2 - 75, 300, 100);
    quitButton.setDst(1280/2 - 150, 720/2 + 100, 300, 100);
    playButton.setDst(1280/2 - 150, 720/2 + 200, 300, 100);
    newGameButton.setDst(1280/2 - 150, 720/2 - 25, 300, 100);


    Player player;
    player.loadTex("graphic/player/player.png", renderer);
    player.loadSound();
    player.setDst(15 * 80, 8 * 80, 120, 120);


    Map game_map;
    game_map.loadMap(renderer, "map/map1.txt");
    game_map.loadEntities(renderer, player);
    game_map.loadTexture(renderer);
    game_map.loadEnemyTexture(renderer);

    Uint32 frameStart = SDL_GetTicks();
    while (running) {
        currentTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                state = quit;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && state == playing) {
                isPaused = true;
                state = pause;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (state == menu) {
                    if (startButton.isClicked(mouseX, mouseY)) {
                        state = instruct;
                    }
                    if (quitButton.isClicked(mouseX, mouseY)) {
                        state = quit;
                    }
                }

                else if (state == pause) {
                    if (resumeButton.isClicked(mouseX, mouseY)) {
                        state = playing;
                        isPaused = false;
                    }
                    if (quitButton.isClicked(mouseX, mouseY)) {
                        state = quit;
                    }
                }

                else if (state == instruct) {
                    if (playButton.isClicked(mouseX, mouseY)) {
                        state = playing;
                    }
                }

                else if (state == over) {
                    if (newGameButton.isClicked(mouseX, mouseY)) {
                        state = menu;
                        Player player_restart;
                        player_restart.loadTex("graphic/player/player.png", renderer);
                        player_restart.setDst(100, 100, 120, 120);
                        player = player_restart;
                        Map new_game_map;
                        new_game_map.loadMap(renderer, "map/map1.txt");
                        new_game_map.loadEntities(renderer, player);
                        new_game_map.loadTexture(renderer);
                        game_map = new_game_map;
                    }
                    if (quitButton.isClicked(mouseX, mouseY)) {
                        state = quit;
                    }
                }
            }
            if (state == playing) {
                game_map.playerUpgrade(event, player);
                player.handleEvent(event);
            }
        }
        if (state == playing && player.isDied && player.die_index == 7) {
                SDL_Delay(1000);
                char score[10];
                sprintf(score, "SCORE: %02d", game_map.score_);
                Score.setText(score, white, renderer);
                state = over;
        }

        SDL_RenderClear(renderer);

        switch (state) {
        case menu:
            background.render(renderer, background.dst, player.camera);
            startButton.renderButton(renderer, player.camera);
            quitButton.renderButton(renderer, player.camera);
            Title.render(renderer);
            break;
        case instruct:
            background.render(renderer, background.dst, player.camera);
            playButton.renderButton(renderer, player.camera);
            Instruction.multiLinesRender("Move with W, A, S, D.\nAttack with J, defend with K.\nIf you defend for more than 2 consecutive seconds,\nyour character will no longer be able to defend.\nThere will be a 15-second break between each wave.\nPress ESC to pause the game.", {255, 255, 255, 255}, renderer);
            break;
        case playing:
            player.update(currentTime);
            game_map.renderMap(renderer, player, currentTime);
            player.renderPlayer(renderer);
            break;
        case pause:
            background.render(renderer, background.dst, player.camera);
            resumeButton.renderButton(renderer, player.camera);
            quitButton.renderButton(renderer, player.camera);
            Title.render(renderer);
            break;
        case over:
            background.render(renderer, background.dst, player.camera);
            newGameButton.renderButton(renderer, player.camera);
            quitButton.renderButton(renderer, player.camera);
            GameOver.render(renderer);
            Score.render(renderer);
            break;
        case quit:
            running = false;
            break;
        }

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
        frameStart = SDL_GetTicks();
    }

    close();

    return 0;
}
