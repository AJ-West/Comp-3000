#pragma once

#include  <SDL3/SDL.h>

#ifndef VARIABLES_H
#define VARIABLES_H

extern int screenWidth;
extern int screenHeight;
extern int ResolutionWidth;
extern int ResolutionHeight;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern bool isRunning;

#endif // !VARIABLES_H
