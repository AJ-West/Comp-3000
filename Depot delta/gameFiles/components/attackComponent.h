#pragma once
#include "gameFiles/entities/gameObject.h"
#include <SDL3_image/SDL_image.h>
#include "gameFiles/entities/Units/bulletHandler.h"
#include "gameFiles/entities/Zombies/zombieObject.h"
#include "gameFiles/components/resourceComponent.h"

class attackComponent : public Component {// renderers the object
public:
	virtual void update(GameObject* owner) { // render the current frame
		if (target == nullptr) { checkRange(); }
		else { 
			if (checkResources()) {
				bullets->update();
				attack();
			}
		}
	}

	void checkRange() {
		SDL_FRect pos = owner->getDimensions();
		Vec2 own{ pos.x + pos.w / 2, pos.y + pos.h / 2 };
		for (auto& potTarget : targets) {
			if (potTarget) {
				float distance = getDistance(own, potTarget->getPos());
				if (distance < targetDistance && distance < attackRange) {
					target = potTarget;
					targetDistance = distance;
					owner->setTargetObject(potTarget);
					owner->getComponent<resourceComponent>()->setResourceUsage(AMMUNITION, 5);
					return;
				}
				owner->getComponent<resourceComponent>()->setResourceUsage(AMMUNITION, 0);
			}
		}
	}

	bool checkResources() {
		if (owner->getComponent<resourceComponent>()->getResourcesCount(AMMUNITION) <= 0) {
			return false;
		}
		return true;
	}

	void attack() {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastAttackTime >= attackCooldownMS) {
			lastAttackTime = currentTime;
			target->takeDamage(5);
			if (!target->getAlive()) {
				target = nullptr;
				owner->setAttacking(false);
				return;
			}
			owner->setAttacking(true);
		}
		if (currentTime - lastBulletTime >= bulletCooldownMS) {
			lastBulletTime = currentTime;
			bullets->spawnBullet(owner->getPos(), target->getDimensions());
		}
		renderAttack();
	}

	void renderAttack() {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		Vec2 ownerPos = owner->getPos();
		Vec2 targetPos = target->getPos();
		SDL_RenderLine(renderer, ownerPos.x - camera.dimen.x, ownerPos.y - camera.dimen.y, targetPos.x - camera.dimen.x, targetPos.y - camera.dimen.y);
		bullets->render(renderer);
	}

	void setPotentialTargets(vector<ZombieObj*> potTargets) { targets = potTargets; }

	attackComponent(GameObject* obj, int damage, int range, int cooldown) : Component(obj), maxDamage(damage), attackRange(range), attackCooldownMS(cooldown) {
		bulletCooldownMS = attackCooldownMS / 5;
	}
	virtual ~attackComponent() {}
private:
	Uint32 lastAttackTime = 0;
	Uint32 lastBulletTime = 0;
	int maxDamage;
	int attackRange;
	int attackCooldownMS;
	int bulletCooldownMS;
	vector<ZombieObj*> targets;
	ZombieObj* target = nullptr;
	bulletHandler* bullets = new bulletHandler();

	float targetDistance = INT_MAX;
};