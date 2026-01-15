#pragma once
#include "gameFiles/entities/humanObject.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/resourceTransferComponent.h"
#include "gameFiles/components/pathfindingComponent.h"
#include "gameFiles/components/attackComponent.h"

class UnitObj : public HumanObj {
public:
	UnitObj(int x, int y, int width, int height, int health, int id) : HumanObj(x, y, width, height, health, id) {}

	virtual void renderHover(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
			rComp->renderResources(renderer, tSize);
		}
	}

	virtual void updateTargets(vector<ZombieObj*> list) {
		getComponent<attackComponent>()->setPotentialTargets(list);
	};
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
	int aRange = 500;
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

	void assignHasResource(UnitObj* unit) {
		auto rComp = unit->getComponent<resourceComponent>();
		if (rComp) {
			vector<bool> hasResources{ rCount[PERSONNEL] == 0, rCount[AMMUNITION] == 0, rCount[DOS] == 0, rCount[FUEL] == 0, rCount[SCRAP] == 0 };
			rComp->setHasResource(hasResources);
		}
	}
};