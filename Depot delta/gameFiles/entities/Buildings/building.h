#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/renderComponent.h"

class BuildingObj : public GameObject {
public:
	BuildingObj(int x, int y, int width, int height, int health, bool used, int type);

	virtual void Update();

	void onClick();

	void clickAway();

	void renderHover(SDL_Renderer* renderer);

	//getters
	int getType() { return rType; }

private:
	int rType;

	int maxHealth = 300;
};