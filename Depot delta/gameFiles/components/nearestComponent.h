#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

class nearestComponent : public Component {
public:
	virtual void update(GameObject* owner) { // update target based off of closest unit (need to add convoys too at later date)
		if (!checkForClosestUnit() && nearestUnit) {
			checkCurrentTarget();
		}
	}

	void checkCurrentTarget() {
		if (nearestUnit && distanceToNearest > searchDistance) {
			nearestUnit = nullptr;
			distanceToNearest = FLT_MAX;
			owner->setTarget(owner->getDimensions().x, owner->getDimensions().y);
			owner->setTargetObject(nullptr);
		}
	}

	bool checkForClosestUnit() {
		nearestUnit = nullptr;
		bool found = false;
		for (auto unit : *nearbyUnits) {
			if (distanceToUnit(unit.get()) <= sightDistance) {
				if (!nearestUnit) {
					nearestUnit = unit.get();
					distanceToNearest = distanceToUnit(unit.get());
					found = true;
				}
				else if (distanceToUnit(unit.get()) < distanceToNearest) {
					nearestUnit = unit.get();
					distanceToNearest = distanceToUnit(unit.get());
					found = true;
				}
			}
		}
		if (distanceToUnit(depot) <= sightDistance) {
			if (nearestUnit) {
				if (distanceToUnit(depot) < distanceToUnit(nearestUnit)) { 
					nearestUnit = depot;
					found = true;
				}
			}
			else {
				nearestUnit = depot;
				found = true;
			}
		}

		if (found) {
			owner->setTarget(nearestUnit->getDimensions().x + nearestUnit->getDimensions().w / 2, nearestUnit->getDimensions().y + nearestUnit->getDimensions().h / 2);
			owner->setTargetObject(nearestUnit);
			distanceToNearest = distanceToUnit(nearestUnit);
		}
		return found;
	}

	float distanceToUnit(GameObject* unit) {
		if (unit) { // no unit if zombie has just spawned
			SDL_FRect dimensions = owner->getDimensions();
			SDL_FRect unitDimensions = unit->getDimensions();
			float dx = (unitDimensions.x + unitDimensions.w / 2) - (dimensions.x + dimensions.w / 2);
			float dy = (unitDimensions.y + unitDimensions.h / 2) - (dimensions.y + dimensions.h / 2);
			return sqrt(dx * dx + dy * dy);
		}
	}

	//setters
	void setnearbyUnits(shared_ptr<vector<shared_ptr<HumanObj>>> units) {
		nearbyUnits = units;
		if (depot) { // without will try to find a target before completed spawning
			checkForClosestUnit();
		}
	}
	void setDepot(DepotObj* dDepot) {depot = dDepot;}
	void setSightDistance(float dist) { sightDistance = dist; }

	nearestComponent(GameObject* obj) : Component(obj) {}
	virtual ~nearestComponent() {}

private:
	shared_ptr<vector<shared_ptr<HumanObj>>> nearbyUnits;
	GameObject* nearestUnit = nullptr;

	DepotObj* depot = nullptr;

	bool nearbyTarget = false;

	float distanceToNearest = FLT_MAX;

	float sightDistance = 200.0f; // distance to check for nearby units
	float searchDistance = sightDistance + 100.0f; // distance to search for units
};