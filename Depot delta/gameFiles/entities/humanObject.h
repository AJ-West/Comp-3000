#pragma once
#include "gameFiles/entities/gameObject.h"

class HumanObj : public GameObject {
public:
	HumanObj(int x, int y, int width, int height, int health, int id) : GameObject(x, y, width, height, health), ID(id) {}
	virtual ~HumanObj() = default;

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

	virtual int getType() { return HUMAN; }
	virtual int getUnitOrConvoy() { return INT_MAX; }

	virtual int getVehicleCapacity() { return 1; };

private:
	int ID;
	int maxHealth = 100;
};