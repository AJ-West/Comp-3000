#pragma once
#include "GameObject.h"
#include "unitObject.h"

class nearestComponent : public Component {
public:
	virtual void update(GameObject* owner) { // update target based off of closest unit (need to add convoys too at later date)
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
		if (nearestUnit && distanceToNearest > searchDistance) {
			nearestUnit = nullptr;
			distanceToNearest = FLT_MAX;
			owner->setTarget(NULL, NULL);
		}
		else if (nearestUnit) {
			owner->setTarget(nearestUnit->getDimensions().x + nearestUnit->getDimensions().w / 2, nearestUnit->getDimensions().y + nearestUnit->getDimensions().h / 2);
			distanceToNearest = distanceToUnit(nearestUnit);
		}
	}

	float distanceToUnit(UnitObj* unit) {
		SDL_FRect dimensions = owner->getDimensions();
		SDL_FRect unitDimensions = unit->getDimensions();
		float dx = (unitDimensions.x + unitDimensions.w / 2) - (dimensions.x + dimensions.w / 2);
		float dy = (unitDimensions.y + unitDimensions.h / 2) - (dimensions.y + dimensions.h / 2);
		return sqrt(dx * dx + dy * dy);
	}

	//setters
	void setnearbyUnits(vector<UnitObj*> units) { nearbyUnits = units; }

	nearestComponent(GameObject* obj) : Component(obj) {}
	virtual ~nearestComponent() {}

private:
	vector<UnitObj*> nearbyUnits;
	UnitObj* nearestUnit = nullptr;

	float distanceToNearest = FLT_MAX;

	float sightDistance = 200.0f; // distance to check for nearby units
	float searchDistance = sightDistance + 100.0f; // distance to search for units
};