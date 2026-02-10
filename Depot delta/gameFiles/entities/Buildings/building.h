#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/renderComponent.h"

class BuildingObj : public GameObject {
public:
	BuildingObj(int x, int y, int width, int height, int health, int type, bool used, SDL_Renderer* renderer);

	virtual void Update();

	void onClick();

	void clickAway();

	void renderHover(SDL_Renderer* renderer);

private:

	int rType;

	int maxHealth = 300;
};