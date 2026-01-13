#pragma once
#include "GameObject.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"
#include "resourceTransferComponent.h"
#include "pathfindingComponent.h"
#include "attackComponent.h"

class UnitObj : public GameObject {
public:
	UnitObj(int x, int y, int width, int height, int health, int id) : GameObject(x, y, width, height, health), ID(id) {
		maxHealth = 100;
	}

	void onClick() {
		selected = !selected;
	}

	void clickAway() { // set target pos to clicked position
		getMapScaledMousePos(&tx, &ty);
		pathToTarget();
	}

	void renderHover(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
			rComp->renderResources(renderer, tSize);
		}
	}

	//getters
	Vec2 getTargetPos() { return Vec2{tx,ty}; }
	int getID() { return ID; }
	virtual int getMaxHealth() { return maxHealth; }

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
	int maxHealth = 100;
};

struct unitStats {
	const char* art = "draftArt/basicUnit.png";
	int movementSpeed = 50;
	int maxHealth = 100;
	std::vector<int> rMax = { 100, 100, 100, 100, 100 };
	std::vector<int> rCount = { 50, 50, 50, 50, 50 };

	int rTransferDistance = 50;
	std::vector<int> rTransferRate = { 5,5,5,5,5 };

	int aDamage = 5;
	int aRange = 100;
	int aCooldown = 500;

	void addComponents(UnitObj* unit) {
		unit->AddComponent(make_shared<renderComponent>(unit, renderer, art));
		unit->AddComponent(make_shared<buttonComponent>(unit));
		unit->AddComponent(make_shared<movementComponent>(unit, movementSpeed));
		unit->AddComponent(make_shared<resourceComponent>(unit, rMax, rCount, loadResourceTextures()));
		unit->AddComponent(make_shared<resourceTransferComponent>(unit, renderer, rTransferDistance, rTransferRate));
		unit->AddComponent(make_shared<pathfindingComponent>(unit, grid));
		unit->AddComponent(make_shared<attackComponent>(unit, aDamage, aRange, aCooldown));
	}
};