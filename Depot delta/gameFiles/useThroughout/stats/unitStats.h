#pragma once

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/resourceTransferComponent.h"
#include "gameFiles/components/pathfindingComponent.h"
#include "gameFiles/components/attackComponent.h"

#ifndef UNITSTATS_H
#define UNITSTATS_H

struct UnitStats { // default
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

	void addComponents(GameObject* unit) {
		unit->AddComponent(make_shared<renderComponent>(unit, renderer, art));
		unit->AddComponent(make_shared<buttonComponent>(unit));
		unit->AddComponent(make_shared<movementComponent>(unit, movementSpeed));
		unit->AddComponent(make_shared<resourceComponent>(unit, rMax, rCount, loadResourceTextures()));
		unit->AddComponent(make_shared<resourceTransferComponent>(unit, renderer, rTransferDistance, rTransferRate));
		unit->AddComponent(make_shared<pathfindingComponent>(unit, grid));
		unit->AddComponent(make_shared<attackComponent>(unit, aDamage, aRange));
	}

	float getROF() { // get rate of fire modifier
		return unitTechVal.at("increaseRateOfFire") * unitTechVal.at("increaseBasicRateOfFire") * aCooldown;
	}

	void assignHasResource(GameObject* unit) {
		auto rComp = unit->getComponent<resourceComponent>();
		if (rComp) {
			vector<bool> hasResources{ rCount[PERSONNEL] == 0, rCount[AMMUNITION] == 0, rCount[DOS] == 0, rCount[FUEL] == 0, rCount[SCRAP] == 0 };
			rComp->setHasResource(hasResources);
		}
	}
};

extern UnitStats unitStats;

#endif // !UNITSTATS_H