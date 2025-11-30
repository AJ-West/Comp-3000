#pragma once
#include "GameObject.h"
#include "resourceComponent.h"
#include "pathfindingComponent.h"

class ZombieObj : public GameObject {
public:
	ZombieObj(int x, int y, int width, int height, int health, int id) : GameObject(x, y, width, height, health), ID(id) {}

	void renderHover(SDL_Renderer* renderer) { cout << "render hover"; } // need at a later date to show zombie health on hover

	//getters
	Vec2 getTargetPos() { return Vec2{ tx,ty }; }
	int getID() { return ID; }

	//setters
	virtual void setTarget(float x, float y) {
		tx = x; ty = y;
		pathToTarget();
	};

	void pathToTarget() {
		Vec2 target = { tx,ty };
		SDL_FRect size = getDimensions();
		Vec2 origin = { size.x + size.w / 2, size.y + size.h / 2 };
		auto pathComp = getComponent<pathfindingComponent>();
		pathComp->computeFlowField(target, origin);
	}
	//unit object functions not needed for zombies

	void onClick() {
		cout << "zombie clicked";
	}

	void clickAway() { // set target pos to clicked position
		cout << "zombie click away";
	}

private:
	int ID;

	SDL_FRect iSize{ camera.dimen.x + 10, camera.dimen.y + camera.dimen.h - 75, 50, 50 };
	SDL_FRect tSize{ camera.dimen.x + 60, camera.dimen.y + camera.dimen.h - 75, 100, 50 };
};