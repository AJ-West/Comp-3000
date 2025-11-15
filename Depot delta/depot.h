#pragma once
#include "GameObject.h"
#include "buttonBase.h"
#include "resourceComponent.h"

class DepotObj : public GameObject, public BaseButton {
public:
	DepotObj(int x, int y, int width, int height) : GameObject(x, y, width, height) {}

	void onClick() {
		selected = !selected;
	}

	void clickAway() {
		getScaledMousePos(&tx, &ty);
	}

	void renderHover(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			rComp->renderResources(renderer, iSize, tSize);
		}
	}

	void renderResources(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			rComp->renderResources(renderer, iSize, tSize);
		}
	}

private:
	SDL_FRect iSize{ camera.x + 10, camera.y + 5, 30, 30 };
	SDL_FRect tSize{ camera.x + 40, camera.y + 5, 60, 30 };
};