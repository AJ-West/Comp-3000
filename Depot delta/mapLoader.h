#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "string"
#include "unitObject.h"
#include "depotObject.h"
#include "convoyObject.h"
#include "zombieObject.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"
#include "resourceTransferComponent.h"
#include "pathfindingComponent.h"
#include "nearestComponent.h"

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
	void addZombieComponents(ZombieObj* zombie, XMLElement* entity);
	void addDepotComponents(DepotObj* unit, XMLElement* entity);

	vector<SDL_Texture*> loadResourceTextures();

	void renderTileMap(SDL_Renderer* renderer);

	//getters
	vector<UnitObj*> getUnitList() { return unitList; }
	vector<ConvoyObj*> getConvoyList() { return convoyList; }
	vector<ZombieObj*> getZombieList() { return zombieList; }
	DepotObj* getDepot() { return depot; }

private:
	int width, height;
	int xCells, yCells;
    vector<vector<int>> tilelocs;
    vector<vector<SDL_FRect>> tilemap;

	SDL_Texture* tilemapTexture;

	DepotObj* depot;
	vector<UnitObj*> unitList;
	vector<ConvoyObj*> convoyList;
	vector<ZombieObj*> zombieList;

	vector<vector<Tile>> grid;
};