#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#include <iostream>
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

static int mouseX, mouseY;
static Uint32 currentTime;
static bool running = true;
static bool isPaused = false;

static int skeleton_base_HP = 20;
static int skeleton_base_speed = 4;
static int rat_base_HP = 1;
static int rat_base_speed = 7;
static int slime_base_HP = 30;
static int slime_base_speed = 2;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define ENEMY_DAMAGE 1
#define SKELETON_DAMAGE 2
#define RAT_DAMAGE 1
#define SLIME_DAMAGE 4
#define BOSS_DAMAGE 10
#define INTERVAL_TIME 3

#endif // CONST_H_INCLUDED
