#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "string"
#include "unitObject.h"

using namespace std;
using namespace tinyxml2;

class MapLoader
{
public:
	MapLoader(const char* filename, SDL_Renderer* renderer);
	~MapLoader();

	void loadTilemap(XMLElement* layer);
	void loadEntities(XMLElement* layer);

	void saveFile(const char* filename, vector<UnitObj*> units);

	void renderTileMap(SDL_Renderer* renderer);

	//getters
	vector<UnitObj*> getUnitList() { return unitList; }

private:
	int width, height;
	int x_cells, y_cells;
    vector<vector<int>> tilelocs;
    vector<vector<SDL_FRect>> tilemap;

	SDL_Texture* tilemapTexture;
	
	vector<UnitObj*> unitList;
};