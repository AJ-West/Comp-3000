#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "tinyxml2.h"
#include "mapLoader.h"
#include "mapSaver.h"

#include "unitObject.h"
#include "depotObject.h"
#include "convoyObject.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"

#include "variables.h"

using namespace std;
using namespace tinyxml2;

class LevelManager {
public:
	LevelManager (SDL_Renderer* SDL_Renderer);
	~LevelManager ();
	void exit();

	void handleInput(SDL_Event event);
	void render();

	void checkHover(SDL_Event event);
	void checkClick();

private:
	SDL_Renderer* renderer;

	MapLoader* mapLoader;
	vector<UnitObj*> unitList;
	vector<ConvoyObj*> convoyList;
	DepotObj* depot;
	GameObject* hoveredUnit = nullptr;
};

