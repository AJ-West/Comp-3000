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
#include "resourceComponent.h"

#include "variables.h"

using namespace std;
using namespace tinyxml2;

vector<UnitObj*> unitList;
UnitObj* hoveredUnit = nullptr;

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
	SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth, ResolutionHeight, SDL_LOGICAL_PRESENTATION_OVERSCAN);

    isRunning = true;
}

vector<SDL_Texture*> loadResourceTextures() {
    vector<SDL_Texture*> resourceTextures;
    vector<const char*> resourceFiles = {
        "draftArt/resources/Personnel.png",
        "draftArt/resources/Ammo.png",
        "draftArt/resources/DoS.png",
        "draftArt/resources/Fuel.png",
        "draftArt/resources/Scrap.png"
    };
    for (int i = 0; i < resourceFiles.size(); i++) {
        SDL_Surface* surface = IMG_Load(resourceFiles[i]);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface); // Free the surface after creating the texture
        resourceTextures.push_back(texture);
    }
    return resourceTextures;
}


void make_units() {
	vector<SDL_Texture*> resourceTextures = loadResourceTextures();
    vector<int> max = { 100, 100, 100, 100, 100 };
    vector<int> count = { 50, 50, 50, 50, 50 };
    for (auto& unit : unitList) {
        unit->AddComponent(make_shared<renderComponent>(unit, renderer, "draftArt/basicUnit.png"));
        unit->AddComponent(make_shared<buttonComponent>(unit));
        unit->AddComponent(make_shared<movementComponent>(unit, 50));
        unit->AddComponent(make_shared<resourceComponent>(unit, max, count, resourceTextures));
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
    system("pause");
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
		hoveredUnit = nullptr;
        for (auto& unit : unitList) {
            unit->Update();
            if (unit->getHovering()) {
				hoveredUnit = unit;
            }
        }
		camera.update();
        if (hoveredUnit) {
            SDL_FRect unitRes = { camera.x, camera.y + screenHeight - 100, camera.width, 100 };
            SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Saddle brown for resource bar background
            SDL_RenderFillRect(renderer, &unitRes);
            hoveredUnit->renderHover(renderer);
        }
        
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