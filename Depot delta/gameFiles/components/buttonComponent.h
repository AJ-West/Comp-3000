#pragma once
#include <iostream>
#include "gameFiles/entities/gameObject.h"

using namespace std;

class buttonComponent: public Component{
public:
	void update() override {
		owner.lock()->setHovering(isWithinRect());
	}	

	bool isWithinRect() {
		SDL_FRect rect = owner.lock()->getDimensions();
		float x, y;
		getMapScaledMousePos(&x, &y);
		if (x < rect.x) return false;
		if (x > rect.x + rect.w) return false;
		if (y < rect.y) return false;
		if (y > rect.y + rect.h) return false;
		return true;
	}

	buttonComponent(weak_ptr<GameObject> obj) :Component(obj) {}
	~buttonComponent() {}
};