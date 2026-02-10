#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/useThroughout/usefulFunctions.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/pathfindingComponent.h"
#include "gameFiles/components/nearestComponent.h"

class ZombieObj : public GameObject {
public:
	ZombieObj(int x, int y, int width, int height, int health, int id) : GameObject(x, y, width, height, health), ID(id) {}

	void renderHover(SDL_Renderer* renderer) { cout << "render hover"; } // need at a later date to show zombie health on hover

	virtual void updateTargets(shared_ptr<vector<shared_ptr<HumanObj>>> list) {
		getComponent<nearestComponent>()->setnearbyUnits(list);
	}

	//getters
	Vec2 getTargetPos() { return Vec2{ tx,ty }; }
	int getID() { return ID; }

	//setters
	virtual void setTarget(float x, float y) {
		tx = x; ty = y;
		pathToTarget();
	};

	void pathToTarget() {
		Vec2 target = { tx,ty };
		SDL_FRect size = getDimensions();
		Vec2 origin = { size.x + size.w / 2, size.y + size.h / 2 };
		if (checkForAttack(target, origin)) {
			attack();
		}
		else {
			attacking = false;
			auto pathComp = getComponent<pathfindingComponent>();
			pathComp->computeFlowField(target, origin);
		}
	}

	void attack() {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastAttackTime >= attackCooldownMS) { 
			if (targetObject->getHealth() <= 0) {
				targetObject = nullptr;
				tx = NULL;
				ty = NULL;
			}
			else {
				targetObject->takeDamage(5);
				lastAttackTime = currentTime;
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

private:
	int ID;

	Uint32 attackCooldownMS = 1000; // 1 sec
	Uint32 lastAttackTime = 0;

	SDL_FRect iSize{ camera.dimen.x + 10, camera.dimen.y + camera.dimen.h - 75, 50, 50 };
	SDL_FRect tSize{ camera.dimen.x + 60, camera.dimen.y + camera.dimen.h - 75, 100, 50 };
};

struct zombieStats {
	const char* art = "draftArt/basicZombie.png";
	int movementSpeed = 25;
	int maxHealth = 100;

	int size = 4; // how many tiles it takes up

	void addComponents(ZombieObj* zombie) {
		zombie->AddComponent(make_shared<renderComponent>(zombie, renderer, "draftArt/basicZombie.png"));
		zombie->AddComponent(make_shared<movementComponent>(zombie, movementSpeed));
		zombie->AddComponent(make_shared<pathfindingComponent>(zombie, grid));
		zombie->AddComponent(make_shared<nearestComponent>(zombie));
	}
};