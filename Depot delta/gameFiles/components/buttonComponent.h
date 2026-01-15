#pragma once
#include <iostream>
#include "gameFiles/entities/gameObject.h"

using namespace std;

class buttonComponent: public Component{
public:
	void update(GameObject* owner) override {
		owner->setHovering(isWithinRect());
	}	

	bool isWithinRect() {
		SDL_FRect rect = owner->getDimensions();
		float x, y;
		getMapScaledMousePos(&x, &y);
		if (x < rect.x) return false;
		if (x > rect.x + rect.w) return false;
		if (y < rect.y) return false;
		if (y > rect.y + rect.h) return false;
		return true;
	}

	buttonComponent(GameObject* obj) :Component(obj) {}
	~buttonComponent() {}
};