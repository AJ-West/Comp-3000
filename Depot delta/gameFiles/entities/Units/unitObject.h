#pragma once

#include "gameFiles/entities/humanObject.h"

class UnitObj : public HumanObj {
public:
	UnitObj(int x, int y, int width, int height, int health, int id);
	~UnitObj() {}

	virtual void renderHover(SDL_Renderer* renderer);

	virtual void updateTargets(shared_ptr<vector<shared_ptr<ZombieObj>>> list);

	virtual float getRateOfFire();
	virtual int getDamage();

	virtual int getUnitOrConvoy() { return UnitConvoyTypes::UNIT; }
	virtual int getVehicleCapacity();
};