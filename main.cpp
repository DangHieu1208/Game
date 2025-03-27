#include "Const.h"
#include "Entity.h"
#include "Player.h"
#include "Tile_map.h"
#include "Enemy.h"
#include "Button.h"
#include "Sound.h"
#include <cstdlib>

enum GameState {menu, playing, quit, pause, instruct, over};
//HIeuDepTRai
void Init() {
    srand(time(0));
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return;
    }
}

void close() {
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
    Title.loadFont("font.ttf", 200, renderer);
    Title.setPosition(80, 0);
    Title.setText("Dungeon Warrior", {255, 255, 255, 255}, renderer);

    Message Instruction;
    Instruction.loadFont("font.ttf", 64, renderer);
    Instruction.setPosition(55, 75);

    Message GameOver;
    GameOver.loadFont("font.ttf", 200, renderer);
    GameOver.setPosition(300, 0);
    GameOver.setText("Game Over", {255, 255, 255, 255}, renderer);

    Message Score;
    Score.loadFont("font.ttf", 100, renderer);
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
    Button exitButton;

    playButton.loadTex("graphic/Play Button.png", renderer);
    resumeButton.loadTex("graphic/Resume Button.png", renderer);
    quitButton.loadTex("graphic/Quit Button.png", renderer);
    startButton.loadTex("graphic/Start Button.png", renderer);
    exitButton.loadTex("graphic/Exit Button.png", renderer);

    playButton.setSrc(0, 0, 600, 200);
    resumeButton.setSrc(0, 0, 600, 200);
    quitButton.setSrc(0, 0, 600, 200);
    startButton.setSrc(0, 0, 600, 200);
    exitButton.setSrc(0, 0, 600, 200);

    startButton.setDst(1280/2 - 150, 720/2 - 75, 300, 100);
    resumeButton.setDst(1280/2 - 150, 720/2 - 75, 300, 100);
    quitButton.setDst(1280/2 - 150, 720/2 + 100, 300, 100);
    playButton.setDst(1280/2 - 150, 720/2 + 200, 300, 100);
    exitButton.setDst(1280/2 - 150, 720/2 - 25, 300, 100);

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
                        playingMusic.stopMusic();
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
                    if (exitButton.isClicked(mouseX, mouseY)) {
                        state = menu;
                        Player player_restart;
                        player_restart.loadTex("graphic/player.png", renderer);
                        player_restart.setDst(100, 100, 120, 120);
                        player = player_restart;
                        skeleton_base_HP = 20;
                        skeleton_base_speed = 4;
                        rat_base_HP = 1;
                        rat_base_speed = 8;
                        game_map = Map();
                        game_map.loadMap(renderer, "map.txt", player);
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
                Score.setText(score, {255, 255, 255, 255}, renderer);
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
            Instruction.multiLinesRender("Move with W, A, S, D.\nAttack with J, defend with K.\nIf you defend for more than 3 consecutive seconds,\nyour character will no longer be able to defend.\nThere will be a 15-second break between each wave.\nPress ESC to pause the game.", {255, 255, 255, 255}, renderer);
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
            exitButton.renderButton(renderer, player.camera);
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
