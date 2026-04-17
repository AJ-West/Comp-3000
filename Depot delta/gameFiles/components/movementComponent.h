#pragma once
#include "gameFiles/entities/gameObject.h"

#include "gameFiles/components/pathfindingComponent.h"

class movementComponent : public Component {
public:
	virtual void update();

	movementComponent(weak_ptr<GameObject> obj, float objSpeed) : Component(obj), speed(objSpeed){}
	virtual ~movementComponent() {}

	//setters
	void setSpeed(float newSpeed) { speed = newSpeed; }

	//getters
	float getSpeed() { return speed; }

private:
	float speed;
};