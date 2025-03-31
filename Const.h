#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#include <iostream>
#include "Message.h"
#include "Sound.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>
using namespace std;

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Event event;

static SDL_Color white = {255, 255, 255, 255};
static SDL_Color red = {255, 0, 0, 255};

static int mouseX, mouseY;
static Uint32 currentTime;
static bool running = true;
static bool isPaused = false;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define TILE_SIZE 80
#define MAP_WIDTH 32
#define MAP_HEIGHT 18
#define MAP_TILES_NUMS 7

#define PLAYER_TILE_SIZE 56

#define ENEMY_DAMAGE 1
#define SKELETON_DAMAGE 2
#define RAT_DAMAGE 1
#define SLIME_DAMAGE 4
#define CHASE_RANGE 1470

#define BOSS_DAMAGE 8
#define BOSS_HP 250
#define BOSS_SPEED 9

#define INTERVAL_TIME 11

#endif // CONST_H_INCLUDED
