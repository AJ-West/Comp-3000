#include "variables.h"

// Frame rate handling
const int FPS = 60;
const int frameDelay = 1000/ FPS;
float deltaTime = 0;

// Screen
int screenWidth = 1920/2;
int screenHeight = 1080/2;
int ResolutionWidth = 1920/2;
int ResolutionHeight = 1080/2;

int worldWidth = 1984;
int worldHeight = 1088;

// SDL
SDL_Window* window;
SDL_Renderer* renderer;

bool isRunning;

Camera camera = { 0,0, screenWidth, screenHeight };
