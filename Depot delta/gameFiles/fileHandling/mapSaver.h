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

	bool saveUnit(XMLElement* entity, vector<HumanObj*> units);
	void saveNewUnit(XMLElement* layer, HumanObj* unit);

	bool saveConvoy(XMLElement* entity, vector<ConvoyObj*> convoys);

	bool saveZombie(XMLElement* entity, vector<ZombieObj*> zombie);
	void saveDepot(XMLElement* entity, DepotObj* depot);

	void saveFile(vector<HumanObj*> unitConvoys, DepotObj* depot, vector<ZombieObj*> zombies);

	void saveHealth(XMLElement* entity, GameObject* obj);
	void saveResources(XMLElement* entity, GameObject* obj);
	void saveMovement(XMLElement* entity, GameObject* obj);

private:
	XMLDocument doc;
	const char* filename;
};