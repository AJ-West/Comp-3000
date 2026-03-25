#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/useThroughout/usefulFunctions.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/pathfindingComponent.h"
#include "gameFiles/components/nearestComponent.h"

struct zombieStats {
	const char* art = "draftArt/basicZombie.png";
	int movementSpeed = 25;
	int maxHealth = 250;

	int damage = 5;

	int size = 4; // how many tiles it takes up

	void addComponents(GameObject* zombie, float sightDistance) {
		zombie->AddComponent(make_shared<renderComponent>(zombie, renderer, art));
		zombie->AddComponent(make_shared<movementComponent>(zombie, movementSpeed));
		zombie->AddComponent(make_shared<pathfindingComponent>(zombie, grid));
		zombie->AddComponent(make_shared<nearestComponent>(zombie, sightDistance));
	}
};

struct bruteZombieStats {
	const char* art = "draftArt/bruteZombie.png";
	int movementSpeed = 15;
	int maxHealth = 2500;

	int damage = 20;

	int size = 8; // how many tiles it takes up

	void addComponents(GameObject* zombie, float sightDistance) {
		zombie->AddComponent(make_shared<renderComponent>(zombie, renderer, art));
		zombie->AddComponent(make_shared<movementComponent>(zombie, movementSpeed));
		zombie->AddComponent(make_shared<pathfindingComponent>(zombie, grid));
		zombie->AddComponent(make_shared<nearestComponent>(zombie, sightDistance));
	}
};

struct quickZombieStats {
	const char* art = "draftArt/quickZombie.png";
	int movementSpeed = 50;
	int maxHealth = 150;

	int damage = 10;

	int size = 4; // how many tiles it takes up

	void addComponents(GameObject* zombie, float sightDistance) {
		zombie->AddComponent(make_shared<renderComponent>(zombie, renderer, art));
		zombie->AddComponent(make_shared<movementComponent>(zombie, movementSpeed));
		zombie->AddComponent(make_shared<pathfindingComponent>(zombie, grid));
		zombie->AddComponent(make_shared<nearestComponent>(zombie, sightDistance));
	}
};

class ZombieObj : public GameObject {
public:
	ZombieObj(int x, int y, int width, int height, int health, int id, int zType) : GameObject(x, y, width, height, health), ID(id), zombieType(zType) {}

	void renderHover(SDL_Renderer* renderer) { cout << "render hover"; } // need at a later date to show zombie health on hover

	virtual void updateTargets(shared_ptr<vector<shared_ptr<HumanObj>>> list) {
		getComponent<nearestComponent>()->setnearbyUnits(list);
	}

	void pathToTarget() {
		Vec2 target = { tx,ty };
		SDL_FRect size = getDimensions();
		Vec2 origin = { size.x + size.w / 2, size.y + size.h / 2 };
		if (checkForAttack(target, origin) && targetObject) {
			attack();
		}
		else {
			attacking = false;
			auto pathComp = getComponent<pathfindingComponent>();
			pathComp->computeFlowField(target, origin);
		}
	}

	void attack() {
		if (frameStart - lastAttackTime >= attackCooldownMS) {
			if (targetObject->getHealth() <= 0) {
				targetObject = nullptr;
				getComponent<nearestComponent>()->setNearestUnit(nullptr);
				tx = NULL;
				ty = NULL;
			}
			else {
				targetObject->takeDamage(getZombieDamage());
				lastAttackTime = frameStart;
				attacking = true;
			}
		}
	}

	bool checkForAttack(Vec2 target, Vec2 origin) {
		return (getDistance(target, origin) < 15);
	}

	//unit object functions not needed for zombies

	void onClick() {
		cout << "zombie clicked";
	}

	void clickAway() { // set target pos to clicked position
		cout << "zombie click away";
	}

	//getters
	virtual int getType() { return ZOMBIE; }
	Vec2 getTargetPos() { return Vec2{ tx,ty }; }
	int getID() { return ID; }
	int getZombieType() { return zombieType; }
	int getZombieDamage() {
		switch (zombieType) {
		case BRUTE: {
			bruteZombieStats stat;
			return stat.damage;
			break;
		}
		case QUICK: {
			quickZombieStats stat;
			return stat.damage;
			break;
		}
		default: {
			zombieStats stat;
			return stat.damage;
			break;
		}
		}
	}


	//setters
	virtual void setTarget(float x, float y) {
		tx = x; ty = y;
		pathToTarget();
	};

private:
	int ID;

	Uint32 attackCooldownMS = 1000; // 1 sec
	Uint32 lastAttackTime = 0;

	SDL_FRect iSize{ camera.dimen.x + 10, camera.dimen.y + camera.dimen.h - 75, 50, 50 };
	SDL_FRect tSize{ camera.dimen.x + 60, camera.dimen.y + camera.dimen.h - 75, 100, 50 };

	int zombieType;
};