#pragma once
#include "GameObject.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"
#include "resourceTransferComponent.h"
#include "pathfindingComponent.h"

class ConvoyObj : public GameObject {
public:
	ConvoyObj(int x, int y, int width, int height, int health, int id) : GameObject(x, y, width, height, health), ID(id) {}

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
			SDL_FRect tSize{ 1254.0f / 1440.0f * camera.dimen.w, 60.0f / 960.0f * camera.dimen.h, 182.0f / 1440.0f * camera.dimen.w, 48.0f / 960.0f * camera.dimen.h };
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

	
};

struct convoyStats {
	const char* art = "draftArt/basicConvoy.png";
	int movementSpeed = 100;
	std::vector<int> rMax = { 100, 100, 100, 100, 100 };
	std::vector<int> rCount = { 50, 50, 50, 50, 50 };

	int rTransferDistance = 50;
	std::vector<int> rTransferRate = { 5,5,5,5,5 };

	void addComponents(ConvoyObj* convoy) {
		convoy->AddComponent(make_shared<renderComponent>(convoy, renderer, art));
		convoy->AddComponent(make_shared<buttonComponent>(convoy));
		convoy->AddComponent(make_shared<movementComponent>(convoy, movementSpeed));
		convoy->AddComponent(make_shared<resourceComponent>(convoy, rMax, rCount, loadResourceTextures()));
		convoy->AddComponent(make_shared<resourceTransferComponent>(convoy, renderer, rTransferDistance, rTransferRate));
		convoy->AddComponent(make_shared<pathfindingComponent>(convoy, grid));
	}
};