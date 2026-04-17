#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/entities/Buildings/building.h"

class nearestComponent : public Component {
public:
	virtual void update() { // update target based off of closest unit (need to add convoys too at later date)
		if (frameCount % 60 == 0) { //check every second instead of every frame
			if (!checkForClosestUnit() && !nearestUnit.expired()) {
				checkCurrentTarget();
			}
		}
	}

	void checkCurrentTarget() {
		if (!nearestUnit.expired() && distanceToUnit(nearestUnit.lock().get()) > searchDistance) {
			nearestUnit.reset();
			owner.lock()->setTarget(owner.lock()->getDimensions().x, owner.lock()->getDimensions().y);
			owner.lock()->removeTargetObject();
		}
	}

	bool checkForClosestUnit() {
		bool found = false;
		if (!nearestUnit.expired()) {
			if (!nearestUnit.lock()->getAlive()) { nearestUnit.reset(); }
		}
		for (auto unit : *nearbyUnits) {
			float dist = distanceToUnit(unit.get());
			if (dist <= sightDistance) {
				if (nearestUnit.expired()) {
					nearestUnit = unit;
					found = true;
				}
				else if (dist <= distanceToUnit(nearestUnit.lock().get())) {
					nearestUnit = unit;
					found = true;
				}
			}
		}
		for (auto building : *nearbyBuildings) {
			if (building->getAlive()) {
				float dist = distanceToUnit(building.get());
				if (dist <= sightDistance) {
					if (nearestUnit.expired()) {
						nearestUnit = building;
						found = true;
					}
					else if (dist <= distanceToUnit(nearestUnit.lock().get())) {
						nearestUnit = building;
						found = true;
					}
				}
			}
		}
		if (distanceToUnit(depot.lock().get()) <= sightDistance) {
			if (!nearestUnit.expired()) {
				if (distanceToUnit(depot.lock().get()) <= distanceToUnit(nearestUnit.lock().get())) {
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
			owner.lock()->setTarget(nearestUnit.lock()->getDimensions().x + nearestUnit.lock()->getDimensions().w / 2, nearestUnit.lock()->getDimensions().y + nearestUnit.lock()->getDimensions().h / 2);
			owner.lock()->setTargetObject(nearestUnit);
		}
		return found;
	}

	float distanceToUnit(GameObject* unit) {
		if (unit) { // no unit if zombie has just spawned
			SDL_FRect dimensions = owner.lock()->getDimensions();
			SDL_FRect unitDimensions = unit->getDimensions();
			float dx = (unitDimensions.x + unitDimensions.w / 2) - (dimensions.x + dimensions.w / 2);
			float dy = (unitDimensions.y + unitDimensions.h / 2) - (dimensions.y + dimensions.h / 2);
			return sqrt(dx * dx + dy * dy);
		}
	}

	//setters
	void setnearbyUnits(shared_ptr<vector<shared_ptr<HumanObj>>> units) {
		nearbyUnits = units;
		if (!depot.expired()) { // without will try to find a target before completed spawning
			checkForClosestUnit();
		}
	}
	void setnearbyBuildings(shared_ptr<vector<shared_ptr<BuildingObj>>> buildings) {
		nearbyBuildings = buildings;
		if (!depot.expired()) { // without will try to find a target before completed spawning
			checkForClosestUnit();
		}
	}
	void setDepot(weak_ptr<DepotObj> dDepot) {depot = dDepot;}
	void setSightDistance(float dist) { sightDistance = dist; }
	void setNearestUnit(weak_ptr<GameObject> unit) {
		nearestUnit = unit;
		if (nearestUnit.expired()) {
			nearbyTarget = false;
		}
	}
	void removeNearestUnit() { nearestUnit.reset(); }

	//getters
	float getSightDistance() { return sightDistance; }

	nearestComponent(weak_ptr<GameObject> obj, float sight) : Component(obj) {
		sightDistance = sight;
		searchDistance = sight + 100.0f;
	}
	virtual ~nearestComponent() {}

private:
	shared_ptr<vector<shared_ptr<HumanObj>>> nearbyUnits;
	shared_ptr<vector<shared_ptr<BuildingObj>>> nearbyBuildings;
	weak_ptr<GameObject> nearestUnit;

	weak_ptr<DepotObj> depot;

	bool nearbyTarget = false;

	float sightDistance; // distance to check for nearby units
	float searchDistance; // distance to search for units
};