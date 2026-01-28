#pragma once
#include <iostream>
#include "dependencies/tinyxml2.h"
#include <vector>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "string"
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/entities/Zombies/zombieObject.h"

#include "gameFiles/components/attackComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/nearestComponent.h"
#include "gameFiles/components/pathfindingComponent.h"
#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/resourceTransferComponent.h"

using namespace std;
using namespace tinyxml2;

class MapLoader
{
public:
	MapLoader(const char* filename, SDL_Renderer* renderer);
	~MapLoader();

	void loadTilemap(XMLElement* layer);
	void loadEntities(XMLElement* layer);
	
	void loadUnit(XMLElement* layer);
	void loadConvoy(XMLElement* layer);
	void loadZombie(XMLElement* layer);
	void loadDepot(XMLElement* layer);

	void addUnitComponents(UnitObj* unit, XMLElement* entity);
	void addConvoyComponents(ConvoyObj* convoy, XMLElement* entity);
	void addZombieComponents(ZombieObj* zombie);
	void addDepotComponents(DepotObj* unit, XMLElement* entity);

	void renderTileMap(SDL_Renderer* renderer);

	//getters
	vector<HumanObj*> getUnitConvoyList() { return unitConvoyList; }
	vector<ZombieObj*> getZombieList() { return zombieList; }
	DepotObj* getDepot() { return depot; }

private:
	int width, height;
	int xCells, yCells;
    vector<vector<int>> tilelocs;
    vector<vector<SDL_FRect>> tilemap;

	SDL_Texture* tilemapTexture;

	DepotObj* depot;
	vector<HumanObj*> unitConvoyList;
	vector<ZombieObj*> zombieList;
};