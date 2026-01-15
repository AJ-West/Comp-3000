#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <vector>

#ifndef VARIABLES_H
#define VARIABLES_H

// Frame rate handling
extern const int FPS;
extern const int frameDelay;
extern float deltaTime;

// Screen
extern int screenWidth;
extern int screenHeight;
extern int ResolutionWidth;
extern int ResolutionHeight;
extern int worldWidth;
extern int worldHeight;
extern const int WORLD_TILE_SIZE;
extern const int WORLD_TILE_COLS;
extern const int WORLD_TILE_ROWS;
extern const float DESIGN_SCALE_X;
extern const float DESIGN_SCALE_Y;

//camera zoom
extern float zoom;

//Scaling art
extern float tileWidth;
extern float tileHeight;

// SDL
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern int FONT_SIZE;

extern bool isRunning;

struct Camera {
	SDL_FRect dimen{0,0,0,0};

	float xScale;
	float yScale;

	int x_dir = 0;
	int y_dir = 0;

	int speed = 150;

	void keyDown(SDL_Keycode key) {
		switch (key) {
		case SDLK_W:
			y_dir = -1;
			break;
		case SDLK_S:
			y_dir = 1;
			break;
		case SDLK_A:
			x_dir = -1;
			break;
		case SDLK_D:
			x_dir = 1;
			break;
		default:
			break;
		}
	}

	void keyUp(SDL_Keycode key) {
		switch (key) {
		case SDLK_W: case SDLK_S:
			y_dir = 0;
			break;
		case SDLK_A: case SDLK_D:
			x_dir = 0;
			break;
		default:
			break;
		}
	}

	void update() {
		dimen.x += x_dir * speed * deltaTime;
		dimen.y += y_dir * speed * deltaTime;
		clamp();
	}

	void clamp() {
		if (dimen.x < 0) dimen.x = 0;
		if (dimen.y < 0) dimen.y = 0;
		if (dimen.x + dimen.w > worldWidth) dimen.x = worldWidth - dimen.w;
		if (dimen.y + dimen.h > worldHeight) dimen.y = worldHeight - dimen.h;
	}

};

extern Camera camera;

enum Resource {
	PERSONNEL,
	AMMUNITION,
	DOS,
	FUEL,
	SCRAP,
	RESOURCE_COUNT
};

enum time { 
	second,
	minute,
	hour,
	day
};

struct Vec2 {
	float x;
	float y;

	Vec2 add(int val) {
		this->x += val;
		this->y += val;
		return *this;
	}

	Vec2 add(Vec2 vals) {
		this->x += vals.x;
		this->y += vals.y;
		return *this;
	}

	Vec2 sub(int val) {
		this->x -= val;
		this->y -= val;
		return *this;
	}

	Vec2 sub(Vec2 vals) {
		this->x -= vals.x;
		this->y -= vals.y;
		return *this;
	}

	Vec2 mul(int mult) {
		this->x *= mult;
		this->y *= mult;
		return *this;
	}

	Vec2 mul(Vec2 mults) {
		this->x *= mults.x;
		this->y *= mults.y;
		return *this;
	}

	Vec2 div(int mult) {
		this->x = this->x/mult;
		this->y = this->y/mult;
		return *this;
	}

	Vec2 div(Vec2 mults) {
		this->x *= this->x / mults.x;
		this->y *= this->y / mults.y;
		return *this;
	}
};

struct Tile {
	bool walkable = false;
	int cost = INT_MAX; // used for pathfinding
	Vec2 direction = { 0,0 }; // used for flow field pathfinding
};

//Used to store the movable world map
extern std::vector<std::vector<Tile>> grid;

#endif // !VARIABLES_H