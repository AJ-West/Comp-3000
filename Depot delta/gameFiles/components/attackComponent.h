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
		if (targets) {
			for (auto& potTarget : *targets) {
				if (potTarget) {
					if (potTarget->getAlive()) {
						float distance = getDistance(own, potTarget->getPos());
						if (distance < targetDistance && distance < attackRange) {
							target = potTarget.get();
							targetDistance = distance;
							owner->setTargetObject(target);
							cout << "firing" << '\n';
							return;
						}
					}
				}
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
		if (frameStart - lastAttackTime >= attackCooldownMS) {
			lastAttackTime = frameStart;
			auto rComp = owner->getComponent<resourceComponent>();
			int count = rComp->getResourcesCount(AMMUNITION);
			if (count > 0) {
				rComp->setResourcesCount(AMMUNITION, count - 1);
				target->takeDamage(owner->getDamage() * rComp->getResourcesCount(PERSONNEL));
				soundEffectEngine->play2D("soundEffects/units/gunshot.wav");
				bullets->spawnBullet(owner->getPos(), target->getDimensions());
				owner->setAttacking(true);
				if (!target->getAlive()) {
					target = nullptr;
					owner->setAttacking(false);
					return;
				}
			}
			else {
				owner->setAttacking(false);
			}
		}
		//if (frameStart - lastBulletTime >= owner->getRateOfFire()/5) {
			//lastBulletTime = frameStart;
			//bullets->spawnBullet(owner->getPos(), target->getDimensions());
			//soundEffectEngine->play2D("soundEffects/units/gunshot.wav");
		//}
		renderAttack();
	}

	void renderAttack() {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		Vec2 ownerPos = owner->getPos();
		Vec2 targetPos = target->getPos();
		SDL_RenderLine(renderer, ownerPos.x - camera.dimen.x, ownerPos.y - camera.dimen.y, targetPos.x - camera.dimen.x, targetPos.y - camera.dimen.y);
		bullets->render(renderer);
	}

	//getters
	int getAttackCooldown() { return attackCooldownMS; }

	//setters
	void setPotentialTargets(shared_ptr<vector<shared_ptr<ZombieObj>>> potTargets) { targets = potTargets; }
	void setAttackCooldown(int newCooldown) { attackCooldownMS = newCooldown; }

	attackComponent(GameObject* obj, int damage, int range) : Component(obj), maxDamage(damage), attackRange(range) {	}
	virtual ~attackComponent() {}
private:
	Uint32 lastAttackTime = 0;
	Uint32 lastBulletTime = 0;
	int maxDamage;
	int attackRange;
	int attackCooldownMS = 60;
	int bulletCooldownMS = 12;
	shared_ptr<vector<shared_ptr<ZombieObj>>> targets;
	ZombieObj* target = nullptr;
	bulletHandler* bullets = new bulletHandler();

	float targetDistance = INT_MAX;
};