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
#include "gameFiles/entities/Buildings/building.h"
#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/entities/Zombies/zombieObject.h"

#include "gameFiles/misc/dayCycle.h"

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

	void readTechFile();
	void loadDepotUpgrades(XMLElement* layer);
	void loadUnitUpgrades(XMLElement* layer);
	void loadConvoyUpgrades(XMLElement* layer);

	void loadTilemap(XMLElement* layer);
	void loadTime(XMLElement* layer);
	void loadEntities(XMLElement* layer);

	void loadSwarms(XMLElement* layer);
	
	void loadUnit(XMLElement* layer);
	void loadConvoy(XMLElement* layer);
	void loadZombie(XMLElement* layer);
	void loadDepot(XMLElement* layer);
	void loadBuilding(XMLElement* layer);

	void loadAllTransfer(XMLElement* layer);
	void loadTransfer(XMLElement* entity);

	void addUnitComponents(UnitObj* unit, XMLElement* entity);
	void addConvoyComponents(ConvoyObj* convoy, XMLElement* entity);
	void addZombieComponents(ZombieObj* zombie);
	void addDepotComponents(DepotObj* unit, XMLElement* entity);
	void addBuildingComponents(BuildingObj* building, XMLElement* entity);

	void loadTechTree();
	void loadDepotTech(XMLElement* layer);
	void loadUnitTech(XMLElement* layer);
	void loadConvoyTech(XMLElement* layer);

	void renderTileMap(SDL_Renderer* renderer);

	//getters
	shared_ptr<vector<shared_ptr<HumanObj>>> getUnitConvoyList() { return unitConvoyList; }
	shared_ptr<vector<shared_ptr<BuildingObj>>> getBuildingList() { return buildingList; }
	shared_ptr<vector<shared_ptr<ZombieObj>>> getZombieList() { return zombieList; }
	DepotObj* getDepot() { return depot; }
	vector<vector<int>> getSwarmTimes() { return swarmTimes; }
	vector<int> getSwarmQuantity() { return swarmQuantity; }
	vector<int> getSwarmDirection() { return swarmDirection; }
	vector<int> getTime() { return time; }


private:
    vector<vector<int>> tilelocs;
    vector<vector<SDL_FRect>> tilemap;

	SDL_Texture* tilemapTexture;

	DepotObj* depot;
	shared_ptr<vector<shared_ptr<HumanObj>>> unitConvoyList = make_shared<vector<shared_ptr<HumanObj>>>();
	shared_ptr<vector<shared_ptr<BuildingObj>>> buildingList = make_shared<vector<shared_ptr<BuildingObj>>>();
	shared_ptr<vector<shared_ptr<ZombieObj>>> zombieList = make_shared<vector<shared_ptr<ZombieObj>>>();

	vector<vector<int>> swarmTimes{};
	vector<int> swarmQuantity{};
	vector<int> swarmDirection{};

	vector<int> time{ 0,0,0,0 };
};