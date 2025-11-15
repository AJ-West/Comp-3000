#pragma once
#include "GameObject.h"
#include "buttonBase.h"
#include "resourceComponent.h"

class ConvoyObj : public GameObject, public BaseButton {
public:
	ConvoyObj(int x, int y, int width, int height, int id) : GameObject(x, y, width, height), ID(id) {}

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

	//getters
	pair<float, float> getTargetPos() { return pair<float, float>{tx, ty}; }
	int getID() { return ID; }

	//setters
	virtual void setTarget(float x, float y) { tx = x; ty = y; };

private:
	int ID;

	SDL_FRect iSize{ camera.x + 10, camera.y + camera.height - 75, 50, 50 };
	SDL_FRect tSize{ camera.x + 60, camera.y + camera.height - 75, 100, 50 };
};