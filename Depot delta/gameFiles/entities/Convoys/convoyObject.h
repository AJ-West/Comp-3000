#pragma once
#include "gameFiles/entities/humanObject.h"

class ConvoyObj : public HumanObj {
public:
	ConvoyObj(int x, int y, int width, int height, int health, int id);
	
	virtual void renderHover(SDL_Renderer* renderer);

	virtual int getUnitOrConvoy() { return UnitConvoyTypes::CONVOY; }
	virtual int getVehicleCapacity();
};