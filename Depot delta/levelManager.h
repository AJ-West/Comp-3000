#pragma once

#include <algorithm>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "tinyxml2.h"
#include "mapLoader.h"
#include "mapSaver.h"
#include "selectedHandler.h"

#include "dayCycle.h"
#include "transferBox.h"

#include "unitObject.h"
#include "depotObject.h"
#include "convoyObject.h"
#include "zombieObject.h"

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
	void saveOnExit();

	void handleInput(SDL_Event event);
	void render();

	//Handle game zoom
	void zoomChange(SDL_Event event);
	void clampZoom();

private:
	SDL_Renderer* renderer;

	levelUI* UI;

	MapLoader* mapLoader;
	vector<GameObject*> unitConvoys;
	vector<UnitObj*> unitList;
	vector<ConvoyObj*> convoyList;
	vector<ZombieObj*> zombieList;
	DepotObj* depot;
	GameObject* hoveredUnit = nullptr;

	SelectedHandler* selector;

	dayCycle* time;

	transferBox* box;

	bool textInput = false;
};

