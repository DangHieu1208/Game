#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
using namespace std;

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Event event;

static int mouseX, mouseY;
static Uint32 currentTime;
static bool running = true;
static bool isPaused = false;
static int ENEMY_NUMS_WAVE_1 = 4;
static int ENEMY_ADD_EACH_WAVE = ENEMY_NUMS_WAVE_1/2;
static int enemyDamage = 1;
static int enemy_base_HP = 20;
static int enemy_base_speed = 4;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#endif // CONST_H_INCLUDED
