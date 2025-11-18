// Depot delta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "tinyxml2.h"
#include "mapLoader.h"

#include "unitObject.h"
#include "depotObject.h"
#include "convoyObject.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"

#include "variables.h"

#include "levelManager.h"

#include "dayCycle.h"

using namespace std;
using namespace tinyxml2;

vector<UnitObj*> unitList;
vector<ConvoyObj*> convoyList;
DepotObj* depot;
GameObject* hoveredUnit = nullptr;

//creates the window, renderer and font for the game
void init_environment() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    window = SDL_CreateWindow("Depot delta", screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        cerr << "Window cannot be created: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        cerr << "Renderer cannot be created: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    font = TTF_OpenFont("art/BorderWall.ttf", FONT_SIZE);
    if (font == nullptr) {
        cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }

	float scaleX = static_cast<float>(screenWidth) / ResolutionWidth;
	float scaleY = static_cast<float>(screenHeight) / ResolutionHeight;

    //Can be used for setting resolution (will be useful when adding settings)
	//SDL_SetRenderScale(renderer, scaleX, scaleY);
    //Used to render consistenly regardless of screensize
	SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth, ResolutionHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    isRunning = true;
}

int main()
{
    system("pause");
    init_environment();
	LevelManager manager(renderer);

    dayCycle cycle;

	Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();

        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else {
				manager.handleInput(event);
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

		SDL_RenderClear(renderer);
        manager.render();
        cycle.update();
        cycle.render(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

        // Frame rate capping
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

	manager.exit();

    return 0;
}