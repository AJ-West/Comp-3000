#pragma once
#include "GameObject.h"

class movementComponent : public Component {
public:
	virtual void update(GameObject* owner) { // update position based off direction of movement
		SDL_FRect dimensions = owner->getDimensions();
		pair<float, float> target = owner->getTargetPos();
		if (target != pair<float, float>{NULL, NULL}) {
			// Move towards target
			float dx = target.first - (dimensions.x + dimensions.w / 2);
			float dy = target.second - (dimensions.y + dimensions.h / 2);
			float distance = std::sqrt(dx * dx + dy * dy);

			if (distance > 1.0f) { // Avoid jitter when close
				float moveDist = speed;
				if (moveDist > distance) {
					moveDist = distance;
					owner->setTarget(NULL, NULL);
				} // Avoid overshooting
				owner->setDimensions({ dimensions.x + dx / distance * moveDist, dimensions.y + dy / distance * moveDist, dimensions.w, dimensions.h });
			}
		}
	}

	movementComponent(GameObject* obj, float objSpeed) : Component(obj), speed(objSpeed){}
	virtual ~movementComponent() {}

private:
	float speed;
};