#pragma once

#include <algorithm>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "dependencies/tinyxml2.h"

#include "gameFiles/fileHandling/mapLoader.h"
#include "gameFiles/fileHandling/mapSaver.h"

#include "gameFiles/levelHandling/selectedHandler.h"

#include "gameFiles/misc/dayCycle.h"

#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/entities/Zombies/zombieObject.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/resourceComponent.h"

#include "gameFiles/useThroughout/variables.h"

class levelUI;
class ZombieSpawner;

using namespace std;
using namespace tinyxml2;

class LevelManager {
public:
	LevelManager (SDL_Renderer* SDL_Renderer);
	~LevelManager ();
	void saveOnExit();

	void handleInput(SDL_Event event);

	void render();
	void unpausedRender();
	void pausedRender();

	void addUnitConvoy(GameObject* unitConvoy);
	void addZombie(ZombieObj* zombie);

	//Handle game zoom
	void zoomChange(SDL_Event event);
	void clampZoom();

	//getters
	int getUnitConvoysSize() { return unitConvoys.size(); }
	int getZombiesSize() { return zombieList.size(); }
	int getNextID() { return unitConvoys.size() + zombieList.size() + 2; } // add 1 for depot and new object for total number of objects
	DepotObj* getDepot() { return depot; } 
	vector<GameObject*> getUnitConvoys() { return unitConvoys; }

	//setters
	void setPaused(bool pause) { paused = pause; }

private:
	SDL_Renderer* renderer;

	levelUI* UI;
	ZombieSpawner* spawner;

	MapLoader* mapLoader;
	vector<GameObject*> unitConvoys;
	vector<ZombieObj*> zombieList;
	DepotObj* depot;
	GameObject* hoveredUnit = nullptr;

	SelectedHandler* selector;

	dayCycle* time;

	bool textInput = false;

	bool paused = false;
};

