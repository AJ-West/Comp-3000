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

	GameObject* findObject(shared_ptr<vector<shared_ptr<GameObject>>> allObjects, int id);

	void saveTime(dayCycle* time, XMLElement* entity);

	void saveUnit(XMLElement* entity, GameObject* unit);
	void saveNewUnit(XMLElement* layer, GameObject* unit);

	bool saveZombies(XMLElement* layer, vector<shared_ptr<ZombieObj>> zombie);
	void saveDepot(XMLElement* entity, GameObject* depot);
	void saveBuilding(XMLElement* entity, GameObject* building);

	void saveFile(shared_ptr<vector<shared_ptr<GameObject>>> allObjects, vector<shared_ptr<ZombieObj>> zombies, dayCycle* time);

	void saveHealth(XMLElement* entity, GameObject* obj);
	void saveResources(XMLElement* entity, GameObject* obj);
	void saveResourceTransfer(XMLElement* entity, GameObject* obj);
	bool saveTransfering(XMLElement* entity, GameObject* target, bool transfering);
	void saveMovement(XMLElement* entity, GameObject* obj);

private:
	XMLDocument doc;
	const char* filename;
};