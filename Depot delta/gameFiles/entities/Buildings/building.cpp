#include "gameFiles/entities/Buildings/building.h"

BuildingObj::BuildingObj(int x, int y, int width, int height, int health, bool used, int type) : GameObject(x, y, width, height, health), rType(type) {
	setAlive(used);
}

//update all components
void BuildingObj::Update() {
	if (alive) {
		for (auto& pair : getComponents()) {
			pair.second->update(this);
		}
	}
	else {
		getComponent<renderComponent>()->update(this);
		getComponent<buttonComponent>()->update(this);
	}
}

void BuildingObj::onClick() {
	selected = !selected;
}

void BuildingObj::clickAway() {
	getMapScaledMousePos(&tx, &ty);
}

void BuildingObj::renderHover(SDL_Renderer* renderer) {
	auto rComp = getComponent<resourceComponent>();
	if (rComp) {
		SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
		rComp->renderResources(renderer, tSize);
	}
}