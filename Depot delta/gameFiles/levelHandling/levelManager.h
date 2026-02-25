#pragma once

#include <algorithm>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include <thread>
#include <chrono>

#include "dependencies/tinyxml2.h"

#include "gameFiles/fileHandling/mapLoader.h"
#include "gameFiles/fileHandling/mapSaver.h"

#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

#include "gameFiles/misc/dayCycle.h"

#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/entities/Zombies/zombieObject.h"
#include "gameFiles/entities/Buildings/building.h"

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

	void addUnitConvoy(HumanObj* unitConvoy);
	void addZombie(ZombieObj* zombie);

	//Handle game zoom
	void zoomChange(SDL_Event event);
	void clampZoom();

	void updateStats(string keyName, bool forUnit);

	void spawnSwarm(int num, int direction);
	void spawnZombie();

	void removeDeadFromLists();

	//getters
	int getUnitConvoysSize() { return unitConvoys->size(); }
	shared_ptr<vector<shared_ptr<HumanObj>>> getUnitConvoys() { return unitConvoys; }
	shared_ptr<vector<shared_ptr<GameObject>>> getAllObjects() { return allObjects; }
	shared_ptr<vector<shared_ptr<ZombieObj>>> getZombieList() { return zombieList; }
	int getZombiesSize() { return zombieList->size(); }
	int getNextID() { return unitConvoys->size() + zombieList->size() + 2; } // add 1 for depot and new object for total number of objects
	shared_ptr<DepotObj> getDepot() { return depot; }
	bool getDepotAlive() { return depotAlive; }

	//setters
	void setPaused(bool pause) { paused = pause; }

private:
	SDL_Renderer* renderer;

	levelUI* UI;
	ZombieSpawner* spawner;

	MapLoader* mapLoader;
	shared_ptr<vector<shared_ptr<HumanObj>>> unitConvoys;
	shared_ptr<vector<shared_ptr<BuildingObj>>> buildingList;
	shared_ptr<vector<shared_ptr<ZombieObj>>> zombieList;
	shared_ptr<DepotObj> depot;
	shared_ptr<GameObject> hoveredUnit = nullptr;
	shared_ptr<BuildingObj> building;
	shared_ptr<vector<shared_ptr<GameObject>>> allObjects = make_shared<vector<shared_ptr<GameObject>>>();

	HandleSelected* handler;

	dayCycle* time;

	bool spawningSwarm = false;
	int swarmLeft = 0;
	int swarmDirection = 0;
	Uint32 lastSpawnTime = 0;
	Uint32 spawnDelay = 500;
	Vec2 swarmPos = { 0,0 };

	bool textInput = false;

	bool paused = false;

	bool depotAlive = true;

	int frameCounter = 0;
};

