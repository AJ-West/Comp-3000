#pragma once
#include "GameObject.h"
#include "buttonBase.h"
#include "resourceComponent.h"

class UnitObj : public GameObject, public BaseButton {
public:
	UnitObj(int x, int y, int id) : GameObject(x,y), ID(id) {}

	virtual void onClick() override {
		selected = !selected;
	}

	void clickAway() {
		getScaledMousePos(&tx, &ty);
	}

	void renderHover(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			rComp->renderResources(renderer);
		}
	}

	//getters
	bool getSelected() { return selected; }
	bool getHovering() { return hovering; }
	pair<float, float> getTargetPos() { return pair<float, float>{tx,ty}; }
	int getID() { return ID; }

	//setters
	void setHovering(bool hover) { hovering = hover; }
	virtual void setTarget(float x, float y) { tx = x; ty = y; };

private:
	bool selected = false;
	bool hovering = false;

	float tx = NULL;
	float ty = NULL;

	int ID;
};