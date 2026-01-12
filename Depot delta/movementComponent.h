#pragma once
#include "GameObject.h"

#include "pathfindingComponent.h"

class movementComponent : public Component {
public:
	virtual void update(GameObject* owner) { // update position based off direction of movement
		SDL_FRect dimensions = owner->getDimensions();
		Vec2 target = owner->getTargetPos();
		if (target.x != 0 && target.y != 0) {
			//get flow direction of current cell
			Vec2 direction = owner->getComponent<pathfindingComponent>()->getFlowDirection({ static_cast<float>(div(dimensions.x + dimensions.w / 2, WORLD_TILE_SIZE).quot), static_cast<float>(div(dimensions.y + dimensions.h / 2, WORLD_TILE_SIZE).quot) });
			//cout << "Direction: " << direction.x << "," << direction.y << "\n";
			float dx = target.x - (dimensions.x + dimensions.w / 2);
			float dy = target.y - (dimensions.y + dimensions.h / 2);
			float distance = std::sqrt(dx * dx + dy * dy);
			float moveDist = speed * deltaTime;
			// if current cell is the target cell
			if ((direction.x == 0 && direction.y == 0) || distance < 5.0f) {
				if (distance > 1.0f) { // Avoid jitter when close
					if (moveDist > distance) {
						moveDist = distance;
						owner->setTarget(NULL, NULL);
					} // Avoid overshooting
					owner->setDimensions({ dimensions.x + dx / distance * moveDist, dimensions.y + dy / distance * moveDist, dimensions.w, dimensions.h });
				}
			}
			else {
				dx = direction.x;
				dy = direction.y;
				distance = std::sqrt(dx * dx + dy * dy);
				owner->setDimensions({ dimensions.x + dx / distance * moveDist, dimensions.y + dy / distance * moveDist, dimensions.w, dimensions.h });
			}
			
			
		}
	}

	movementComponent(GameObject* obj, float objSpeed) : Component(obj), speed(objSpeed){}
	virtual ~movementComponent() {}

private:
	float speed;
};