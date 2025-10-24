#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <vector>
#include <sstream>

using namespace std;
using namespace tinyxml2;

class levelManager
{
public:
	levelManager(const char* filename);
	~levelManager();

private:
	int width, height;
    int cell_width, cell_height;
	int x_cells, y_cells;
    vector<vector<int>> tilemap;
};