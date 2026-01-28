#pragma once
#include "gameFiles/entities/gameObject.h"

class HumanObj : public GameObject {
public:
	HumanObj(int x, int y, int width, int height, int health, int id) : GameObject(x, y, width, height, health), ID(id) {}

	void onClick();

	void clickAway();

	virtual void renderHover(SDL_Renderer* renderer) {}

	//getters
	Vec2 getTargetPos() { return Vec2{ tx, ty }; }
	int getID() { return ID; }
	virtual int getMaxHealth() { return maxHealth; }

	//setters
	virtual void setTarget(float x, float y) {
		tx = x; ty = y;
		pathToTarget();
	};

	void pathToTarget();

	void updateStats(string keyName, bool forUnit);

private:
	int ID;
	int maxHealth = 100;
};