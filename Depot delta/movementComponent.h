#pragma once
#include "GameObject.h"

class movementComponent : public Component {
public:
	virtual void update(GameObject* owner) { // update position based off direction of movement
		SDL_FRect dimensions = owner->getDimensions();
		pair<float, float> direction = owner->getDirection();
		pair<float, float> target = owner->getTargetPos();
		cout << "direction x" << direction.first << " direction y " << direction.second << endl;
		cout << "magx" << mag(dimensions.x - target.first) << " magy " << mag(dimensions.y - target.second) << endl;
		if (direction.first * speed > mag(dimensions.x - target.first) && direction.second * speed > mag(dimensions.y - target.second)) {
			owner->setDimensions({ target.first, target.second, dimensions.w, dimensions.h });
			owner->setDirection({ 0,0 });
		}
		else {
			owner->setDimensions({ dimensions.x + direction.first * speed, dimensions.y + direction.second * speed, dimensions.w, dimensions.h });
		}
		
	}

	movementComponent(GameObject* obj, float objSpeed) : Component(obj), speed(objSpeed){}
	virtual ~movementComponent() {}

private:
	float speed;
};