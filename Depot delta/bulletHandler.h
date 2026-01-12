#pragma once
#include <SDL3/SDL.h>
#include "variables.h"
#include "usefulFunctions.h"
#include <random>
#include <iomanip>

struct Bullet {
	Vec2 pos, vel;
	int speed = 1;
	bool inUse = false;
	SDL_FRect size = { 0, 0, 5, 5 };
	int lifetime;

	void init(Vec2 pVec, SDL_FRect target) {
		Vec2 tVec = Vec2{ (target.x + target.w / 2) + target.w* randDir(), (target.y + target.h / 2)+ target.h* randDir()};
		//bullet init pos
		pos.x = pVec.x;
		pos.y = pVec.y;
		size.x = pos.x - size.w/2;
		size.y = pos.y - size.h/2;
		//bullet velocity
		vel = normalise(Vec2{ tVec.x - pVec.x, tVec.y - pVec.y });
		//how long until the bullet reaches its target
		lifetime = mag(tVec.sub(pVec))/ mag(vel);
		inUse = true;
	}

	// creates random value between -0.5 and 0.5 to add bullet spread
	float randDir() {
		return (static_cast<float>(rand()) / RAND_MAX) - 0.5f;
	}

	void update() {
		if (inUse) {
			pos = pos.add(vel.mul(speed));
			lifetime -= 1;
			if (lifetime <= 0) {	inUse = false; }
		}
	}

	void render(SDL_Renderer*) {
		if (inUse) {
			size.x = pos.x - size.w / 2;
			size.y = pos.y - size.h / 2;
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderFillRect(renderer, &size);
		}
	}

	bool getInUse() { return inUse;	}
	void setInUse(bool use) { inUse = use; }
};

class bulletHandler {
public:
	void spawnBullet(Vec2 pVec, SDL_FRect target) {
		for (int i = 0; i < POOL_SIZE; i++) {
			if (!bullets[i].getInUse()) {
				bullets[i].init(pVec, target);
				return;
			}
		}
	}

	void clearBullets() {
		for (int i = 0; i < POOL_SIZE; i++) {
			bullets[i].setInUse(false);
		}
	}

	void update() {
		for (int i = 0; i < POOL_SIZE; i++) {
			bullets[i].update();
		}
	}

	void render(SDL_Renderer* renderer) {
		for (int i = 0; i < POOL_SIZE; i++) {
			bullets[i].render(renderer);
		}
	}

private:
	static const int POOL_SIZE = 150;
	Bullet bullets[POOL_SIZE];

};