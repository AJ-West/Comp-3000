#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/entities/humanObject.h"

void movementComponent::update() { // update position based off direction of movement
	SDL_FRect dimensions = owner.lock()->getDimensions();
	Vec2 target = owner.lock()->getTargetPos();
	if (target.x != 0 && target.y != 0) {
		//get flow direction of current cell
		Vec2 direction = owner.lock()->getComponent<pathfindingComponent>()->getFlowDirection({static_cast<float>(div(dimensions.x + dimensions.w / 2, WORLD_TILE_SIZE).quot), static_cast<float>(div(dimensions.y + dimensions.h / 2, WORLD_TILE_SIZE).quot)});
		float dx = target.x - (dimensions.x + dimensions.w / 2);
		float dy = target.y - (dimensions.y + dimensions.h / 2);
		float distance = std::sqrt(dx * dx + dy * dy);
		float moveDist = speed * deltaTime;
		// if current cell is the target cell
		if ((direction.x == 0 && direction.y == 0) || distance < 5.0f) {
			if (distance > 1.0f) { // Avoid jitter when close
				if (moveDist > distance) {
					moveDist = distance;
					owner.lock()->setTarget(NULL, NULL);
				} // Avoid overshooting
				owner.lock()->setDimensions({dimensions.x + dx / distance * moveDist, dimensions.y + dy / distance * moveDist, dimensions.w, dimensions.h});
			}
			if (owner.lock()->getType() == HUMAN) {
				owner.lock()->getComponent<resourceComponent>()->setResourceChange(FUEL, 0);
			}
		}
		else {
			dx = direction.x;
			dy = direction.y;
			distance = std::sqrt(dx * dx + dy * dy);
			owner.lock()->setDimensions({dimensions.x + dx / distance * moveDist, dimensions.y + dy / distance * moveDist, dimensions.w, dimensions.h});
		}
	}
}