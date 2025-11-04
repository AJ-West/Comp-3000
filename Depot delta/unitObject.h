#pragma once
#include "GameObject.h"
#include "buttonBase.h"

class UnitObj : public GameObject, public BaseButton {
public:
	UnitObj() : GameObject() {}

	virtual void onClick() override {
		selected = !selected;
	}

	void clickAway() {
		SDL_GetMouseState(&x, &y);
		SDL_FRect rect = getDimensions();
		direction = normalise({ x - (rect.x + rect.w / 2), y - (rect.y + rect.h / 2) });
		cout << direction.first << ", " << direction.second << endl;
	}

	//getters
	bool getSelected() { return selected; }
	bool getHovering() { return hovering; }
	pair<float, float> getDirection() { return direction; }
	pair<float, float> getTargetPos() { return pair<float, float>{x,y}; }

	//setters
	void setHovering(bool hover) { hovering = hover; }
	void setDirection(pair<float, float> dir) { direction = dir; }

private:
	bool selected = false;
	bool hovering = false;

	float x = 0;
	float y = 0;

	pair<float, float> direction = { 0,0 };
};