#pragma once
#include "variables.h"
#include "levelManager.h"

class ZombieSpawner {
public:
	ZombieSpawner(LevelManager* lManager): manager(lManager) {}
	~ZombieSpawner() {}

	bool checkIfSpawn() {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - sLastTime >= static_cast<int>(sCooldownMS)) {
			sLastTime = currentTime;
			sCooldownMS *= 0.9;
			manager->addZombie(spawnZombie());
			return true;
		}
		return false;
	}

	ZombieObj* spawnZombie() {
		zombieStats stats;
		// check for where is valid to spawn will later be done by areas 'claimed' by the player (where their units currently are)
		bool spawnPosFound = false;
		Vec2 pos = { 0,0 };
		Vec2 depotPos = manager->getDepot()->getPos();
		while (!spawnPosFound) {
			pos = { (static_cast<float>(rand()) / RAND_MAX) * worldWidth, (static_cast<float>(rand()) / RAND_MAX) * worldHeight };
			if (getDistance(pos, depotPos) >= sDistanceFromDepot) {
				spawnPosFound = true;
			}
		}
		cout << "Pos X: " << pos.x << '\n';
		cout << "Pos Y: " << pos.y << '\n';
		ZombieObj* zombie = new ZombieObj(pos.x, pos.y, stats.size, stats.size, stats.maxHealth, manager->getNextID());
		stats.addComponents(zombie);
		zombie->getComponent<nearestComponent>()->setnearbyUnits(manager->getUnitConvoys());
		zombie->getComponent<nearestComponent>()->setDepot(manager->getDepot());
		return zombie;
	}

private:
	LevelManager* manager;
	zombieStats stats;

	Uint32 sLastTime = 0;
	float sCooldownMS = 5000.0; // initial cooldown in seconds (5 seconds)

	int sDistanceFromDepot = 50;
};