#include "gameFiles/entities/humanObject.h"

#include "gameFiles/components/attackComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/pathfindingComponent.h"

void HumanObj::onClick() {
	selected = !selected;
}

void HumanObj::clickAway() { // set target pos to clicked position
	getMapScaledMousePos(&tx, &ty);
	pathToTarget();
	auto rC = getComponent<resourceComponent>();
	rC->adjustResourceChange(FUEL, -1 * rC->getResourcesCount(PERSONNEL)/ getVehicleCapacity()); // fuel use increase for every 5 units
	soundEffectEngine->play2D(pickRandomFile(orderVAFiles));
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
		if (keyName == "increaseRateOfFire") {
			auto aC = getComponent<attackComponent>();
			if (aC) {
				aC->setAttackCooldown(aC->getAttackCooldown() * unitTechVal[keyName]);
			}
		}
		else if (keyName == "largerBasicInfantry") {
			auto rC = getComponent<resourceComponent>();
			if (rC) {
				rC->setResourceMax(PERSONNEL, rC->getResourcesMax(PERSONNEL) * unitTechVal[keyName]);
			}
		}
		else if (keyName == "largerBasicCarryingCapacity") {
			auto rC = getComponent<resourceComponent>();
			if (rC) {
				rC->setResourceMax(AMMUNITION, rC->getResourcesMax(AMMUNITION) * unitTechVal[keyName]);
				rC->setResourceMax(DOS, rC->getResourcesMax(DOS) * unitTechVal[keyName]);
				rC->setResourceMax(FUEL, rC->getResourcesMax(FUEL) * unitTechVal[keyName]);
				rC->setResourceMax(SCRAP, rC->getResourcesMax(SCRAP) * unitTechVal[keyName]);
			}
		}
		else if (keyName == "increaseBasicRateOfFire") {
			auto rC = getComponent<resourceComponent>();
			if (rC) {
				rC->setResourceMax(PERSONNEL, rC->getResourcesMax(PERSONNEL) * unitTechVal[keyName]);
			}
		}
	}
	else {
		if (keyName == "increaseConvoyCapacity") {
			auto rC = getComponent<resourceComponent>();
			if (rC) {
				rC->setResourceMax(AMMUNITION, rC->getResourcesMax(AMMUNITION) * convoyTechVal[keyName]);
				rC->setResourceMax(DOS, rC->getResourcesMax(DOS) * convoyTechVal[keyName]);
				rC->setResourceMax(FUEL, rC->getResourcesMax(FUEL) * convoyTechVal[keyName]);
				rC->setResourceMax(SCRAP, rC->getResourcesMax(SCRAP) * convoyTechVal[keyName]);
			}
		}
		else if (keyName == "increaseBasicSpeed") {
			auto mC = getComponent<movementComponent>();
			if (mC) {
				mC->setSpeed(mC->getSpeed() * convoyTechVal[keyName]);
			}
		}
		else if (keyName == "increaseBasicCapacity") {
			auto rC = getComponent<resourceComponent>();
			if (rC) {
				rC->setResourceMax(AMMUNITION, rC->getResourcesMax(AMMUNITION) * convoyTechVal[keyName]);
				rC->setResourceMax(DOS, rC->getResourcesMax(DOS) * convoyTechVal[keyName]);
				rC->setResourceMax(FUEL, rC->getResourcesMax(FUEL) * convoyTechVal[keyName]);
				rC->setResourceMax(SCRAP, rC->getResourcesMax(SCRAP) * convoyTechVal[keyName]);
			}
		}
	}
}