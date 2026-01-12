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

using namespace std;
using namespace tinyxml2;

class MapSaver
{
public:
	MapSaver(const char* filename);
	~MapSaver();

	bool saveUnit(XMLElement* entity, vector<GameObject*> units);
	void saveNewUnit(XMLElement* layer, GameObject* unit);

	bool saveConvoy(XMLElement* entity, vector<ConvoyObj*> convoys);

	bool saveZombie(XMLElement* entity, vector<ZombieObj*> zombie);
	void saveDepot(XMLElement* entity, DepotObj* depot);

	void saveFile(vector<GameObject*> unitConvoys, DepotObj* depot, vector<ZombieObj*> zombies);

	void saveHealth(XMLElement* entity, GameObject* obj);
	void saveResources(XMLElement* entity, GameObject* obj);
	void saveMovement(XMLElement* entity, GameObject* obj);

private:
	XMLDocument doc;
	const char* filename;
};