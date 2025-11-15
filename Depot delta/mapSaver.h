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

	void save_unit(XMLElement* entity, vector<UnitObj*> units);
	void save_convoy(XMLElement* entity, vector<ConvoyObj*> convoys);
	void save_depot(XMLElement* entity, DepotObj* depot);

	void saveFile(vector<UnitObj*> units, DepotObj* depot, vector<ConvoyObj*> convoys);

	void save_resources(XMLElement* entity, GameObject* unit);

private:
	XMLDocument doc;
	const char* filename;
};