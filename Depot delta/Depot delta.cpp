// Depot delta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "tinyxml2.h"
#include "mapLoader.h"

#include "unitObject.h"
#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"

#include "variables.h"

using namespace std;
using namespace tinyxml2;

vector<UnitObj*> unitList;

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

	float scaleX = static_cast<float>(screenWidth) / ResolutionWidth;
	float scaleY = static_cast<float>(screenHeight) / ResolutionHeight;

    //Can be used for setting resolution (will be useful when adding settings)
	//SDL_SetRenderScale(renderer, scaleX, scaleY);
    //Used to render consistenly regardless of screensize
	SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth, ResolutionHeight, SDL_LOGICAL_PRESENTATION_OVERSCAN);

    isRunning = true;
}

void make_units() {
    for (auto& unit : unitList) {
        unit->AddComponent(make_shared<renderComponent>(unit, renderer, "draftArt/basicUnit.png"));
        unit->AddComponent(make_shared<buttonComponent>(unit));
        unit->AddComponent(make_shared<movementComponent>(unit, 50));
    }
}

void checkUnitHover(SDL_Event event) {
	for (auto& unit : unitList) {
        unit->checkHover(event.motion.x, event.motion.y);
	}
}

void checkClick() {
	UnitObj* selectedUnit = nullptr;
    //see if there is a currently selected unit
    for (auto& unit : unitList) {
        if (unit->getSelected()) {
			selectedUnit = unit;
            break;
        }
    }

	bool selectedSomething = false;

	//see if a unit has been clicked
    for (auto& unit : unitList) {
        if (unit->getHovering() && !unit->getSelected()) {
            unit->onClick();
            if (selectedUnit) {
                selectedUnit->onClick();
            }
            selectedUnit = unit;
            selectedSomething = true;
            break;
        }
        else if (unit->getHovering() && unit->getSelected()) {
			selectedUnit = nullptr;
			unit->onClick();
			selectedSomething = true;
			break;

        }
    }
    
    if (!selectedSomething && selectedUnit) {
        selectedUnit->clickAway();
    }
}


int main()
{
    //system("pause");
    init_environment();
    MapLoader manager("maps/test.xml", renderer);
	unitList = manager.getUnitList();
    make_units();

	Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();

        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_MOUSE_MOTION) {
				checkUnitHover(event);
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
					checkClick();
                }
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                camera.keyDown(event.key.key);
            }
            else if (event.type == SDL_EVENT_KEY_UP) {
                camera.keyUp(event.key.key);
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

		SDL_RenderClear(renderer);
		manager.renderTileMap(renderer);
        for (auto& unit : unitList) {
            unit->Update();
        }
		camera.update();
		SDL_RenderPresent(renderer);

        // Frame rate capping
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

	manager.saveFile("maps/test.xml", unitList);

    return 0;
}