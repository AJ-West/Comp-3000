#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>

// SDL includes
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

// ImGui includes
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/backends/imgui_impl_sdl3.h>
#include <dependencies/imgui/backends/imgui_impl_sdlrenderer3.h>

#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/gameManager.h"
#include "gameFiles/misc/dayCycle.h"

using namespace std;

bool dev_mode = false;

//creates the window, renderer and font for the game
void init_SDL_environment();

//creates ImGui
ImGuiIO& init_ImGui_environment();

void handleInput(SDL_Event event, GameManager* manager);

void adjustForZoom(ImGuiIO* io);

void renderImGUI(ImGuiIO& io);