#include "gameFiles/entities/humanObject.h"

#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/pathfindingComponent.h"

void HumanObj::onClick() {
	selected = !selected;
}

void HumanObj::clickAway() { // set target pos to clicked position
	getMapScaledMousePos(&tx, &ty);
	pathToTarget();
	getComponent<resourceComponent>()->setResourceChange(FUEL, -1);
}

void HumanObj::pathToTarget() {
	Vec2 target = { tx,ty };
	SDL_FRect size = getDimensions();
	Vec2 origin = { size.x + size.w / 2, size.y + size.h / 2 };
	auto pathComp = getComponent<pathfindingComponent>();
	pathComp->computeFlowField(target, origin);
}

void HumanObj::updateStats(string keyName, bool forUnit) {
	if (forUnit) {
		if (keyName == "increaseUnitDoSEfficiency") { // need to do but requires rework to resource system
			return;
		}
		if (keyName == "increaseUnitFuelEfficiency") { // need to do but requires rework to resource system
			return;
		}
		if (keyName == "increaseRateOfFire") {
			return;
		}
		if (keyName == "largerBasicInfantry") {
			return;
		}
		if (keyName == "largerBasicCarryingCapacity") {
			return;
		}
		if (keyName == "increaseBasicFuelEfficiency") { // need to do but requires rework to resource system
			return;
		}
		if (keyName == "increaseBasicRateOfFire") {
			return;
		}
		if (keyName == "increaseBasicDamage") {
			return;
		}
	}
	else {
		if (keyName == "increaseConvoyCapacity") {
			return;
		}
		if (keyName == "increaseConvoyFuelEfficiency") { // need to do but requires rework to resource system
			return;
		}
		if (keyName == "increaseBasicArmour") {
			return;
		}
		if (keyName == "increaseBasicSpeed") {
			return;
		}
		if (keyName == "increaseBasicCapacity") {
			return;
		}
	}
}