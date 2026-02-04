#include "gameFiles/useThroughout/variables.h"

// Frame rate handling
const int FPS = 60;
const int frameDelay = 1000/ FPS;
float deltaTime = 0;

// Screen
int screenWidth = 1920/2;
int screenHeight = 1080/2;
int ResolutionWidth = screenWidth/2;
int ResolutionHeight = screenHeight/2;

int worldWidth = 1984;
int worldHeight = 1088;
const int WORLD_TILE_SIZE = 16;
const int WORLD_TILE_COLS = worldWidth / WORLD_TILE_SIZE;
const int WORLD_TILE_ROWS = worldHeight / WORLD_TILE_SIZE;
const float DESIGN_SCALE_X = 1440.0f;
const float DESIGN_SCALE_Y = 960.0f;

// camera zoom
float zoom = 1.0;

//Scaling art
float tileWidth;
float tileHeight;

// SDL
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
int FONT_SIZE = 20;

bool isRunning;

Camera camera = { 0,0, ResolutionWidth, ResolutionHeight };

std::vector<std::vector<Tile>> grid;

unordered_map<string, float> depotTechVal = {
	{"depotHealthIncrease", 1},
	{"fuelProductionIncrease", 1},
	{"dosProductionIncrease", 1},
	{"ammoProductionIncrease", 1},
	{"decreaseFuelCosts", 1},
	{"decreaseDoSCosts", 1},
	{"decreaseAmmoCosts", 1},
	{"increasePersonnelCapacity", 1},
};

unordered_map<string, float> unitTechVal = {
	{"increaseUnitDoSEfficiency", 1},
	{"increaseUnitFuelEfficiency", 1},
	{"increaseRateOfFire", 1},
	{"largerBasicInfantry", 1},
	{"largerBasicCarryingCapacity", 1},
	{"increaseBasicFuelEfficiency", 1},
	{"increaseBasicRateOfFire", 1},
	{"increaseBasicDamage", 1},
};

unordered_map<string, float> convoyTechVal = {
	{"increaseConvoyCapacity", 1},
	{"increaseConvoyFuelEfficiency", 1},
	{"increaseBasicArmour", 1},
	{"increaseBasicSpeed", 1},
	{"increaseBasicCapacity", 1},
};