#pragma once
#include "GameObject.h"
#include "buttonBase.h"
#include "resourceComponent.h"

class DepotObj : public GameObject, public BaseButton {
public:
	DepotObj(int x, int y, int width, int height) : GameObject(x, y, width, height) {}

	virtual void onClick() override {
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

	//getters
	bool getSelected() { return selected; }
	bool getHovering() { return hovering; }

	//setters
	void setHovering(bool hover) { hovering = hover; }

private:
	bool selected = false;
	bool hovering = false;

	float tx = NULL;
	float ty = NULL;

	int ID;

	SDL_FRect iSize{ camera.x + 10, camera.y + 5, 30, 30 };
	SDL_FRect tSize{ camera.x + 40, camera.y + 5, 60, 30 };
};