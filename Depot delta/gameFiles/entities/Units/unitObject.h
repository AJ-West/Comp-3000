#pragma once

#include "gameFiles/entities/humanObject.h"

class UnitObj : public HumanObj {
public:
	UnitObj(int x, int y, int width, int height, int health, int id);

	virtual void renderHover(SDL_Renderer* renderer);

	virtual void updateTargets(vector<ZombieObj*> list);

	virtual float getRateOfFire();
	virtual int getDamage();
};