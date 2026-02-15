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

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/resourceComponent.h"

using namespace std;
using namespace tinyxml2;

class MapSaver
{
public:
	MapSaver(const char* filename);
	~MapSaver();

	shared_ptr<GameObject> findObject(shared_ptr<vector<shared_ptr<GameObject>>> allObjects, int id);

	void saveUnit(XMLElement* entity, shared_ptr<GameObject> units);
	void saveNewUnit(XMLElement* layer, shared_ptr<GameObject> unit);

	bool saveZombie(XMLElement* entity, vector< shared_ptr<ZombieObj>> zombie);
	void saveDepot(XMLElement* entity, shared_ptr<GameObject> depot);
	void saveBuilding(XMLElement* entity, shared_ptr<GameObject> building);

	void saveFile(shared_ptr<vector<shared_ptr<GameObject>>> allObjects, vector<shared_ptr<ZombieObj>> zombies);

	void saveHealth(XMLElement* entity, shared_ptr < GameObject> obj);
	void saveResources(XMLElement* entity, shared_ptr < GameObject> obj);
	void saveResourceTransfer(XMLElement* entity, shared_ptr < GameObject> obj);
	bool saveTransfering(XMLElement* entity, shared_ptr < GameObject> target, bool transfering);
	void saveMovement(XMLElement* entity, shared_ptr < GameObject> obj);

private:
	XMLDocument doc;
	const char* filename;
};