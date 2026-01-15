#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

class nearestComponent : public Component {
public:
	virtual void update(GameObject* owner) { // update target based off of closest unit (need to add convoys too at later date)
		nearestUnit = nullptr;
		for (auto unit : nearbyUnits) {
			if (distanceToUnit(unit) <= sightDistance) {
				if (!nearestUnit) {
					nearestUnit = unit;
					distanceToNearest = distanceToUnit(unit);
				}
				else if (distanceToUnit(unit) < distanceToNearest) {
					nearestUnit = unit;
					distanceToNearest = distanceToUnit(unit);
				}
			}
		}
		if (distanceToUnit(depot) < distanceToNearest) {
			nearestUnit = nullptr;
			distanceToNearest = distanceToUnit(depot);
			owner->setTarget(depot->getPos().x, depot->getPos().x);
			owner->setTargetObject(depot);
		}
		if (nearestUnit && distanceToNearest > searchDistance) {
			nearestUnit = nullptr;
			distanceToNearest = FLT_MAX;
			owner->setTarget(depot->getPos().x, depot->getPos().x);
			owner->setTargetObject(depot);
		}
		else if (nearestUnit) {
			owner->setTarget(nearestUnit->getDimensions().x + nearestUnit->getDimensions().w / 2, nearestUnit->getDimensions().y + nearestUnit->getDimensions().h / 2);
			owner->setTargetObject(nearestUnit);
			distanceToNearest = distanceToUnit(nearestUnit);
		}
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
	void setnearbyUnits(vector<GameObject*> units) { nearbyUnits = units; }
	void setDepot(DepotObj* dDepot) {depot = dDepot;}

	nearestComponent(GameObject* obj) : Component(obj) {}
	virtual ~nearestComponent() {}

private:
	vector<GameObject*> nearbyUnits;
	GameObject* nearestUnit = nullptr;

	DepotObj* depot = nullptr;

	float distanceToNearest = FLT_MAX;

	float sightDistance = 200.0f; // distance to check for nearby units
	float searchDistance = sightDistance + 100.0f; // distance to search for units
};