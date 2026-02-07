#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/resourceComponent.h"

class BuildingObj : public GameObject {
public:
	BuildingObj(int x, int y, int width, int height, int health, int type, bool used);

	virtual void Update();

	void onClick();

	void clickAway();

	void renderHover(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);

private:

	int rType;

	int maxHealth = 300;
};