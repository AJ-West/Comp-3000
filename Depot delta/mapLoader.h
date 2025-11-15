#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "string"
#include "unitObject.h"
#include "depot.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"

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
	void loadDepot(XMLElement* layer);

	void addUnitComponents(UnitObj* unit, XMLElement* entity);
	void addDepotComponents(DepotObj* unit, XMLElement* entity);

	vector<SDL_Texture*> loadResourceTextures();

	void saveFile(const char* filename, vector<UnitObj*> units, DepotObj* depot);

	void renderTileMap(SDL_Renderer* renderer);

	void save_unit(XMLElement* entity, XMLDocument& doc, vector<UnitObj*> units);
	void save_depot(XMLElement* entity, XMLDocument& doc, DepotObj* depot);

	void save_resources(XMLElement* entity, XMLDocument& doc, GameObject* unit);

	//getters
	vector<UnitObj*> getUnitList() { return unitList; }
	DepotObj* getDepot() { return depot; }

private:
	int width, height;
	int x_cells, y_cells;
    vector<vector<int>> tilelocs;
    vector<vector<SDL_FRect>> tilemap;

	SDL_Texture* tilemapTexture;

	DepotObj* depot;
	vector<UnitObj*> unitList;
};