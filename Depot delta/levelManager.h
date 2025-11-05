#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

using namespace std;
using namespace tinyxml2;

class LevelManager
{
public:
	LevelManager(const char* filename, SDL_Renderer* renderer);
	~LevelManager();

	void loadTileMap();

	void renderTileMap(SDL_Renderer* renderer);

private:
	int width, height;
    int cell_width, cell_height;
	int x_cells, y_cells;
    vector<vector<int>> tilelocs;
    vector<vector<SDL_FRect>> tilemap;

	SDL_Texture* tilemapTexture;
};