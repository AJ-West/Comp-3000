#pragma once
#include "GameObject.h"
#include "resourceComponent.h"
#include "resourceTransferComponent.h"

class ConvoyObj : public GameObject {
public:
	ConvoyObj(int x, int y, int width, int height, int id) : GameObject(x, y, width, height), ID(id) {}

	void onClick() {
		selected = !selected;
	}

	void clickAway() { // set target pos to clicked position
		getScaledMousePos(&tx, &ty);
		pathToTarget();
	}

	void renderHover(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			rComp->renderResources(renderer, tSize);
		}
		auto rTComp = getComponent<resourceTransferComponent>();
		if (rTComp) {
			rTComp->renderTransferArea();
		}
	}

	//getters
	Vec2 getTargetPos() { return Vec2{tx, ty}; }
	int getID() { return ID; }

	//setters
	virtual void setTarget(float x, float y) { 
		tx = x; ty = y;
		pathToTarget();
	};

	void pathToTarget() {
		Vec2 target = { tx,ty };
		SDL_FRect size = getDimensions();
		Vec2 origin = { size.x + size.w / 2, size.y + size.h / 2 };
		auto pathComp = getComponent<pathfindingComponent>();
		pathComp->computeFlowField(target, origin);
	}

private:
	int ID;

	SDL_FRect tSize{ 1254.0f / 1440.0f * camera.dimen.w, 132.0f / 960.0f * camera.dimen.h, 182.0f / 1440.0f * camera.dimen.w, 48.0f / 960.0f * camera.dimen.h };
};