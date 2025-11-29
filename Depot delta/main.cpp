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

	float scaleX = 1;
	float scaleY = 1;

    //Can be used for setting resolution (will be useful when adding settings)
	SDL_SetRenderScale(renderer, zoom, zoom);
    //Used to render consistenly regardless of screensize
	SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth, ResolutionHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    isRunning = true;
}

void clamp_zoom() {
    if (zoom > 2.0f) {
        zoom = 2.0f;
    }
    else if (zoom < 0.5f) {
        zoom = 0.5f;
    }
}

int main()
{
    init_environment();
	LevelManager manager(renderer);

    //dayCycle cycle;

	Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();

        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                //mouse position before zoom
                float mxb = NULL;
                float myb = NULL;
                getScaledMousePos(&mxb, &myb);
                zoom += event.wheel.y * 0.1; // zoom in or out based on mouse wheel scroll
                clamp_zoom();
                SDL_SetRenderScale(renderer, zoom, zoom);
                //mouse position after zoom
                float mxa = NULL;
                float mya = NULL;
                getScaledMousePos(&mxa, &mya);
                camera.dimen.x += mxb - mxa;
                camera.dimen.y += myb - mya;
                camera.dimen.w = ResolutionWidth/zoom;
                camera.dimen.h = ResolutionHeight/zoom;
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
        //cycle.update();
        //cycle.render(renderer);
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