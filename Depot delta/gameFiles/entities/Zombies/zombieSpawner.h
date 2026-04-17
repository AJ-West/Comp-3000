#pragma once
#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/levelHandling/levelManager.h"

class ZombieSpawner {
public:
	ZombieSpawner(LevelManager* lManager): manager(lManager) {}
	~ZombieSpawner() {}

	bool checkIfSpawn() {
		if (frameStart - sLastTime >= static_cast<int>(sCooldownMS)) {
			sLastTime = frameStart;
			sCooldownMS *= 0.99;
			manager->addZombie(spawnZombie());
			return true;
		}
		return false;
	}

	ZombieObj* spawnZombie() { // may want to return a shared ptr
		zombieStats stats;
		// check for where is valid to spawn will later be done by areas 'claimed' by the player (where their units currently are)
		bool spawnPosFound = false;
		Vec2 pos = { 0,0 };
		Vec2 depotPos = manager->getDepot()->getPos();
		while (!spawnPosFound) {
			pos = { (static_cast<float>(rand()) / RAND_MAX) * (worldWidth-16), (static_cast<float>(rand()) / RAND_MAX) * (worldHeight-16) }; // -16 to ensure it doesnt spawn outside the map
			if (getDistance(pos, depotPos) >= sDistanceFromDepot) {
				spawnPosFound = true;
			}
		}
		shared_ptr<ZombieObj> zombie = make_shared<ZombieObj>(pos.x, pos.y, stats.size, stats.size, stats.maxHealth, manager->getNextID(), 0);
		stats.addComponents(zombie, 50.0f);
		zombie->getComponent<nearestComponent>()->setnearbyUnits(manager->getUnitConvoys());
		zombie->getComponent<nearestComponent>()->setnearbyBuildings(manager->getBuildingList());
		zombie->getComponent<nearestComponent>()->setDepot(manager->getDepot());
		return zombie.get();
	}

private:
	LevelManager* manager;
	zombieStats stats;

	Uint32 sLastTime = 0;
	float sCooldownMS = 5000.0; // initial cooldown in seconds (5 seconds)

	int sDistanceFromDepot = 50;
};